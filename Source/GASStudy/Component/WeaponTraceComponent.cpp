//// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WeaponTraceComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/GameplayTag.h"
#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "Interface/CombatInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UWeaponTraceComponent::UWeaponTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UWeaponTraceComponent::Init(AActor* Actor, FName SocketStart, FName SocketEnd, EWeaponTraceShape WeaponTraceShape)
{
	if (!Actor)
	{
		return false;
	}

	OwnerActor = Actor;
	
	mMeshComp = Actor->FindComponentByClass<USkeletalMeshComponent>();
	if (!mMeshComp)
	{
		return false;
	}

	mAnimInst = mMeshComp->GetAnimInstance();
	if (!mAnimInst)
	{
		return false;
	}

	mWeaponAttachedSocketStart = SocketStart; // SocketStart;
	mWeaponAttachedSocketEnd = SocketEnd;

	SetWeaponTraceShape(WeaponTraceShape);
	return true;
}


bool UWeaponTraceComponent::SetTraceData(float NotifyStartTime, FName CurrentAttackName)
{
	// ANS 의 Begine 에서 호출되는 함수 
	if (NAME_None == mWeaponAttachedSocketStart)
	{
		return false;
	}

	if (NAME_None == mWeaponAttachedSocketEnd)
	{
		return false;
	}

	if (false == SetAnimData())
	{
		return false;
	}

	// mAnimIndx = AnimIdx; 항상 첫번째 애니메이션이 되도록 변경
	mNotifyStartTime = NotifyStartTime;
	mPrevMontagePos = mNotifyStartTime;
	mPrevMeshTransform = mMeshComp->GetComponentTransform();
	AttackName = CurrentAttackName;
	/*
		전프레임과 현재프레임 사이를 연결해주면서 충돌처리를 하는데, 
		프레임이 낮아서 ANS 가 원하는 곳에서 시작하지 않게되면 전프레임과 현재프레임을 이을때 고려해야하는게 많아짐

		그렇기때문에 항상 동일한 곳에서 충돌처리를 시작하게 하기 위해서 강제적으로 시작시점에 충돌체크를 하고, 
		위치를 업데이트 시킴 

		이렇게 해야지 항상 일관되게 동일한 곳에서 충돌처리를 시작한다.
	*/
	{
		FFrameTime FrameTime = mNotifyStartTime * mCurrentMontageFrameRate; // 재생시간 * 프레임레이트 = 현재 재생되는 프레임
		FFrameNumber FrameNumber = FrameTime.GetFrame(); // 현재 재생되는 프레임의 정수 부분 

		FTransform CurrentStartSocket;
		FTransform CurrentEndSocket;
		GetWorldSocketTransform(CurrentStartSocket, CurrentEndSocket, FrameTime, mPrevMeshTransform);

		CollisionTestByTraceShape(CurrentStartSocket.GetLocation(), CurrentEndSocket.GetLocation(), false);
		LineTrace(CurrentEndSocket.GetLocation(), CurrentStartSocket.GetLocation(), true);

		MyDrawLine(CurrentStartSocket.GetLocation(), CurrentEndSocket.GetLocation(), FColor::Red, 2.0f);
		mCurrentStartLocation = CurrentStartSocket.GetLocation();
		mCurrentEndLocation = CurrentEndSocket.GetLocation();

		if (CanDrawDebug)
		{
			MyDrawSphere(CurrentStartSocket.GetLocation(), FColor::Yellow, 2.0f);
			MyDrawSphere(CurrentEndSocket.GetLocation(), FColor::Yellow, 2.0f);
		}
	}

	SetActivate(true);
	return true;
}



void UWeaponTraceComponent::Trace()
{
	if (!mMeshComp)
	{
		return;
	}

	mCurrentMeshTransform = mMeshComp->GetComponentTransform();
	// 소켓 위치 구하기 
	if (mAnimInst->Montage_IsPlaying(mCurrentPlayingMontage))
	{
		// 현재 재생되는 몽타주의 시간대 
		mCurrentMontagePos = mAnimInst->Montage_GetPosition(mCurrentPlayingMontage);

		// 현재 재생되는 시간대 * 1 프레임에 걸리는 시간 = 현재 몽타주 프레임 
		FFrameTime FrameTime = mCurrentMontagePos * mCurrentMontageFrameRate; // 재생시간 * 프레임레이트 = 현재 재생되는 프레임
		FFrameNumber FrameNumber = FrameTime.GetFrame(); // 현재 재생되는 프레임의 정수 부분 
		float SubFrame = FrameTime.GetSubFrame(); // 현재 재생되는 프레임의 소숫점 부분

		float DiffCurrent2Prev = (mCurrentMontagePos - mPrevMontagePos); // 현재 프레임과 전 프레임의 재생된 곳의 차이값 
		FMath::Abs(DiffCurrent2Prev);
		FTransform CurrentStartSocket;
		FTransform CurrentEndSocket;
		GetWorldSocketTransform(CurrentStartSocket, CurrentEndSocket, FrameTime, mCurrentMeshTransform);

		// 프레임 차이 시간 / 보간이 일어나는 단위 시간 => 보간 횟수
		int CurrentLerpCount = FMath::TruncToInt(DiffCurrent2Prev / mlerpInterval); // 소수점은 날려준다.
		//UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentLerpCount);

		/*
			선형보간 : 두점 사의 직선의 형태로 그사이에 존재하는 점들의 위치를 알아낼 수 있음 
		*/

		// 보간횟수에 따른 알파.
		float Alpha = 1.0f / CurrentLerpCount; 
		//for (int i = 1; i <= 1; ++i)		// 보간 x 
		for (int i = 1; i <= CurrentLerpCount; ++i)	// 보간 o
		{
			// 전 프레임 -> 현 프레임으로, i * Alpha 만큼 보간
			float LerpedPos = FMath::Lerp(mPrevMontagePos, mCurrentMontagePos, i * Alpha);
			
			// 보간된 프레임 시간을 FrameRate 와 곱하여, 보간된 프레임타임(정수 + 실수 프레임) 얻기
			FFrameTime LerpedFrameTime =  LerpedPos * mCurrentMontageFrameRate;	

			// Mesh 의 위치를 보간 
			FTransform LerpedMeshTransform;
			LerpedMeshTransform.Blend(mPrevMeshTransform, mCurrentMeshTransform, i * Alpha);

			FTransform LerpedStartSocket;
			FTransform LerpedEndSocket;
			
			// 
			GetWorldSocketTransform(LerpedStartSocket, LerpedEndSocket, LerpedFrameTime, LerpedMeshTransform);
			CollisionTestByTraceShape(LerpedStartSocket.GetLocation(), LerpedEndSocket.GetLocation(), false);

			mPrevStartLocation = mCurrentStartLocation;
			mPrevEndLocation = mCurrentEndLocation;
			mCurrentStartLocation = LerpedStartSocket.GetLocation();
			mCurrentEndLocation = LerpedEndSocket.GetLocation();

			ConnectLineToLine();

			if (CanDrawDebug)
			{
				MyDrawSphere(LerpedStartSocket.GetLocation(), FColor::Yellow, 2.0f);
				MyDrawSphere(LerpedEndSocket.GetLocation(), FColor::Yellow, 2.0f);
			}
		}

		// 현재 위치는 진작 구했지만 여기서하는 이유는,
		// 높은 확률로 매 Trace 함수를 호출할때마다 내부에서 보간처리를 하게 됨.
		// 보간 처리는 결국 현재 구한 소켓의 위치보다 이전 위치이기 때문에 여기서 처리한다.
		{
			CollisionTestByTraceShape(CurrentStartSocket.GetLocation(), CurrentEndSocket.GetLocation(), false);
			LineTrace(CurrentEndSocket.GetLocation(), CurrentStartSocket.GetLocation(), true);

			mPrevStartLocation = mCurrentStartLocation;
			mPrevEndLocation = mCurrentEndLocation;
			mCurrentStartLocation = CurrentStartSocket.GetLocation();
			mCurrentEndLocation = CurrentEndSocket.GetLocation();

			ConnectLineToLine();
			if (CanDrawDebug)
			{
				MyDrawSphere(CurrentStartSocket.GetLocation(), FColor::Red, 2.0f);
				MyDrawSphere(CurrentEndSocket.GetLocation(), FColor::Red, 2.0f);
			}
		}
	}

	mPrevMontagePos = mCurrentMontagePos;
	mPrevMeshTransform = mCurrentMeshTransform;
}



void UWeaponTraceComponent::Reset()
{
	//mNotifyStartTime = 0.f;
	//mCurrentMontagePos = 0.f; // 현재 몽타주 재생 위치 
	//mPrevMontagePos = 0.f; // 이전 몽타주 재생 위치 

	//mOneFPSInterval = 0.0; // 1 프레임에 걸리는 시간 ex) 120 fps => 1 / 120 
	//mlerpInterval = 0.0; // 1 프레임에 3 번 보간한다면, 몇초에 한번씩 일어나야하는지 ex) 1 / 120 / mInterpCount 


	mCurrentStartLocation = FVector::Zero();
	mCurrentEndLocation = FVector::Zero();
	mPrevStartLocation = FVector::Zero();
	mPrevEndLocation = FVector::Zero();

	mHitDetectedActor.Empty();

	SetActivate(false);
}



bool UWeaponTraceComponent::SetAnimData()
{
	mCurrentPlayingMontage = mAnimInst->GetCurrentActiveMontage();

	//if (nullptr == mCurrentPlayingMontage)
	//{
	//	return false;
	//}

	//auto DataModel_1 = mCurrentPlayingMontage->GetDataModelInterface(); // TScriptInterface - ObjectPtr + InterfaceePtr
	//auto DataModel_2 = DataModel_1.GetInterface(); // Interface
	
	// 3 과 4 는 Ojbect 형식으로 가져온다 
	// 오브젝트는 인터페이스 함수를 호출할 수 없으므로 현재 필요한게 아니다.
	//auto DataModel_3 = DataModel_1.GetObject();	// ObjectPtr 동일 
	//auto DataModel_4 = DataModel_1.GetObjectRef(); // ObjectPtr 동일 
	
	// 2 와  4 는 동일하게 Interface 형식을 가져온다
	//auto DataModel_5 = mCurrentPlayingMontage->GetDataModel(); // 동일 

	/*DataModel_1->GetNumberOfFrames();
	DataModel_2->GetNumberOfFrames();
	DataModel_5->GetNumberOfFrames();*/

	// 모두 DataModel 로 이동해서 처리됨 
	// 현재 트랙 정보가 없어서 출력이 안되는데 그 이유가 뭘까?

	/*float CurrentMontagePos = mAnimInst->Montage_GetPosition(Montage);
	FFrameRate FrameRate = DataModel_1->GetFrameRate();
	FFrameTime FrameTime = CurrentMontagePos * FrameRate;*/


	/*FTransform T1 = DataModel_1->EvaluateBoneTrackTransform(FName("hand_r"), 10, EAnimInterpolationType::Linear);
	FTransform T2 =DataModel_2->EvaluateBoneTrackTransform(FName("hand_r"), 10, EAnimInterpolationType::Linear);
	FTransform T3 = DataModel_5->EvaluateBoneTrackTransform(FName("hand_r"), 10, EAnimInterpolationType::Linear);*/
	//DataModel_3->GetNumberOfFrames();		// 오브젝트 타입으로 현재 필요하지 않다
	//DataModel_4->GetNumberOfFrames();		// 오브젝트 타입으로 현재 필요하지 않다


	// 1 ~ 5 는 동일한 AnimDataModel 이다
	//
	// 몽타주는 SlotAnimTracks 에 몽타의 슬롯에 저장된 애니메이션 정보들을 가지고 있음 

	// 몽타주에 애니메이션 슬롯에 애니메이션이 하나만 있다는 전재를 둔다면
	// 
	// ANS 에서 몇번째 애니메이션인지 설정할 수 있게만들면 
	// 아래의 번거로운 과정을 피할 수 있음 
	auto Segment = mCurrentPlayingMontage->SlotAnimTracks[0].AnimTrack.AnimSegments[mAnimIndx];

	//for (const FSlotAnimationTrack& SlotTrack : Montage->SlotAnimTracks)
	{
		// SlotTrack 에는 여러 애니메이션중 하나를 의미함 
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *SlotTrack.SlotName.ToString());

		// FSlotAnimationTrack 의 AnimTrack / AnimTrack 의 AnimSegments
		// 애님세그먼트는 애니메이션이 무엇이고 어떻게 될지 정의
		//for (const FAnimSegment& Segment : SlotTrack.AnimTrack.AnimSegments)
		{
			//if ((CurrentMontageTime >= Segment.StartPos) && (CurrentMontageTime < (Segment.StartPos + Segment.AnimEndTime)))
			{

				UAnimSequenceBase* USB = Segment.GetAnimReference();
				//UE_LOG(LogTemp, Display, TEXT("애님시퀀스 이름 : %s"), *(USB->GetName()));

				//MontageStartPos = Segment.StartPos;

				if (nullptr == USB)
				{
					return false;
				}

				//auto DataModel = /*Cast<UAnimDataModel>*/(USB->GetDataModel());

			/*	UAnimSequence* Seq = Cast<UAnimSequence>(USB);
				if (!Seq) return false;

				float Length = Seq->GetPlayLength();
				int32 Frames = Seq->GetNumberOfSampledKeys();

				float FrameRate = Frames / Length;
				mOneFPSInterval = 1.0f / FrameRate;
				mlerpInterval = mOneFPSInterval / mInterpCount;*/

				// DetaModel_6 는 시퀀서데이터모데일로 처리됨 
				mDataModelInterface = USB->GetDataModelInterface();

				if (nullptr == mDataModelInterface.GetInterface())
				{
					return false;
				}

				// 애니메이션이 시작되는 부분 
				// 이정보가 필요한 이유는 몽타주에서 항상 애니메이션을 처음부터 재생하지 않기때문..!
				// 120 fps 로 만들어진 경우에는 
				// 분모 1, 분자 120 으로 만들어진다. 분모 / 분자 -> 1 / 120 -> 한프레임에 걸리는 시간 
				mCurrentMontageFrameRate = mDataModelInterface->GetFrameRate(); 
				// 한프레임 걸리는 시간 
				mOneFPSInterval = mCurrentMontageFrameRate.AsInterval();
				// 한프레임 사이를 mInterpCount 횟수로 보간한다면, 몇초에 한번씩 진행해야 하는지
				// ex) mOneFPSInterval 가 0.016 이고 이시간에 3 번 보간한다면, 0.005 정도가 된다.
				mlerpInterval = mOneFPSInterval / mInterpCount;

				mCurrentMontageFrameRate = mDataModelInterface->GetFrameRate();
				mOneFPSInterval = mCurrentMontageFrameRate.AsInterval();
				mlerpInterval = mOneFPSInterval / mInterpCount;
			}
		}
	}

	return true;
}

void UWeaponTraceComponent::AccumulateParent2RootBoneTransform(FTransform& AccumulatedLocalTransform, const FFrameTime& FrameTime)
{

	FName CurrentBone = GetOwner()->ActorHasTag(TEXT("Player")) ? TEXT("Weapon_r") : TEXT("hand_r");
	//FName CurrentBone("Weapon_r");
	FName Root("root");

	// 루트는 제외해야함 
	// 실질적으로 루트는 Pelvis 임 

	while ((CurrentBone != NAME_None) && (CurrentBone != Root))
	{
		// FrameTime의 BoneTrackTransform 을 얻어옴
		FTransform BoneLocalTransform = mDataModelInterface->EvaluateBoneTrackTransform(CurrentBone, FrameTime, EAnimInterpolationType::Linear);
		// 부모 → 자식 순서로 누적
		AccumulatedLocalTransform = AccumulatedLocalTransform * BoneLocalTransform;
		CurrentBone = mMeshComp->GetParentBone(CurrentBone);
	}
}

void UWeaponTraceComponent::LineTrace(FVector Start, FVector End, bool Debug)
{
	TArray<FHitResult> HitResults;
	TArray<FHitResult> HitResultsFiltered;

	FCollisionObjectQueryParams CollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel3);
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT_NAME_ONLY("Weapon Trace"), false, OwnerActor.Get());
	FCollisionResponseParams CollisionResponseParams;
	FCollisionShape CollisionShape;
	
	// bool Result = GetWorld()->LineTraceMultiByObjectType(HitResults, Start, End, CollisionObjectQueryParams, CollisionQueryParams);
	//bool Result = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, CollisionShape, CollisionQueryParams);
	bool Result = GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel3, CollisionQueryParams);
	if (Result)
	{
		for (const auto& HitResult : HitResults)
		{
			const FString& HitActorName = HitResult.GetActor()->GetName();

			// 존재할경우
			if (mHitDetectedActor.Contains(*HitActorName))
			{
				continue;
			}
			else
			{
				DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, 10.0f);
				UE_LOG(LogTemp, Warning, TEXT("Hit Detected Actor : %s"), *HitActorName);
				
				mHitDetectedActor.Emplace(*HitActorName);
				// mOnWeaponHit.Broadcast(FAttackHitResult(OwnerActor.Get(), HitResult.GetActor(), HitResult));
				HitResultsFiltered.Add(HitResult);
			}
		}

		if (!HitResultsFiltered.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("출력 시작"));
			for (const auto& HitResult : HitResultsFiltered)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
			}
			UE_LOG(LogTemp, Warning, TEXT("출력 종료"));
			mOnDetecedHit.Broadcast(HitResultsFiltered);
		}
	}


	if (CanDrawDebug)
	{
		MyDrawLine(Start, End, FColor::Blue, 2.f);
	}
}

void UWeaponTraceComponent::CapsuleTrace(FVector Start, FVector End, bool Debug)
{
	TArray<FHitResult> HitResults;
	TArray<FHitResult> HitResultsFiltered;

	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT_NAME_ONLY("Weapon Trace"), false, OwnerActor.Get());
	float CapsuleRadius = 30.f;
	float CapsuleHalfHeight = (FVector::Distance(Start, End) * 0.5f);
	
	FVector CapsuleDirection = (End - Start).GetSafeNormal();
	FQuat CapsuleRotation = FQuat::FindBetweenNormals(FVector::UpVector, CapsuleDirection);

	bool Result = GetWorld()->SweepMultiByChannel(HitResults, Start, End, CapsuleRotation, ECollisionChannel::ECC_EngineTraceChannel3, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), CollisionQueryParams);
	if (Result)
	{
		for (const auto& HitResult : HitResults)
		{
			const FString& HitActorName = HitResult.GetActor()->GetName();

			// 존재할경우
			if (mHitDetectedActor.Contains(*HitActorName))
			{
				continue;
			}
			else
			{
				DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, 10.0f);
				UE_LOG(LogTemp, Warning, TEXT("Hit Detected Actor : %s"), *HitActorName);
				
				mHitDetectedActor.Emplace(*HitActorName);
				// mOnWeaponHit.Broadcast(FAttackHitResult(OwnerActor.Get(), HitResult.GetActor(), HitResult));
				HitResultsFiltered.Add(HitResult);
			}
		}

		if (!HitResultsFiltered.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("출력 시작"));
			for (const auto& HitResult : HitResultsFiltered)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
			}
			UE_LOG(LogTemp, Warning, TEXT("출력 종료"));
			mOnDetecedHit.Broadcast(HitResultsFiltered);
		}
	}

	if (CanDrawDebug)
	{
		MyDrawCapsule(Start, End, CapsuleHalfHeight, CapsuleRadius, CapsuleRotation, FColor::Red, 1.0f);
	}
}

void UWeaponTraceComponent::MyDrawLine(FVector Start, FVector End, FColor Color, float Time)
{
	DrawDebugLine(GetWorld(), Start, End, Color, false, Time);
}

void UWeaponTraceComponent::MyDrawSphere(FVector Center, FColor Color, float Time)
{
	DrawDebugSphere(GetWorld(), Center, 2.0f, 12, Color, false, Time);
}

void UWeaponTraceComponent::MyDrawCapsule(FVector Start, FVector End, float HalfHeight, float Radius, FQuat Rotation, FColor Color, float Time)
{
	FVector Center = Start + (End - Start) / 2.f;

	float HalfSize = (End - Start).Size() / 2.f;

	DrawDebugCapsule(GetWorld(), Center, HalfSize, Radius, Rotation, Color, false, Time);
}

void UWeaponTraceComponent::ConnectLineToLine()
{
	// 트레이스 타입이 라인인 경우에만 삼각형을 만든다.
	//if (EWeaponTraceShape::Line != mWeaponTraceShape)
	//{
	//	return;
	//}
	//float Dist = FVector::Dist(mPrevEndLocation, mCurrentEndLocation);
	//if (Dist > 30.f)
	//	return;

	FVector CurrentMid = (mCurrentStartLocation + mCurrentEndLocation) / 2.0f;
	FVector PrevMid = (mPrevStartLocation + mPrevEndLocation) / 2.0f;

	DrawDebugLine(GetWorld(), mPrevEndLocation, mCurrentEndLocation, FColor::Red, false, 2.0f);

	// 삼각형 만들기부터..
	LineTrace(mPrevEndLocation, mCurrentEndLocation, false);
	LineTrace(mCurrentEndLocation, PrevMid, false);
	LineTrace(PrevMid, CurrentMid, false);
	LineTrace(CurrentMid, mPrevStartLocation, false);
	LineTrace(mPrevStartLocation, mCurrentStartLocation, false);

}

void UWeaponTraceComponent::GetWorldSocketTransform(FTransform& Start, FTransform& End, const FFrameTime& FrameTime, const FTransform& ToWorld)
{
	// 소켓 위치 
	FTransform Socket_Start = mMeshComp->GetSocketTransform(mWeaponAttachedSocketStart, RTS_ParentBoneSpace);
	FTransform Socket_End = mMeshComp->GetSocketTransform(mWeaponAttachedSocketEnd, RTS_ParentBoneSpace);

	FTransform AccumulatedLocalTransform = FTransform::Identity;

	// 부모 뼈대 -> 전체 계층구조 
	AccumulateParent2RootBoneTransform(AccumulatedLocalTransform, FrameTime);

	// Socket_Start * AccumulatedLocalTransform 로 얻어지는 것은 로컬공간 기준
	// 월드의 위치로 변환시키위해서 Mesh 의 월드 Transform 을 곱해준다
	Start = Socket_Start * AccumulatedLocalTransform * ToWorld;
	End = Socket_End * AccumulatedLocalTransform * ToWorld;
}

void UWeaponTraceComponent::SetWeaponTraceShape(EWeaponTraceShape NewShape)
{
	mWeaponTraceShape = NewShape;

	switch (mWeaponTraceShape)
	{
	case EWeaponTraceShape::Line:
		SetInterpCount(/*ETraceType::LINE*/EIntervalTraceCountByTraceShape::LINE);
		break;
	case EWeaponTraceShape::Capsule:
		SetInterpCount(/*ETraceType::CAPSULE*/EIntervalTraceCountByTraceShape::CAPSULE);
		break;
	}
}

void UWeaponTraceComponent::SetInterpCount(int32 NewCount)
{
	mInterpCount = NewCount;
}

void UWeaponTraceComponent::CollisionTestByTraceShape(FVector Start, FVector End, bool Debug)
{
	switch (mWeaponTraceShape)
	{
	case EWeaponTraceShape::Line:
		LineTrace(Start, End, Debug);
		break;
	case EWeaponTraceShape::Capsule:
		CapsuleTrace(Start, End, Debug);
		break;
	}
}
//
//
//
// 
// Shipping
// 
// 
// 
//#include "Component/WeaponTraceComponent.h"
//#include "AbilitySystemBlueprintLibrary.h"
//#include "Tag/GameplayTag.h"
//#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
//#include "Interface/CombatInterface.h"
//#include "GameplayTagAssetInterface.h"
//#include "Kismet/GameplayStatics.h"
//#include "GameFramework/Character.h"
//#include "DrawDebugHelpers.h"
//
//UWeaponTraceComponent::UWeaponTraceComponent()
//{
//	PrimaryComponentTick.bCanEverTick = false;
//}
//
//bool UWeaponTraceComponent::Init(AActor* Actor, FName SocketStart, FName SocketEnd, EWeaponTraceShape WeaponTraceShape)
//{
//	if (!Actor) return false;
//
//	OwnerActor = Actor;
//	mMeshComp = Actor->FindComponentByClass<USkeletalMeshComponent>();
//	if (!mMeshComp) return false;
//
//	mAnimInst = mMeshComp->GetAnimInstance();
//	if (!mAnimInst) return false;
//
//	mWeaponAttachedSocketStart = SocketStart;
//	mWeaponAttachedSocketEnd = SocketEnd;
//
//	SetWeaponTraceShape(WeaponTraceShape);
//	return true;
//}
//
//bool UWeaponTraceComponent::SetTraceData(float NotifyStartTime, FName CurrentAttackName)
//{
//	if (NAME_None == mWeaponAttachedSocketStart || NAME_None == mWeaponAttachedSocketEnd) return false;
//
//	// 패키징을 위해 에디터 전용인 SetAnimData() 대신 런타임 변수 초기화
//	mNotifyStartTime = NotifyStartTime;
//	mPrevMontagePos = mNotifyStartTime;
//	mPrevMeshTransform = mMeshComp->GetComponentTransform();
//	AttackName = CurrentAttackName;
//
//	mHitDetectedActor.Empty();
//
//	// 초기 트랜스폼 설정 (에디터 데이터 모델 대신 현재 메시의 소켓 상태 사용)
//	LastStartTransform = mMeshComp->GetSocketTransform(mWeaponAttachedSocketStart, RTS_World);
//	LastEndTransform = mMeshComp->GetSocketTransform(mWeaponAttachedSocketEnd, RTS_World);
//
//	mCurrentStartLocation = LastStartTransform.GetLocation();
//	mCurrentEndLocation = LastEndTransform.GetLocation();
//
//	// 시작점 충돌 처리
//	CollisionTestByTraceShape(mCurrentStartLocation, mCurrentEndLocation, false);
//
//	if (CanDrawDebug)
//	{
//		//MyDrawSphere(mCurrentStartLocation, FColor::Yellow, 2.0f);
//		//MyDrawSphere(mCurrentEndLocation, FColor::Yellow, 2.0f);
//	}
//
//	SetActivate(true);
//	return true;
//}
//
//void UWeaponTraceComponent::Trace()
//{
//	if (!mMeshComp || !mAnimInst) return;
//
//	mCurrentMeshTransform = mMeshComp->GetComponentTransform();
//	mCurrentPlayingMontage = mAnimInst->GetCurrentActiveMontage();
//
//	if (mCurrentPlayingMontage && mAnimInst->Montage_IsPlaying(mCurrentPlayingMontage))
//	{
//		mCurrentMontagePos = mAnimInst->Montage_GetPosition(mCurrentPlayingMontage);
//
//		// 실제 월드 소켓 트랜스폼 획득
//		FTransform CurrentStartSocket = mMeshComp->GetSocketTransform(mWeaponAttachedSocketStart, RTS_World);
//		FTransform CurrentEndSocket = mMeshComp->GetSocketTransform(mWeaponAttachedSocketEnd, RTS_World);
//
//		// 보간 횟수 결정 (mInterpCount 사용)
//		int32 CurrentLerpCount = FMath::Max(1, mInterpCount);
//		float AlphaStep = 1.0f / (float)CurrentLerpCount;
//
//		for (int i = 1; i <= CurrentLerpCount; ++i)
//		{
//			float Alpha = i * AlphaStep;
//
//			// 트랜스폼 보간 (위치는 Lerp, 회전은 Slerp로 자동 처리됨)
//			FTransform LerpedStart;
//			LerpedStart.Blend(LastStartTransform, CurrentStartSocket, Alpha);
//
//			FTransform LerpedEnd;
//			LerpedEnd.Blend(LastEndTransform, CurrentEndSocket, Alpha);
//
//
//			// 에디터용 비주얼 디버깅 (궤적 그리기)
//#if WITH_EDITOR
//			if (CanDrawDebug)
//			{
//				// 이전 보간 지점과 현재 보간 지점을 연결하여 궤적을 시각화
//				// Tip(끝점) 궤적은 빨간색, Base(시작점) 궤적은 녹색
//				DrawDebugLine(GetWorld(), mCurrentEndLocation, LerpedEnd.GetLocation(), FColor::Red, false, 1.0f, 0, 1.0f);
//				DrawDebugLine(GetWorld(), mCurrentStartLocation, LerpedStart.GetLocation(), FColor::Green, false, 1.0f, 0, 1.0f);
//				// 무기 날의 현재 보간 상태를 파란색으로 표시
//				DrawDebugLine(GetWorld(), LerpedStart.GetLocation(), LerpedEnd.GetLocation(), FColor::Blue, false, 1.0f, 0, 0.5f);
//			}
//#endif
//
//
//			// 보간된 위치에서 충돌 테스트
//			CollisionTestByTraceShape(LerpedStart.GetLocation(), LerpedEnd.GetLocation(), false);
//
//			// 궤적 연결을 위한 위치 갱신
//			mPrevStartLocation = mCurrentStartLocation;
//			mPrevEndLocation = mCurrentEndLocation;
//			mCurrentStartLocation = LerpedStart.GetLocation();
//			mCurrentEndLocation = LerpedEnd.GetLocation();
//
//			// 궤적 사이의 면(삼각형) 채우기
//			ConnectLineToLine();
//
//			if (CanDrawDebug)
//			{
//				//MyDrawSphere(mCurrentStartLocation, FColor::Yellow, 1.0f);
//				//MyDrawSphere(mCurrentEndLocation, FColor::Yellow, 1.0f);
//			}
//		}
//
//		// 다음 프레임을 위해 저장
//		LastStartTransform = CurrentStartSocket;
//		LastEndTransform = CurrentEndSocket;
//		mPrevMontagePos = mCurrentMontagePos;
//		mPrevMeshTransform = mCurrentMeshTransform;
//	}
//}
//
//void UWeaponTraceComponent::Reset()
//{
//	mCurrentStartLocation = FVector::ZeroVector;
//	mCurrentEndLocation = FVector::ZeroVector;
//	mPrevStartLocation = FVector::ZeroVector;
//	mPrevEndLocation = FVector::ZeroVector;
//
//	mHitDetectedActor.Empty();
//
//	SetActivate(false);
//}
//
//// 패키징 호환을 위해 기능을 비우거나 런타임 데이터로 교체
//bool UWeaponTraceComponent::SetAnimData()
//{
//	// 런타임에선 DataModelInterface를 사용할 수 없으므로 true만 반환하거나 
//	// 필요한 최소한의 몽타주 정보만 할당합니다.
//	return true;
//}
//
//void UWeaponTraceComponent::LineTrace(FVector Start, FVector End, bool Debug)
//{
//	TArray<FHitResult> HitResults;
//	TArray<FHitResult> HitResultsFiltered;
//
//	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, OwnerActor.Get());
//
//	// 프로젝트의 트레이스 채널에 맞춰 ECC_GameTraceChannel3 등으로 수정 가능
//	bool Result = GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, CollisionQueryParams);
//
//	if (Result)
//	{
//		for (const auto& HitResult : HitResults)
//		{
//			AActor* HitActor = HitResult.GetActor();
//			if (!HitActor) continue;
//
//			FName HitActorName = HitActor->GetFName();
//
//			if (mHitDetectedActor.Contains(HitActorName)) continue;
//
//			mHitDetectedActor.Add(HitActorName);
//			HitResultsFiltered.Add(HitResult);
//
//			if (CanDrawDebug)
//			{
//				DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, 2.0f);
//			}
//		}
//
//		if (HitResultsFiltered.Num() > 0)
//		{
//			mOnDetecedHit.Broadcast(HitResultsFiltered);
//		}
//	}
//
//	if (CanDrawDebug)
//	{
//		//MyDrawLine(Start, End, FColor::Blue, 1.0f);
//	}
//}
//
//void UWeaponTraceComponent::CapsuleTrace(FVector Start, FVector End, bool Debug)
//{
//
//	///////////////////////////////////////////////
//	TArray<FHitResult> HitResults;
//	TArray<FHitResult> HitResultsFiltered;
//
//	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT_NAME_ONLY("Weapon Trace"), false, OwnerActor.Get());
//	float CapsuleRadius = 30.f;
//	float Dist = FVector::Distance(Start, End);
//	float CapsuleHalfHeight = (Dist * 0.5f) + CapsuleRadius;
//
//	FVector Dir = (End - Start).GetSafeNormal();
//	FVector Center = (Start + End) * 0.5f;
//	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(Dir).ToQuat();
//
//
//	bool Result = GetWorld()->SweepMultiByChannel(HitResults, Start, End, CapsuleRotation, ECollisionChannel::ECC_EngineTraceChannel3, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), CollisionQueryParams);
//	//bool Result = GetWorld()->SweepMultiByChannel(HitResults, Center, Center, CapsuleRotation, ECC_Visibility, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), CollisionQueryParams);
//	if (Result)
//	{
//		for (const auto& HitResult : HitResults)
//		{
//			const FString& HitActorName = HitResult.GetActor()->GetName();
//		
//			if (mHitDetectedActor.Contains(*HitActorName))
//			{
//				continue;
//			}
//			else
//			{
//				mHitDetectedActor.Emplace(*HitActorName);
//				HitResultsFiltered.Add(HitResult);
//			}
//		}
//		
//		if (!HitResultsFiltered.IsEmpty())
//		{
//			UE_LOG(LogTemp, Warning, TEXT("출력 시작"));
//			for (const auto& HitResult : HitResultsFiltered)
//			{
//				UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
//			}
//			UE_LOG(LogTemp, Warning, TEXT("출력 종료"));
//			mOnDetecedHit.Broadcast(HitResultsFiltered);
//		}
//	}
//
//
//#if WITH_EDITOR
//	//MyDrawCapsule(Start, End, CapsuleHalfHeight, CapsuleRadius, CapsuleRotation, FColor::Red, 1.0f);
//
//	FVector CCenter = Start + (End - Start) / 2.f;
//	float HalfSize = (End - Start).Size() / 2.f;
////	DrawDebugCapsule(GetWorld(), Center, HalfSize, Radius, Rotation, Color, false, Time);
//	DrawDebugCapsule(GetWorld(), CCenter, HalfSize, CapsuleRadius, CapsuleRotation, FColor::Red, false, 1.f);
//#endif
//
//}
//
//void UWeaponTraceComponent::ConnectLineToLine()
//{
//	if (EWeaponTraceShape::Line != mWeaponTraceShape) return;
//	if (mPrevEndLocation.IsNearlyZero() || mCurrentEndLocation.IsNearlyZero()) return;
//
//	FVector CurrentMid = (mCurrentStartLocation + mCurrentEndLocation) / 2.0f;
//	FVector PrevMid = (mPrevStartLocation + mPrevEndLocation) / 2.0f;
//
//	// 궤적 사이를 메우는 사선 트레이스
//	LineTrace(mPrevEndLocation, mCurrentEndLocation, false);
//	LineTrace(mCurrentEndLocation, PrevMid, false);
//	LineTrace(PrevMid, CurrentMid, false);
//	LineTrace(CurrentMid, mPrevStartLocation, false);
//	LineTrace(mPrevStartLocation, mCurrentStartLocation, false);
//
//#if WITH_EDITOR
//	if (CanDrawDebug)
//	{
//		// 궤적 내부를 메우는 선들을 아주 잠깐씩 그려서 면의 흐름을 확인
//		DrawDebugLine(GetWorld(), mPrevEndLocation, mCurrentStartLocation, FColor::White, false, 0.5f, 0, 0.2f);
//	}
//#endif
//}
//
//// 패키징 대응을 위해 함수 형식을 유지하되 내부 로직은 불필요하므로 비워둠
//void UWeaponTraceComponent::GetWorldSocketTransform(FTransform& Start, FTransform& End, const FFrameTime& FrameTime, const FTransform& ToWorld)
//{
//	// 이 함수는 이제 Trace() 내부에서 GetSocketTransform(RTS_World)로 대체되었습니다.
//}
//
//void UWeaponTraceComponent::SetWeaponTraceShape(EWeaponTraceShape NewShape)
//{
//	mWeaponTraceShape = NewShape;
//	SetInterpCount(5); // 기본 보간 횟수 설정
//}
//
//void UWeaponTraceComponent::SetInterpCount(int32 NewCount)
//{
//	mInterpCount = NewCount;
//}
//
//void UWeaponTraceComponent::CollisionTestByTraceShape(FVector Start, FVector End, bool Debug)
//{
//	//switch (mWeaponTraceShape)
//	//{
//	//case EWeaponTraceShape::Line:
//	//	LineTrace(Start, End, Debug);
//	//	break;
//	//case EWeaponTraceShape::Capsule:
//	//	CapsuleTrace(Start, End, Debug);
//	//	break;
//	//}
//	CapsuleTrace(Start, End, Debug);
//}