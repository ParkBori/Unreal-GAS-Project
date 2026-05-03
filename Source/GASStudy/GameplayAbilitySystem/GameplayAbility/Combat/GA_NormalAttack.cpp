// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_NormalAttack.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "Component/EquipComponent.h"

#include "Shared/Macro/Log.h"

#include "Shared/Commbat/ComboAttackData.h"

#include "GameplayAbilitySystem/GameplayEffect/GE_Combat.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "GameFramework/Character.h"



UGA_NormalAttack::UGA_NormalAttack()
{
	// 기본태그
	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(Ability::Ability_Combat_NormalAttack);
	AbilityTag.AddTag(Ability::Ability_Combat);
	SetAssetTags(AbilityTag);
	
	// 활성화 소유 태그 
	ActivationOwnedTags.AddTag(State::State_Attacking);

	// 이벤트 트리거 태그
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = Ability::Ability_Combat_NormalAttack;
	AbilityTriggers.Add(TriggerData);

	// 무기 활성화 상태에서만 작동
	ActivationRequiredTags.AddTag(State::State_WeaponActivated);

	// 발동시 취소시킬 어빌리티
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_ParriedReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_BlockReaction);
	CancelAbilitiesWithTag.AddTag(Ability::Ability_Combat_Hit);


	static ConstructorHelpers::FObjectFinder<UDataTable> AnimDTAsset(TEXT("/Script/Engine.DataTable'/Game/GASStudy/Data/Combat/Player/Normal/DT_NoramCombat.DT_NoramCombat'"));
	if (AnimDTAsset.Succeeded())
	{
		mAnimDT = AnimDTAsset.Object;
	}

	mSectionName = TEXT("Combo.State");

}


void UGA_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 재생 목록 초기화
	if (!mEquipComponent.IsValid())
	{
		mEquipComponent = ActorInfo->AvatarActor->GetComponentByClass<UEquipComponent>();
	}

	if (!mEquipComponent.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!SetComboData(GetLastTagName(mEquipComponent->GetEquipWeaponTag())))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	SetPlayMontage();

	FGameplayTagContainer Tags(Ability::Ability_Combat_ParryReaction);
	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&Tags);

	if (CanExecuteFinisher())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_NormalAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	mCurrentCombo = (mCurrentCombo + 1) % mMaxCombo;
	PlayMontage(SetPlayMontage());
}

bool UGA_NormalAttack::CanExecuteFinisher()
{
	APlayerController* PC = CurrentActorInfo->PlayerController.Get();
	if (!PC) return false;

	FVector CameraLocation;
	FRotator CameraRotation;

	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Start = CameraLocation;
	FVector End = Start + (CameraRotation.Vector() * 5000.f);

	// 방향만 뽑아서
	FVector Dir = (End - Start).GetSafeNormal();

	// Z 제거
	Dir.Z = 0.f;
	Dir.Normalize();

	// 캐릭터 기준으로 다시
	FVector NewStart = GetAvatarActorFromActorInfo()->GetActorLocation() + FVector(0, 0, 50);
	FVector NewEnd = NewStart + Dir * 5000.f;

	FHitResult Hit;

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT_NAME_ONLY("Weapon Trace"), false, GetAvatarActorFromActorInfo());
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, NewStart, NewEnd, ECollisionChannel::ECC_EngineTraceChannel3, CollisionQueryParams);
	
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), NewStart, NewEnd, FColor::Red, false, 5.f, 2.f);
#endif

	if (bSuccess)
	{
		AActor* HitActor = HitResult.GetActor();
	
		bool bIsBehind = IsBehindTarget(HitActor);
		bool bIsGroggy = false;

		auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!ASC)
		{
			return false;
		}

		auto EnemyTags = ASC->GetOwnedGameplayTags();
		bIsGroggy = EnemyTags.HasTagExact(State::State_Groggy);

		if (bIsBehind || bIsGroggy)
		{
			FGameplayEventData EventData;
			EventData.Target = HitActor;
			EventData.EventMagnitude = bIsBehind ? 1.f : 0.f;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), Event::Event_Ability_Finisher, EventData);
			return true;
		}
	}



	return false;
}

bool UGA_NormalAttack::SetComboData(const FString& WeaponTag)
{
	if (WeaponTag.IsEmpty())
	{
		return false;
	}

	FString RowName = WeaponTag + TEXT(".");

	if (CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayTagContainer& Tags = CurrentActorInfo->AbilitySystemComponent->GetOwnedGameplayTags();
		if (Tags.HasTagExact(State::State_Dodging))
		{
			RowName += TEXT("Dodge");
		}

		else if (Tags.HasTagExact(State::State_Dodging))
		{
			RowName += TEXT("Sprint");
		}

		else if (Tags.HasTag(State::State_ParryReactiing))
		{
			RowName += TEXT("SuccessParry");

		}

		else
		{
			RowName += TEXT("Normal");
		}
	}

	if (!mAnimDT)
	{
		return false;
	}

	const FNoramlCombatDataByWeapon* Data = mAnimDT->FindRow<FNoramlCombatDataByWeapon>(*RowName, TEXT(""));
	if (!Data || !Data->NormalAttackMontageList.Num())
	{
		return false;
	}
	
	mPlayList = Data->NormalAttackMontageList;
	mCurrentCombo = 0;
	mMaxCombo = mPlayList.Num();
	
	return true;
}

UAnimMontage* UGA_NormalAttack::SetPlayMontage()
{
	if (!mPlayList.IsValidIndex(mCurrentCombo))
	{
		return nullptr;
	}

	return mPlayMontage = mPlayList[mCurrentCombo];
}

FString UGA_NormalAttack::GetLastTagName(const FGameplayTag& Tag)
{
	FString TagStr = Tag.ToString();

	int32 Index;
	if (TagStr.FindLastChar(TEXT('.'), Index))
	{
		return TagStr.Mid(Index + 1);
	}

	return TagStr;
}

bool UGA_NormalAttack::IsBehindTarget(AActor* Target)
{
	if (!Target)
	{
		return false;
	}


	auto TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!IsValid(TargetAsc))
	{
		return false;
	}

	// 적이 나를 인지하는 경우
	if (TargetAsc->GetOwnedGameplayTags().HasTagExact(State::State_HasTarget))
	{
		return false;
	}

	// 각도 체크 
	FVector PlayerForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector TargetForward = Target->GetActorForwardVector();

	// 두 전방 벡터의 내적 (둘 다 같은 방향을 보고 있으면 1에 가까움)
	float ForwardDot = FVector::DotProduct(PlayerForward, TargetForward);

	// 약 45도 범위 내에서 뒤를 잡았는지 확인 (0.7 이상이면 대략 뒤쪽)
	bool bIsBehind = ForwardDot > 0.7f;

	return bIsBehind;
}
