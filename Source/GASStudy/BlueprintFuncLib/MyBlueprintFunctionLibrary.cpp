// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

EHitDirection UMyBlueprintFunctionLibrary::CalculateDirection(const AActor* StandardActor, const AActor* TargetActor)
{
	FVector Target = TargetActor->GetActorLocation() - StandardActor->GetActorLocation();
	Target = Target.GetSafeNormal();
	FVector Fwd = StandardActor->GetActorForwardVector();

	float DotValue = Fwd.Dot(Target);
	float Angle = FMath::Acos(DotValue);
	Angle = FMath::RadiansToDegrees(Angle);

	// 이 값의 결과에 따라서 왼쪽  오른쪽을 결정하게됨 (왼손좌표계고려)
	// 0 보다 작으면 -> 왼쪽
	// 0 보다 크면 -> 오른쪽
	FVector CrossValue = Fwd.Cross(Target).GetSafeNormal();
	float IsLeft = CrossValue.Dot(FVector::UpVector);

	if (0.0f <= Angle && Angle <= 60.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("Angle : %f 앞"), Angle);
		return EHitDirection::Fwd;
	}
	// 뒤 
	else if (120.0f <= Angle && Angle <= 180.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("Angle : %f 뒤"), Angle);
		return EHitDirection::Back;
	}
	else if (IsLeft < 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Angle : %f 왼"), Angle);
		return EHitDirection::Left;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Angle : %f 오"), Angle);
		return EHitDirection::Right;
	}
}

EHitDirectionArea UMyBlueprintFunctionLibrary::CalculateDirectionToLocation(const AActor* StandardActor, const FVector& TargetLocation)
{
	//// 1. 타겟 위치를 캐릭터의 로컬 좌표계로 변환 (X: 앞/뒤, Y: 좌/우)
	//FVector LocalTargetPos = StandardActor->GetTransform().InverseTransformPosition(TargetLocation);
	//LocalTargetPos.Z = 0;
	//LocalTargetPos.Normalize();

	//// 2. 각도 구하기 (-180 ~ 180)
	//float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(LocalTargetPos.Y, LocalTargetPos.X));

	//// 3. 4사분면 판정 (각 영역은 90도씩 차지함)
	//// 우상단 (FrontRight): 0 ~ 90도 사이
	//if (AngleDeg >= 0.f && AngleDeg < 90.f)
	//{
	//	return EHitDirectionArea::FrontRight;
	//}
	//// 우하단 (BackRight): 90 ~ 180도 사이
	//else if (AngleDeg >= 90.f && AngleDeg <= 180.f)
	//{
	//	return EHitDirectionArea::BackRight;
	//}
	//// 좌상단 (FrontLeft): -90 ~ 0도 사이
	//else if (AngleDeg >= -90.f && AngleDeg < 0.f)
	//{
	//	return EHitDirectionArea::FrontLeft;
	//}
	//// 좌하단 (BackLeft): -180 ~ -90도 사이
	//else
	//{
	//	return EHitDirectionArea::BackLeft;
	//}

	if (!StandardActor)
	{
		return EHitDirectionArea::FrontRight;
	}

	// 1. 기준 벡터들
	const FVector Forward = StandardActor->GetActorForwardVector();
	const FVector Right = StandardActor->GetActorRightVector();

	// 2. 타겟 방향 벡터
	FVector ToTarget = TargetLocation - StandardActor->GetActorLocation();
	ToTarget.Z = 0.f;

	// 3. 길이 체크 (0 벡터 방지)
	if (ToTarget.IsNearlyZero())
	{
		return EHitDirectionArea::FrontRight;
	}

	ToTarget.Normalize();

	// 4. Dot 계산
	const float ForwardDot = FVector::DotProduct(Forward, ToTarget);
	const float RightDot = FVector::DotProduct(Right, ToTarget);

	// 5. 사분면 판정
	if (ForwardDot >= 0.f)
	{
		if (RightDot >= 0.f)
		{
			return EHitDirectionArea::FrontRight; // 우상단
		}
		else
		{
			return EHitDirectionArea::FrontLeft;  // 좌상단
		}
	}
	else
	{
		if (RightDot >= 0.f)
		{
			return EHitDirectionArea::BackRight;  // 우하단
		}
		else
		{
			return EHitDirectionArea::BackLeft;   // 좌하단
		}
	}
}

EHitDirectionArea UMyBlueprintFunctionLibrary::CalculateDirectionToHitResult(const AActor* StandardActor, const FHitResult& HitResult)
{
	if (!StandardActor)
	{
		return EHitDirectionArea::FrontRight;
	}

	// 1. 기준 벡터 (Z 제거해서 평면 기준)
	FVector Forward = StandardActor->GetActorForwardVector();
	FVector Right = StandardActor->GetActorRightVector();

	Forward.Z = 0.f;
	Right.Z = 0.f;

	Forward.Normalize();
	Right.Normalize();

	// 2. 공격 방향 벡터 (핵심)
	// ImpactNormal은 "표면에서 밀려나오는 방향"이라 반대로 뒤집어야 공격 방향이 됨
	FVector AttackDir = -HitResult.ImpactNormal;
	AttackDir.Z = 0.f;

	// 3. 예외 처리 (노말이 이상한 경우 대비)
	if (AttackDir.IsNearlyZero())
	{
		// fallback: 위치 기반
		AttackDir = HitResult.ImpactPoint - StandardActor->GetActorLocation();
		AttackDir.Z = 0.f;

		if (AttackDir.IsNearlyZero())
		{
			return EHitDirectionArea::FrontRight;
		}
	}

	AttackDir.Normalize();

	// 4. Dot 계산
	const float ForwardDot = FVector::DotProduct(Forward, AttackDir);
	const float RightDot = FVector::DotProduct(Right, AttackDir);

	// 5. 사분면 판정 (절대 튀지 않는 구조)
	if (ForwardDot >= 0.f)
	{
		if (RightDot >= 0.f)
		{
			return EHitDirectionArea::FrontRight; // 우상단
		}
		else
		{
			return EHitDirectionArea::FrontLeft;  // 좌상단
		}
	}
	else
	{
		if (RightDot >= 0.f)
		{
			return EHitDirectionArea::BackRight;  // 우하단
		}
		else
		{
			return EHitDirectionArea::BackLeft;   // 좌하단
		}
	}
}

//FGameplayAbilityTargetDataHandle UMyBlueprintFunctionLibrary::MakeTargetDataHandleforAnimation(const FGameplayAbilityTargetData_AnimationInfo& Info)
//{
//	// 우리의 Target Data 타입을 생성합니다.  
//	// 핸들은 소멸될 때 데이터를 자동으로 정리하고 삭제합니다.  
//	// 만약 이 데이터를 핸들에 추가하지 않는다면 메모리 관리와 메모리 누수 문제가 발생할 수 있으니,  
//	// 안전하게 프레임 내 어느 시점에라도 항상 핸들에 추가하는 것이 좋습니다!
//	FGameplayAbilityTargetData_Animation* MyCustomData = new FGameplayAbilityTargetData_Animation(Info);
//	// 구조체의 정보를 설정하여 입력된 이름과 우리가 원하는 다른 변경 사항을 적용합니다.
//
//	// Blueprint에서 사용할 핸들 래퍼를 만듭니다.
//	FGameplayAbilityTargetDataHandle Handle;
//	// 타겟 데이터를 핸들에 추가합니다.
//	Handle.Add(MyCustomData);
//	// 핸들을 Blueprint로 출력합니다.
//	return Handle;
//}
//
//FGameplayAbilityTargetData_Animation UMyBlueprintFunctionLibrary::GetTargetDataAnimation(const FGameplayAbilityTargetDataHandle& Handle, int32 Index)
//{
//	// NOTE: ::Get(int32 Index) 함수에는 두 가지 버전이 있습니다;
//   // 1) const 버전은 const FGameplayAbilityTargetData*를 반환하며, Target Data 값을 읽기에 적합합니다.
//   // 2) non-const 버전은 FGameplayAbilityTargetData*를 반환하며, Target Data 값을 수정하기에 적합합니다.
//	const FGameplayAbilityTargetData* Data = Handle.Get(Index); // 이는 인덱스를 유효성 검사해줍니다.
//
//	// 사용할 수 있는 데이터가 있는지 확인, null 데이터는 캐스팅할 수 없음을 의미합니다.
//	if (Data == nullptr)
//	{
//		// 여기에 
//		return FGameplayAbilityTargetData_Animation();
//	}
//
//	// 이것은 기본적으로 타입 검사 단계입니다. static_cast는 타입 안전성이 없기 때문에 이 검사를 수행합니다.
//	// 이 검사를 하지 않으면 구조체가 객체 슬라이싱되어 타입을 확인할 방법이 없어집니다.
//	if (Data->GetScriptStruct() == FGameplayAbilityTargetData_Animation::StaticStruct())
//	{
//		// 이제 캐스팅을 하는 부분입니다. 이미 올바른 타입임을 알기 때문에 안심하고 캐스팅할 수 있습니다.
//		const FGameplayAbilityTargetData_Animation* CustomData = static_cast<const FGameplayAbilityTargetData_Animation*>(Data);
//		return *CustomData;
//	}
//
//	return FGameplayAbilityTargetData_Animation();
//}
//
//FGameplayAbilityTargetDataHandle UMyBlueprintFunctionLibrary::MakeTargetDataHandleforEnemyAttack(const FGameplayAbilityTargetData_EnemyAttackInfo& Info)
//{
//	// 우리의 Target Data 타입을 생성합니다.  
//	// 핸들은 소멸될 때 데이터를 자동으로 정리하고 삭제합니다.  
//	// 만약 이 데이터를 핸들에 추가하지 않는다면 메모리 관리와 메모리 누수 문제가 발생할 수 있으니,  
//	// 안전하게 프레임 내 어느 시점에라도 항상 핸들에 추가하는 것이 좋습니다!
//	FGameplayAbilityTargetData_EnemyAttack* MyCustomData = new FGameplayAbilityTargetData_EnemyAttack(Info);
//	// 구조체의 정보를 설정하여 입력된 이름과 우리가 원하는 다른 변경 사항을 적용합니다.
//
//	// Blueprint에서 사용할 핸들 래퍼를 만듭니다.
//	FGameplayAbilityTargetDataHandle Handle;
//	// 타겟 데이터를 핸들에 추가합니다.
//	Handle.Add(MyCustomData);
//	// 핸들을 Blueprint로 출력합니다.
//	return Handle;
//}
//
//FGameplayAbilityTargetData_EnemyAttack UMyBlueprintFunctionLibrary::GetTargetDataEnemyAttack(const FGameplayAbilityTargetDataHandle& Handle, int32 Index)
//{
//	// NOTE: ::Get(int32 Index) 함수에는 두 가지 버전이 있습니다;
//   // 1) const 버전은 const FGameplayAbilityTargetData*를 반환하며, Target Data 값을 읽기에 적합합니다.
//   // 2) non-const 버전은 FGameplayAbilityTargetData*를 반환하며, Target Data 값을 수정하기에 적합합니다.
//	const FGameplayAbilityTargetData* Data = Handle.Get(Index); // 이는 인덱스를 유효성 검사해줍니다.
//
//	// 사용할 수 있는 데이터가 있는지 확인, null 데이터는 캐스팅할 수 없음을 의미합니다.
//	if (Data == nullptr)
//	{
//		// 여기에 
//		return FGameplayAbilityTargetData_EnemyAttack();
//	}
//
//	// 이것은 기본적으로 타입 검사 단계입니다. static_cast는 타입 안전성이 없기 때문에 이 검사를 수행합니다.
//	// 이 검사를 하지 않으면 구조체가 객체 슬라이싱되어 타입을 확인할 방법이 없어집니다.
//	if (Data->GetScriptStruct() == FGameplayAbilityTargetData_EnemyAttack::StaticStruct())
//	{
//		// 이제 캐스팅을 하는 부분입니다. 이미 올바른 타입임을 알기 때문에 안심하고 캐스팅할 수 있습니다.
//		const FGameplayAbilityTargetData_EnemyAttack* CustomData = static_cast<const FGameplayAbilityTargetData_EnemyAttack*>(Data);
//		return *CustomData;
//	}
//
//	return FGameplayAbilityTargetData_EnemyAttack();
//}
//
