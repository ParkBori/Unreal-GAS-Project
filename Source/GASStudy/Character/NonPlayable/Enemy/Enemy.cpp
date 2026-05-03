// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NonPlayable/Enemy/Enemy.h"


#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BlueprintFuncLib/MyBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"
#include "Tag/GameplayTag.h"


#include "Component/EnemyCombatComponent.h"
#include "MotionWarpingComponent.h"
#include "Component/WeaponTraceComponent.h"

#include "Weapon/WeaponBase.h"
#include "Shared/Tag/SharedTag.h"


#include "AI/EnemyController.h"

#include "Component/QuestTargetComponent.h"

AEnemy::AEnemy()
{
	GetMesh()->SetCollisionProfileName(TEXT("CharacterBody"), true);

	// AIControllerClass = 해당 객체를 조정할 클래스 
	mMotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	mEnemyCombatComp = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("CombatComponent"));
	mEnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));
	mWeaponTraceComp = CreateDefaultSubobject<UWeaponTraceComponent>(TEXT("WeaponTraceComponent"));
	mQuestTargetComp = CreateDefaultSubobject<UQuestTargetComponent>(TEXT("QuestTargetComponent"));
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();





	SpawnWeapon();
}

void AEnemy::SetStrafe(bool Value)
{
	GetCharacterMovement()->bOrientRotationToMovement = Value;
}

void AEnemy::SpawnWeapon()
{
	if (!mWeaponClass)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;
	mCurrentEquipWeapon = GetWorld()->SpawnActor<AWeaponBase>(mWeaponClass, Params);

	mCurrentEquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, mCurrentEquipWeapon->GetAttachSocket());
}


void AEnemy::EventDeath(const FGameplayTag Tag, int32 NewCount)
{
	Super::EventDeath(Tag, NewCount);

	AEnemyController* EC = GetController<AEnemyController>();
	if (!EC)
	{
		return;
	}

	EC->StopAI();
	EC->StopPerception();


	mQuestTargetComp->UpdateQuestProgress();
}





#pragma region 레거시_전투
//void AEnemy::EventDeath(const FGameplayTag Tag, int32 NewCount)
//{
//	FTimerHandle Handle;
//	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
//		{
//			OnEnemyDead.Broadcast(this);
//		}, 5.f, false);
//}
//void AEnemy::SetCanNextCombo(bool CanCombo)
//{
//	AAIController* AIC = GetAIController();
//	if (IsValid(AIC))
//	{
//		UBlackboardComponent* BB = AIC->GetBlackboardComponent();
//		if (IsValid(BB))
//		{
//			BB->SetValueAsBool(BBKEY_CANNEXTCOMBO, CanCombo);
//		}
//	}
//}
//
//const FAIAttackTableInfo* AEnemy::SelectAttack(EnemyType SelectedAttackType)
//{
//	const UAIAttackData* DataDefault = GetDefault<UAIAttackData>();
//	if (DataDefault)
//	{
//		int32 RowCount = DataDefault->GetDataTableRowCount(SelectedAttackType);
//		int32 AttackIdx = FMath::RandRange(0, RowCount - 1);
//		FString AttackName = TEXT("Attack_") + FString::FromInt(AttackIdx);
//		CurrentDataTableName = *AttackName;
//
//		return DataDefault->FindAIAttackInfo(CurrentDataTableName, SelectedAttackType);
//	}
//
//	return nullptr;
//}
//
//AActor* AEnemy::GetTarget() const
//{
//	AAIController* AIC = GetAIController();
//	if (IsValid(AIC))
//	{
//		UBlackboardComponent* BB = AIC->GetBlackboardComponent();
//		if (IsValid(BB))
//		{
//			UObject* TargetObj = BB->GetValueAsObject(BBKEY_TARGET);
//			if (IsValid(TargetObj))
//			{
//				AActor* TargetActor = Cast<AActor>(TargetObj);
//				return TargetActor;
//			}
//		}
//	}
//
//	return nullptr;
//}
//
//
//
//
//void AEnemy::SetEnemyInfoToHUD(const FGameplayTag Tag, int32 NewCount)
//{
//}
//
//void AEnemy::DeadMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
//{
//	OnEnemyDead.Broadcast(this);
//}
//

//

//
//bool AEnemy::DoAttack(EnemyType SelectedAttackType)
//{
//	FGameplayEventData EventData;
//	EventData.TargetData = UMyBlueprintFunctionLibrary::MakeTargetDataHandleforEnemyAttack(FGameplayAbilityTargetData_EnemyAttackInfo(SelectedAttackType));
//	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_ENEMYATTACK, EventData);
//	return true;
//	//return ActivateAbilityBySlot(EAbilitySlot::LightAttack);
//}
//
//bool AEnemy::DoBlock()
//{
//	// TODO : 
//	// 현재 공격을 당하고 있다면, 블럭을 할 수 없음 
//	// 실제 블록어빌리티를 활성화 
//	return ActivateAbilityBySlot(EAbilitySlot::Block);
//}
//
//bool AEnemy::IsBlocking()
//{
//	FGameplayTagContainer TagContainer(TAG_COMBAT_MELEE_BLOCK);
//	TArray<UGameplayAbility*> ActiveAbilities;
//	GetActiveAbilitiesWitTags(TagContainer, ActiveAbilities, true);
//
//	return !(ActiveAbilities.IsEmpty());
//}
//
//void AEnemy::EndBlock()
//{
//	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_BLOCKDIRECTEND, FGameplayEventData());
//}
//
//void AEnemy::AddDamagedCount()
//{
//	DamagedCount += 1;
//	UE_LOG(LogTemp, Warning, TEXT("Damged Count : %d"), DamagedCount);
//}
//
//int32 AEnemy::GetDamagedCount()
//{
//	return DamagedCount;
//}
//
//void AEnemy::RestDamagedCount()
//{
//	DamagedCount = 0;
//}
//
//bool AEnemy::CheckCanCounter()
//{
//	if (IsValid(AbilitySystemComponent))
//	{
//		int32 TagCount = 0;
//		bool IsTagExist = AbilitySystemComponent->HasMatchingGameplayTag(TAG_ATTACKEDCOUNTER);
//		if (IsTagExist)
//		{
//			TagCount = GetAbilitySystemComponent()->GetTagCount(TAG_ATTACKEDCOUNTER);
//
//			if (TagCount >= 2)
//			{
//				bool CanCounter = AbilitySystemComponent->HasMatchingGameplayTag(TAG_STATE_CANCOUNTER);
//				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Enemy Can Counter"));
//				return CanCounter;
//			}
//		}
//	}
//
//	return false;
//}
//
//bool AEnemy::DoCounter(ECounterType CounterType)
//{
//	// Block or 블럭 
//
//	if (ECounterType::Block == CounterType)
//	{
//		// Block/Roll 
//		SetCounterType(ECounterType::Block);
//		return DoDodge();
//	}
//	else
//	{
//		// Blockㄴ
//		// 아래처럼 직접 호출이 아닌 래핑 함수 생성해야함 
//		SetCounterType(ECounterType::Block);
//		if (true == AbilitySystemComponent->HasMatchingGameplayTag(TAG_STATE_BLOCK))
//		{
//			UE_LOG(LogTemp, Warning, TEXT("ReInputBlock"));
//			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_BLOCKREINPUT, FGameplayEventData());
//			return true;
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("NewBlock"));
//			return ActivateAbilityBySlot(EAbilitySlot::Block);
//		}
//	}
//}
//
//void AEnemy::EndCounter()
//{
//	// DoCounter 에서 블럭이 실행된 경우에는, 
//	// 블럭을 풀어주는 작업이 필요 
//	switch (CurrentCounterType)
//	{
//	case ECounterType::Block:
//		break;
//	case ECounterType::Block:
//		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_BLOCKEND, FGameplayEventData());
//		break;
//	}
//
//	RestDamagedCount();
//
//	AAIController* AIC = Cast<AAIController>(GetController());
//	if (AIC)
//	{
//		UBlackboardComponent* BB = AIC->GetBlackboardComponent();
//		if (BB)
//		{
//			BB->SetValueAsBool(BBKEY_CANCOUNTER, false);
//		}
//	}
//}
//
//bool AEnemy::DoDodge()
//{
//	return ActivateAbilityWithTags(FGameplayTagContainer(TAG_LOCOMOTION_DODGE));
//}
//
//bool AEnemy::CanFinishCounter()
//{
//	switch (CurrentCounterType)
//	{
//	case ECounterType::Block:
//		return (false == HasMatchingGameplayTag(TAG_STATE_DODGINE));
//	case ECounterType::Block:
//		return (false == HasMatchingGameplayTag(TAG_STATE_CANCOUNTER));
//	}
//
//	return false;
//}
//
//void AEnemy::SetFocus(bool NewFocus)
//{
//	AAIController* AIC = GetAIController();
//	AActor* TargetActor = GetTarget();
//	if (IsValid(AIC))
//	{
//		if (NewFocus)
//		{
//			AIC->SetFocus(TargetActor);
//		}
//		else
//		{
//			AIC->ClearFocus(EAIFocusPriority::Gameplay);
//		}
//	}
//}
//
//void AEnemy::SetEnemyGait(EGait NewGait)
//{
//	if (EGait::Run == NewGait)
//	{
//		SetStrafe(false);
//	}
//
//	SetGait(NewGait);
//}
//
//bool AEnemy::DoSpecialAttack()
//{
//	AAIController* AIC = GetAIController();
//	if (IsValid(AIC))
//	{
//		AActor* TargetActor = GetTarget();
//		if (TargetActor)
//		{
//			FGameplayEventData EventData;
//			EventData.Target = TargetActor;
//			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_ENEMYSPECIALATTACK, EventData);
//			return true;
//		}
//	}
//
//	return false;
//}
//
//FVector AEnemy::GetLockPoint()
//{
//	return GetMesh()->GetSocketLocation(FName("neck_01"));
//
//}
//
//void AEnemy::SetAttackName(FName NewAttackName)
//{
//	CurrentAttackName = NewAttackName;
//}
//
//bool AEnemy::IsAttacking()
//{
//	FGameplayTagContainer TagContainer(TAG_COMBAT_MELEE_ATTACK_LIGHT);
//	TArray<UGameplayAbility*> ActiveAbilities;
//	GetActiveAbilitiesWitTags(TagContainer, ActiveAbilities, false);
//
//	return !(ActiveAbilities.IsEmpty());
//}
//
//void AEnemy::DoNextAttack()
//{
//	SetCanNextCombo(false);
//
//	//mCurrentPlayMontageIdx += 1;
//
//	//if (CurrentAttackData->AttackMontages.IsValidIndex(mCurrentPlayMontageIdx))
//	//{
//	//	FGameplayEventData EventData;
//	//	EventData.OptionalObject = CurrentAttackData->AttackMontages[mCurrentPlayMontageIdx];
//	//	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_EVENT_ATTACKREINPUT, EventData);
//	//}
//}
//
//void AEnemy::ComboInputSwitch(UANS_ComboAttack* ANS, bool Value)
//{
//}
//
//void AEnemy::StartHitStop(float Duration)
//{
//	CustomTimeDilation = 0.0f;
//
//	// 계속해서 반복시키는게 아니기 때문에 이 값을 플레이어가 따로 저장해놀 필요는 없음
//	FTimerHandle TimerHandle;
//
//	// Create 시리즈와 Bind 시리즈의 차이점은 / FTimerDelegate 인스턴스를 생성하고 함수를 바인딩한땔드 -> Bind, 생성하지 않고 함수 바인딩과 동시에 FTimerDelegate  넘겨줄때는 Create
//
//	// C++ 함수를 바인딩한다면 -> UObject 
//	// UFunction 으로 구현된 함수 및 블루프린트 구현함수의 경우는 UFunction
//
//	FTimerDelegate TimerDelegate;
//	TimerDelegate.BindUObject(this, &AEnemy::EndHitStop);
//
//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Duration, false);
//}
//
//void AEnemy::EndHitStop()
//{
//	CustomTimeDilation = 1.0f;
//}
//
#pragma endregion 레거시_전투
