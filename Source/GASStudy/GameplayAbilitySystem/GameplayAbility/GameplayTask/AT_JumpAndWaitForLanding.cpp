// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayAbility/GameplayTask/AT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UAT_JumpAndWaitForLanding::UAT_JumpAndWaitForLanding()
{
}

UAT_JumpAndWaitForLanding* UAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UAT_JumpAndWaitForLanding* NewAT = NewAbilityTask<UAT_JumpAndWaitForLanding>(OwningAbility);
	return NewAT;
}

void UAT_JumpAndWaitForLanding::Activate()
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->Jump();
	Character->LandedDelegate.AddDynamic(this, &UAT_JumpAndWaitForLanding::OnLandedCallback);

	SetWaitingOnAvatar();
}

void UAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UAT_JumpAndWaitForLanding::OnLandedCallback);
}

void UAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
