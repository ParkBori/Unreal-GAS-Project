// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TargetLockComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/OverlapResult.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Shared/Tag/SharedTag.h"
#include "Shared/Macro/Log.h"


// Sets default values for this component's properties
UTargetLockComponent::UTargetLockComponent()
{
	//TODO("어빌리티 시스템 / 블루프린라이브러리 태그에셋인터페이스 캐스팅이 너무 많이 일어난다. 추후, 타겟의 어빌리티시스템을 캐싱하여 캐스팅 횟수를 줄일 수 있도록 수정이 필요하다");
}

// Called when the game starts
void UTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();

	// 컴포넌트의 오너는 자주 사용될 수 있으므로, 캐싱 
	OwnerActor = GetOwner();
	OwnerPawn = Cast<APawn>(OwnerActor);
	OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	OwnerCamera = OwnerActor->FindComponentByClass<UCameraComponent>();
	check(OwnerCamera);
}

void UTargetLockComponent::SetOwnerRotator()
{
	SetControllRotationOnTarget();
}

AActor* UTargetLockComponent::TraceTargetByForward(FVector FwdDir)
{
	// TODO : 타겟 검출 
	// 스피어 콜리전을 활용해서, LockRadiut 반경 내부에 있는 적들을 탐색

	// 임시타겟 목록 
	TMap< float, AActor*> TracedActorsMap;

	FVector PlayerLocation = OwnerActor->GetActorLocation();

	TArray<struct FHitResult> OutHits;
	FCollisionQueryParams CollisionQueryParams(FName("CheckInCircleRange"), false, OwnerActor.Get());
	if (TargetActor.Get())
	{
		CollisionQueryParams.AddIgnoredActor(TargetActor.Get());
	}
	FCollisionObjectQueryParams CollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);

	bool HitResult = GetWorld()->SweepMultiByObjectType(OutHits, PlayerLocation, PlayerLocation, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(LockRadius), CollisionQueryParams);
	//
	if (HitResult)
	{
		for (const FHitResult& OutHit : OutHits)
		{
			AActor* TracedActor = OutHit.GetActor();

			// 이부분이 살짝 애매한데..죽은 적은 콜리전이 꺼지기 때문에... 애당초 죽은 적은 탐색이 안되는게 맞음 
			// 하지만 혹시 몰라서 IsAlive 를 체크중
			if (TargetIsAlive(TracedActor) && CheckTargetIsInPlayerSight(TracedActor, 90, false, FwdDir))
			{
				float Player2TargetDist = FVector::Dist(TracedActor->GetActorLocation(), PlayerLocation);

				TracedActorsMap.Add(Player2TargetDist, TracedActor);
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), PlayerLocation, LockRadius, 30, FColor::Red, false, CollisionTestRate);

	FVector OnwerFwd = OwnerActor->GetActorForwardVector();
	FVector RightVector = OnwerFwd.RotateAngleAxis(PlayerSight / 2.0f, FVector::UpVector) * LockRadius;
	FVector LeftVector = OnwerFwd.RotateAngleAxis(-PlayerSight / 2.0f, FVector::UpVector) * LockRadius;

	DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + RightVector, FColor::Red, false, CollisionTestRate, CollisionTestRate);
	DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + LeftVector, FColor::Red, false, CollisionTestRate, CollisionTestRate);
#endif
	
	if (false == TracedActorsMap.IsEmpty())
	{
		return TracedActorsMap.begin()->Value;
	}

	return nullptr;
}

void UTargetLockComponent::TargetLockSwitch(bool Switch)
{
	if (Switch)
	{
		TargetLockOn();
		// 폰이 Controller 의 Yaw 값과 동기화 시켜주면, 별도의 회전 보간없이 타겟을 바라보게 된다.
		
	}
	else
	{
		TargetLockOff();
	}

}

bool UTargetLockComponent::CheckObsatacleBetweenOwnerAndTarget(AActor* CheckTarget)
{
	// TODO 
	/*
	* 플레이어와 타겟 사이의 장애물 체크 
	*/
	if (nullptr == CheckTarget)
	{
		return false;
	}

	FVector OwnerLocation = OwnerActor->GetActorLocation();

	//FVector CameraLocation = OwnerCamera->GetComponentLocation();
	FCollisionQueryParams CollisionQueryParams(FName("CheckPlayer2Target"), false, OwnerActor.Get());
	FHitResult OutHit;
	// 내 캐릭터와 타겟의 사이에 장애물 체크 
	bool Result = GetWorld()->LineTraceSingleByChannel(OutHit, OwnerLocation, CheckTarget->GetActorLocation(), ECollisionChannel::ECC_Camera, CollisionQueryParams);

#if ENABLE_DRAW_DEBUG
	//DrawDebugLine(GetWorld(), CameraLocation, Target->GetActorLocation(), FColor::Green, false, 1.0f);
#endif

	// 현재 타겟을 가로막는 물체가 있다면, 이는 시야 범위에 존재하지 않는것으로 판단. 
	// 이렇게 하면, 두 마리의 적이 앞뒤로 나란히 있일 경우 플레이어 방향
	if (OutHit.GetActor() != CheckTarget)
	{
		return true;
	}

	return false;
}

void UTargetLockComponent::TargetLockOff_IsObstacle()
{
	// 가로막고 있는 물체가 있을경우 제거
	bHasObstacleBetweenOwnerAndTarget = false;
	
	if (CheckObsatacleBetweenOwnerAndTarget(TargetActor.Get()))
	{
		TargetLockOff();
	}
}

void UTargetLockComponent::TargetLockOn()
{
	// TODO : TargetLock 초기 설정 

	FindTarget();

	if (TargetActor.IsValid())
	{
		OwnerPawn->bUseControllerRotationYaw = true;

		// 타겟 Lock Handler 활성화 (타겟 유효성 + 회전로직)
		if (false == LockOnTimerHandle.IsValid())
		{
			LockOnHandler(true);
		}

		// 컨트롤러 Look 무시 
		if (OwnerPlayerController.IsValid())
		{
			OwnerPlayerController->SetIgnoreLookInput(true);
		}

		SetOwnerStarfe(true);

		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(TargetActor.Get(), FGameplayTagContainer(State::State_Targeted));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Isnt"));
	}
}

void UTargetLockComponent::TargetLockOff()
{
	// TODO : TargetLock 해제 설정  

	if (TargetActor.IsValid())
	{
		OwnerPawn->bUseControllerRotationYaw = false;
		
		LockOnHandler(false);

		// 타겟 Lock Handler 비활성화 
		LockOnHandler(false);

		// 컨트롤러 Look 해제 
		if (OwnerPlayerController.IsValid())
		{
			OwnerPlayerController->ResetIgnoreLookInput();
		}
		
		SetOwnerStarfe(false);

		TargetActor = nullptr;

	/*	FTimerHandle WidgetOff;
		GetWorld()->GetTimerManager().SetTimer(WidgetOff, []() 
			{
				UPlayerHUD* HUD = CUIManager::GetInst()->FindWidget<UPlayerHUD>(TEXT("PlayerHUD"));
				if (IsValid(HUD))
				{
					HUD->SetVisible(false);
				}
			}, 5.0f, false);*/
	
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(TargetActor.Get(), FGameplayTagContainer(State::State_Targeted));
	}
}

void UTargetLockComponent::SetControllRotationOnTarget()
{
	if (false == OwnerPlayerController.IsValid())
	{
		return;
	}
	
	if (!TargetActor.IsValid())
	{
		return;
	}

	const FRotator ControlRotation = GetControlRotationOnTarget();
	/*if (OnTargetSetRotation.IsBound())
	{
		OnTargetSetRotation.Broadcast(TargetActor, ControlRotation);
	}*/
	//else
	{
		OwnerPlayerController->SetControlRotation(ControlRotation);
	}
}

FRotator UTargetLockComponent::GetControlRotationOnTarget() const
{
	if (false == OwnerPlayerController.IsValid() || !TargetActor.IsValid())
	{
		return FRotator::ZeroRotator;
	}

	auto* TargetMesh = TargetActor->GetMesh();
	if (!TargetMesh)
	{
		return FRotator::ZeroRotator;
	}

	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();
	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector OtherActorLocation = TargetMesh->DoesSocketExist(mTargetSocketName) ? 
		TargetMesh->GetSocketLocation(mTargetSocketName) : TargetActor->GetActorLocation();

	// Find look at rotation
	const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, OtherActorLocation);
	float Pitch = LookRotation.Pitch;
	
	FRotator TargetRotation;
	if (bAdjustPitchBasedOnDistanceToTarget)
	{
		const float DistanceToTarget = GetDistanceFromOwner(TargetActor.Get());
		const float PitchInRange = (DistanceToTarget * PitchDistanceCoefficient + PitchDistanceOffset) * -1.0f;
		const float PitchOffset = FMath::Clamp(PitchInRange, PitchMin, PitchMax);

		Pitch = Pitch + PitchOffset;
		TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);
	}
	else
	{
		if (bIgnoreLookInput)
		{
			TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);
		}
		else
		{
			TargetRotation = FRotator(ControlRotation.Pitch, LookRotation.Yaw, ControlRotation.Roll);
		}
	}

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
}

float UTargetLockComponent::GetDistanceFromOwner(const AActor* OtherActor) const
{
	// 타겟과의 거리 반환 
	return OwnerActor->GetDistanceTo(OtherActor);
}

void UTargetLockComponent::SearchLeftRight(Direction InDirection)
{
	// 임시 배열 목록 생성
	TArray<TObjectPtr< AActor>> DirectionTargets;
	
	// 다시 트레이스 
	TArray<AActor*> Targets = TraceSphereRangeTargets();

	// 트레이스 된것을 기준으로 좌우 구별하기 
	FVector OwnerLocation = OwnerActor->GetActorLocation();
	FVector Owner2TargetVector = (TargetActor->GetActorLocation() - OwnerLocation).GetSafeNormal();

	for (AActor* Target : Targets)
	{
		if (Target != TargetActor)
		{
			FVector TargetVector = (Target->GetActorLocation() - OwnerLocation).GetSafeNormal();

			// 이 값의 결과에 따라서 왼쪽  오른쪽을 결정하게됨 (왼손좌표계고려)
			// 0 보다 작으면 -> 왼쪽
			// 0 보다 크면 -> 오른쪽
			float Dir = Owner2TargetVector.Cross(TargetVector).Dot(FVector::UpVector);
			
			if (InDirection == Direction::Left)
			{
				if (Dir < 0)
				{
					DirectionTargets.Add(Target);
				}
			}
			else
			{
				if (Dir > 0)
				{
					DirectionTargets.Add(Target);
				}
			}
		}
	}


	// 현재 캐릭터가 바라보는 방향중에서 제일 각도차이가 적은 액터 
	DirectionTargets.Sort([&](const TObjectPtr<AActor>& A1, const TObjectPtr<AActor>&  A2)
		{
			float Dot1 = ((A1->GetActorLocation() - OwnerLocation).GetSafeNormal()).Dot(Owner2TargetVector);
			UE_LOG(LogTemp, Warning, TEXT("A1 Name : %s / %f"), *A1->GetName(), Dot1);
			float Dot2 = ((A2->GetActorLocation() - OwnerLocation).GetSafeNormal()).Dot(Owner2TargetVector);
			UE_LOG(LogTemp, Warning, TEXT("A2 Name : %s / %f"), *A2->GetName(), Dot2);
			return Dot1 > Dot2;
		});


	if (DirectionTargets.Num() > 0)
	{
		AActor* ClosestActor = DirectionTargets[0];
		if (ClosestActor)
		{
			TargetActor = Cast<ACharacter>(ClosestActor);

			InterpSpeed = 1.0f;

			if (TargetChangedTargetHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(TargetChangedTargetHandle);
				TargetChangedTargetHandle.Invalidate();
			}
			GetWorld()->GetTimerManager().SetTimer(TargetChangedTargetHandle, FTimerDelegate::CreateLambda([&]() { InterpSpeed = 9.0; }), 1.0f, false);
		}
	}
	else
	{
		FString dir;
		InDirection == Direction::Left ? dir = TEXT("Left") : dir = TEXT("Right");

		UE_LOG(LogTemp, Display, TEXT("In %s, Don't Exist Character"), *dir);
	}
}

TArray<AActor*> UTargetLockComponent::TraceSphereRangeTargets(bool CheckPlayerSight)
{
	TArray<AActor*> TracedActorsMap;

	// 현재 플레이어의 위치 
	FVector PlayerLocation = OwnerActor->GetActorLocation();

	// 타겟락 가능한 주변 반경을 탐색 
	TArray<FHitResult> OutHits;
	FCollisionQueryParams CollisionQueryParams(FName("CheckInCircleRange"), false, OwnerActor.Get());
	if (TargetActor.IsValid())
	{
		CollisionQueryParams.AddIgnoredActor(TargetActor.Get());
	}

	FCollisionObjectQueryParams CollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);

	bool HitResult = GetWorld()->SweepMultiByObjectType(OutHits, PlayerLocation, PlayerLocation, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(LockRadius), CollisionQueryParams);
	
	TArray<FOverlapResult> OverlapResults;
	HitResult = GetWorld()->OverlapMultiByObjectType(OverlapResults, PlayerLocation, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(LockRadius), CollisionQueryParams);

	// DrawDebugSphere(GetWorld(), )
	
	//
	if (HitResult)
	{
		for (const FOverlapResult& OutHit : OverlapResults)
		{
			AActor* TracedActor = OutHit.GetActor();

			// 이부분이 살짝 애매한데..죽은 적은 콜리전이 꺼지기 때문에... 애당초 죽은 적은 탐색이 안되는게 맞음 
			// 하지만 혹시 몰라서 IsAlive 를 체크중
			// 주변 적이 살아있는지, 시야 범위내부에 존재하는지 확인

			// 플레이어 사이에 장애물 있는지 확인 
			bool IsObstacleBetween = CheckObsatacleBetweenOwnerAndTarget(TracedActor);
			// 플레이어 시야범위에 있는지 확인 
			
			if (CheckPlayerSight)
			{
				bool IsInPlayerSight = CheckTargetIsInPlayerSight(TracedActor, PlayerSight);
				if (false == IsInPlayerSight)
					continue;
			}

			if (TargetIsAlive(TracedActor) && (false == IsObstacleBetween))
			{
				float Player2TargetDist =  GetDistanceFromOwner(TracedActor);
				TracedActorsMap.Add(TracedActor);
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	// 주변 환경 구모양 탐색
	DrawDebugSphere(GetWorld(), PlayerLocation, LockRadius, 30, FColor::Red, false, CollisionTestRate);

	FVector OnwerFwd = OwnerActor->GetActorForwardVector();
	FVector RightVector = OnwerFwd.RotateAngleAxis(PlayerSight / 2.0f, FVector::UpVector) * LockRadius;
	FVector LeftVector = OnwerFwd.RotateAngleAxis(-PlayerSight / 2.0f, FVector::UpVector) * LockRadius;

	// 시야범위 라인 탐색 
	DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + RightVector, FColor::Red, false, CollisionTestRate, CollisionTestRate);
	DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + LeftVector, FColor::Red, false, CollisionTestRate, CollisionTestRate);
#endif

	return TracedActorsMap;
}


void UTargetLockComponent::FindTarget()
{
	// TODO : 타겟 검출 
	// 스피어 콜리전을 활용해서, LockRadiut 반경 내부에 있는 적들을 탐색


	TArray<AActor*> FindedTargets = TraceSphereRangeTargets(true);
	// 시야 범위 내에서 가까운 순으로 정렬
	FindedTargets.Sort([&](const TObjectPtr<AActor>& A1, const TObjectPtr<AActor>& A2)
		{
			return GetDistanceFromOwner(A1) < GetDistanceFromOwner(A2);
		});
	

	// 위의 과정을 통해 검출된 적이 있을 경우
	if (false == FindedTargets.IsEmpty())
	{
		// 가장 가까운 적 (맵에 제일 앞에 있는 액터)
		TargetActor = Cast<ACharacter>(FindedTargets[0]);
		if (TargetActor.Get())
		{
			LockOnHandler(true);
		}
	}
}

void UTargetLockComponent::CheckTargetHandler_V2(bool Switch)
{
	if (Switch)
	{
		GetWorld()->GetTimerManager().SetTimer(CheckTargetTimerHandle, FTimerDelegate::CreateUObject(this, &UTargetLockComponent::CheckTargetIsValid_V2), CollisionTestRate, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckTargetTimerHandle);
		CheckTargetTimerHandle.Invalidate();
	}
}

void UTargetLockComponent::CheckTargetIsValid_V2()
{
	// TODO : 타겟이 유효한지 판단 

	FVector OwnerLocation = OwnerActor->GetActorLocation();

	if (TargetActor.IsValid())
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		float Dist = FVector::Dist(OwnerLocation, TargetLocation);
		bool IsAlive = TargetIsAlive(TargetActor.Get());
		bool IsInPlayerSight = CheckTargetIsInPlayerSight(TargetActor.Get(), PlayerSight);
		if (IsAlive && Dist <= LockRadius /*&& IsInPlayerSight*/)
		{
#if ENABLE_DRAW_DEBUG
			DrawDebugSphere(GetWorld(), TargetLocation, 22, 15, FColor::Green, false, CollisionTestRate);
#endif
			SetOwnerStarfe(true);
			return;
		}
	}

	// 현재 타겟이 유효하지 않음
	// 새로운 유효성 체크 
	SetOwnerStarfe(false);
	CheckTargetHandler_V2(false);
	TargetActor = nullptr;
}

bool UTargetLockComponent::CheckTargetIsInPlayerSight(AActor* CheckTarget, float PlayerSightRange, bool UseOwnerFwdVector, FVector NewFwdVector)
{
	float DrawDebugTime = 100;
	// TODO : 
	/*
	* 타겟으로 들어온 플레이어가 시야 범위 내에 존재하는지 확인 
	*/

	// 현재 시야범위 경계 값 
	// 플레이어가 전방 벡터와, 플레이어 -> 타겟 벡터가 일치하면, 두각은 0 도로 Cos 값은 1 이 된다.
	// 즉 PlayerSightDotValue  보다  Dot(플레이어 전방벡터, 플레이어 -> 타겟) 값이 커야 시야범위 내에 존재한다고 판단한다.
	
	float PlayerSightDotValue = FMath::Cos(FMath::DegreesToRadians(PlayerSightRange / 2.0f));
	float DotToDegree = FMath::RadiansToDegrees(FMath::Acos(PlayerSightDotValue));
	FVector OwnerLocation = OwnerActor->GetActorLocation();

	// 플레이어와 타겟 사이의 각도 구하기. 
	FVector BaseFwdVector;
	
	if (UseOwnerFwdVector)
	{
		BaseFwdVector = OwnerActor->GetActorForwardVector();
		//UE_LOG(LogTemp, Warning, TEXT("Character FWD : %s"), *BaseFwdVector.ToString());

	}
	else
	{
		BaseFwdVector = NewFwdVector;
		DrawDebugLine(GetWorld(), OwnerActor->GetActorLocation(), OwnerActor->GetActorLocation() + BaseFwdVector * 1000.f, FColor::Blue, false, DrawDebugTime);
	}

	FVector Player2TargetDir = (CheckTarget->GetActorLocation() - OwnerLocation).GetSafeNormal();
	//DrawDebugLine(GetWorld(), OwnerLocation, OwnerLocation + Player2TargetDir * 1000.f, FColor::Yellow, false, DrawDebugTime);

	float Player2TargetDotValue = FVector::DotProduct(BaseFwdVector, Player2TargetDir);

	if (PlayerSightDotValue >= Player2TargetDotValue)
	{
		return false;
	}

	return true;
}

void UTargetLockComponent::SetOwnerStarfe(bool Switch)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwnerActor);
	if (CombatInterface)
	{
		if (Switch)
		{
			CombatInterface->SetStrafe(true);
		}
		else
		{
			CombatInterface->SetStrafe(false);
		}
	}
}

void UTargetLockComponent::LockOnHandler(bool Switch)
{
	// 주변 적을 탐색하게 함.
	if (Switch)
	{
		GetWorld()->GetTimerManager().SetTimer(LockOnTimerHandle, FTimerDelegate::CreateUObject(this, &UTargetLockComponent::LockOn), 0.001, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(LockOnTimerHandle);
		LockOnTimerHandle.Invalidate();
	}
}

void UTargetLockComponent::LockOn()
{
	// 현재 타겟이 유효한지 확인 
	bool IsAlive = TargetIsAlive(TargetActor.Get());
	float Dist = GetDistanceFromOwner(TargetActor.Get());
	bool IsObstacleBetween = CheckObsatacleBetweenOwnerAndTarget(TargetActor.Get());
	//bool IsInPlayerSight = CheckTargetIsInPlayerSight(TargetActor.Get(), PlayerSight);

	if ((false == IsAlive) /*|| (false == IsInPlayerSight)*/ || (Dist > LockRadius))
	{
		TargetLockOff();
		return;
	}


	// bHasObstacleBetweenOwnerAndTarget 해당 값은 처음에 false 로 설정되어있어서
	// 타겟이 범위 밖에 존재한다면, 해당 트리거 딱 한번 들어올 수 있으며,
	// TargetLockOff_IsObstacle 에서 다시 false 가 되어서
	// 결과적으로 TargetRemoveDelayByObstaclet 시간 후에 다시 체크해서 
	// 타겟 사이에 장애물이 없다면, 다시 락온될수 있게끔 됨
	if (true == IsObstacleBetween && !bHasObstacleBetweenOwnerAndTarget)
	{
		bHasObstacleBetweenOwnerAndTarget = true;
		GetWorld()->GetTimerManager().SetTimer(TargetRemoveByObstacleTimerHandle, this, &UTargetLockComponent::TargetLockOff_IsObstacle, TargetRemoveDelayByObstacle, false);
	}

	// 닷지하는 동안 Lock 으로 쫓아가면 
	// 플레이어가 공격을하면서 회전해버림 
	// 락온을 못하게 하되, 끝나는 시점에서,
	// 다시 LockOn을 하지만, 쫓아가는 속도를 천천히 해야함 
	//IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(TargetActor);
	//if (TagInterface)
	//{
	//	if (TagInterface->HasMatchingGameplayTag(State::State_Targeted))
	//	{
	//		return;
	//	}
	//}

	// 유요할 경우 
	SetControllRotationOnTarget();
	//DrawDebugSphere(GetWorld(), TargetActor->GetActorLocation(), 20, 5, FColor::Green, false);
}

AActor* UTargetLockComponent::GetTargetActor() const
{
	return TargetActor.Get();
}

bool UTargetLockComponent::TargetIsAlive(AActor* Target)
{
	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Target);
	if (!TagInterface)
	{
		return false;
	}

	return !TagInterface->HasMatchingGameplayTag(State::State_Dead);
}

FRotator UTargetLockComponent::GetLockOnCameraRotation()
{
	FVector CameraLocation = OwnerCamera->GetComponentLocation();
	// 이것도 따지면 캐스트이기 때문에 
	// 너무 많으 캐스팅이 이루어지고있음.
	ICombatInterface* CharacterUtilInterface = Cast<ICombatInterface>(TargetActor);
	FVector TargetLocation = CharacterUtilInterface != nullptr ? CharacterUtilInterface->GetLockPoint() : TargetActor->GetActorLocation();

	float Offset = (FVector::Dist(OwnerActor->GetActorLocation(), TargetLocation)) / 3.0f;
	FVector FinalTargetLocation(TargetLocation.X, TargetLocation.Y, TargetLocation.Z - Offset);
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, FinalTargetLocation);

	return CameraRotation;
}

void UTargetLockComponent::SwitchTargetLeft()
{
	if (TargetActor.IsValid())
	{
		SearchLeftRight(Direction::Left);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Can Use Exist TargetedCharacter"));
	}
}

void UTargetLockComponent::SwitchTargetRight()
{
	if (TargetActor.IsValid())
	{
		SearchLeftRight(Direction::Right);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Can Use Exist TargetedCharacter"));
	}
}
