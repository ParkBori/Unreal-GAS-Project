// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NonPlayable/Enemy/Boss.h"
#include "GameMode/RPGGameMode.h"

#include "AbilitySystemComponent.h"
#include "Shared/Tag/SharedTag.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIBlackBoardData.h"

void ABoss::EventStartBossFight_Implementation()
{
	// 보스의 경우 특정 시점에 트리거를 발동시킨다

	auto* AIC = GetController<AAIController>();
	if (IsValid(AIC))
	{
		auto* BB = AIC->GetBlackboardComponent();
		if (IsValid(BB))
		{
			BB->SetValueAsBool(BBKEY_BossActive, true);
		}
	}
}


void ABoss::EventBossFightEnd_Implementation()
{
}

void ABoss::EventDeath(const FGameplayTag Tag, int32 NewCount)
{
	Super::EventDeath(Tag, NewCount);
	EventBossFightEnd();
}


