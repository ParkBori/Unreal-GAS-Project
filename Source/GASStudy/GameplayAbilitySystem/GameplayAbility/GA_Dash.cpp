// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GA_Dash.h"
#include "Tag/GameplayTag.h"

UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 이렇게 접근하는게 안된다는거임 	
	/*
	게임어빌리티시스템을 사용하는 이상 
	어떠한 수치적 변경은 어빌리티시스템과 어트리뷰트를 활용하는게 맞는데
	이걸 캐릭터에 직접적으로 적용시키는것은 어빌리티 시스템을 사용하는 의미가 없을뿐더러 
	나중에 추적하려면 어빌리티 + 외부코드까지 봐야함 
	*/

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	//ASC->ApplyGameplayEffectSpecToSelf();
	//ASC->ApplyGameplayEffectToSelf();


}

bool UGA_Dash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// 실행되어서는 안되는 상황을 여기서 체크 
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	//UE_LOG(LogTemp, Display, TEXT("GA_Dash CanAtivate? %s"), (Result ? TEXT("True") : TEXT("False")));
	return Result;
}

void UGA_Dash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	FGameplayTagContainer RemoveTagContainer;
	//RemoveTagContainer.AddTag()
	//ASC->RemoveActiveEffectsWithTags();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
