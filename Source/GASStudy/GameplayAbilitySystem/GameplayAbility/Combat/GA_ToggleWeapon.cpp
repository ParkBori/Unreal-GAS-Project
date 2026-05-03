// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/Combat/GA_ToggleWeapon.h"
#include "Component/EquipComponent.h"

#include "AbilitySystemComponent.h"

#include "Shared/Commbat/CombatData.h"

#include "Shared/Macro/Log.h"




UGA_ToggleWeapon::UGA_ToggleWeapon()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DTAsset(TEXT("/Script/Engine.DataTable'/Game/GASStudy/Data/Equip/DT_EquipAnim.DT_EquipAnim'"));
	if (DTAsset.Succeeded())
	{
		mEquipAnimDT = DTAsset.Object;
	}
}

void UGA_ToggleWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UEquipComponent* EC = ActorInfo->AvatarActor->GetComponentByClass<UEquipComponent>();
	if (!EC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FGameplayTag& EquipWeaponTag = EC->GetEquipWeaponTag();
	if (!EquipWeaponTag.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		MY_LOG(Error, "장착 무기가 없음");
		return;
	}

	if (!ActorInfo->AbilitySystemComponent.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayTagContainer OwnedTags;
	GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags(OwnedTags);
	bool IsWeaponActiveed = OwnedTags.HasTagExact(State::State_WeaponActivated);

	MY_LOG(Error, "%s", *EquipWeaponTag.ToString());

	mPlayMontage = SetPlayMontage(IsWeaponActiveed, EquipWeaponTag);
	if (!mPlayMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UAnimMontage* UGA_ToggleWeapon::SetPlayMontage(bool IsWeaponActiveed, const FGameplayTag& EquipWeaponTag)
{
	if (!mEquipAnimDT)
	{
		return nullptr;
	}

	FString Key = FString::Printf(TEXT("%s_%s"), IsWeaponActiveed ? TEXT("UnActivate") : TEXT("Activate"), *EquipWeaponTag.ToString());
	const FEquipAnim* EquipAnim = mEquipAnimDT->FindRow<FEquipAnim>(*Key, TEXT(""));
	if (!EquipAnim || !EquipAnim->Montage)
	{
		return nullptr;
	}

	return EquipAnim->Montage;
}

