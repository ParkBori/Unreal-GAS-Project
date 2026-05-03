// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_AttackSelecter.h"
#include "AbilitySystemComponent.h"
#include "Tag/GameplayTag.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UGA_AttackSelecter::UGA_AttackSelecter()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_AttackSelecter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Display, TEXT("UGA_AttackSelecter"));

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);
	// 몽타주 선택 로직 

	// 익스큐션 
	FGameplayTagContainer TagContainer;
	ASC->GetOwnedGameplayTags(TagContainer);

	// 실행시키 태그 설정 
	FGameplayTagContainer ActivateAbilityTag;
	
	ACharacter* Owner = Cast<ACharacter>(ASC->GetAvatarActor());
	bool IsFalling = Owner->GetCharacterMovement()->IsFalling();

	if (TagContainer.HasTagExact(TAG_CHARACTER_STATE_IsBlocking))
	{
		UE_LOG(LogTemp, Display, TEXT("Command Attack"));

		// 커맨드공격 (우클릭 + 좌클릭) 
		// 이 이벤트에 들어온게 좌클리이고 
		// 우클릭이 눌린상태에서 좌클릭을 실행하면 
		ActivateAbilityTag.AddTag(TAG_CHARACTER_ATTACK_COMMANDATTACK);
		ASC->TryActivateAbilitiesByTag(ActivateAbilityTag);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	else if (TagContainer.HasTagExact(TAG_CHARACTER_STATE_ISDASHING) || TagContainer.HasTagExact(TAG_CHARACTER_STATE_ISDODGING))
	{
		UE_LOG(LogTemp, Display, TEXT("DashDodge Attack"));
		// 대쉬닷지
		ActivateAbilityTag.AddTag(TAG_CHARACTER_ATTACK_DASHDODGEATTACK);
		ASC->TryActivateAbilitiesByTag(ActivateAbilityTag);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	else if (TagContainer.HasTagExact(TAG_CHARACTER_STATE_ISJUMPING) || IsFalling)
	{
		// 점프
		ActivateAbilityTag.AddTag(TAG_CHARACTER_ATTACK_JUMPATTACK);
		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
		ASC->FindAllAbilitiesWithTags(AbilitySpecHandles, ActivateAbilityTag);


		//FGameplayAbilitySpec* AbilitySpec_GAComboAttack = ASC->FindAbilitySpecFromClass(UGA_ComboAttack::StaticClass());
		FGameplayAbilitySpec* AbilitySpec_JumpAttack = ASC->FindAbilitySpecFromHandle(AbilitySpecHandles[0]);
		if (AbilitySpec_JumpAttack->IsActive())
		{
			ASC->AbilitySpecInputPressed(*AbilitySpec_JumpAttack);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		else
		{
			ActivateAbilityTag.AddTag(TAG_CHARACTER_ATTACK_JUMPATTACK);
			ASC->TryActivateAbilitiesByTag(ActivateAbilityTag);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
	else
	{
		// 콤보 
		UE_LOG(LogTemp, Display, TEXT("Combo Attack"));

		ActivateAbilityTag.AddTag(TAG_CHARACTER_ATTACK_COMBOATTACK);
		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
		ASC->FindAllAbilitiesWithTags(AbilitySpecHandles, ActivateAbilityTag);


		//FGameplayAbilitySpec* AbilitySpec_GAComboAttack = ASC->FindAbilitySpecFromClass(UGA_ComboAttack::StaticClass());
		FGameplayAbilitySpec* AbilitySpec_GAComboAttack = ASC->FindAbilitySpecFromHandle(AbilitySpecHandles[0]);
		if (AbilitySpec_GAComboAttack->IsActive())
		{
			ASC->AbilitySpecInputPressed(*AbilitySpec_GAComboAttack);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		else
		{
			ActivateAbilityTag.AddTag(TAG_CHARACTER_ATTACK_COMBOATTACK);
			ASC->TryActivateAbilitiesByTag(ActivateAbilityTag);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	
	}
}
