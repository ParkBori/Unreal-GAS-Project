// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyController.h"
#include "AI/AIBlackBoardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Component/EnemyCombatComponent.h"
#include "Character/NonPlayable/Enemy/Enemy.h"

AEnemyController::AEnemyController()
{
	mAIPerceptionComponent = CreateDefaultSubobject< UAIPerceptionComponent>(TEXT("AIPercption"));
	
	SetPerceptionComponent(*mAIPerceptionComponent);


	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	
	// 시야 반경 
	mSightConfig->SightRadius = 2000.f;
	
	// 시야해제 반경 
	mSightConfig->LoseSightRadius= 2000.f;
	
	// 주변인식 각도 
	mSightConfig->PeripheralVisionAngleDegrees = 180.f;

	// 제네릭 팀아이디로 설정된 얘들을 인식여부

	// 친구 탐지 
	mSightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// 적 탐지
	mSightConfig->DetectionByAffiliation.bDetectEnemies = true;

	// 중립 탐지
	mSightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	mAIPerceptionComponent->ConfigureSense(*mSightConfig);

	mAIPerceptionComponent->SetDominantSense(mSightConfig->GetSenseImplementation());
}

void AEnemyController::StopPerception()
{
	mAIPerceptionComponent->OnTargetPerceptionUpdated.RemoveAll(this);
	mAIPerceptionComponent->Deactivate();
}

void AEnemyController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);


	auto* ECC = aPawn->GetComponentByClass<UEnemyCombatComponent>();
	if (ECC)
	{
		mOnTargetActorChanged.AddUObject(ECC, &UEnemyCombatComponent::SetTarget);
	}

	mAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::Callback_OnTarget);
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyController::Callback_OnTarget(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && IsValid(Actor))
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET, Actor);
		mOnTargetActorChanged.Broadcast(Actor);
	}
	else
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET, nullptr);
		mOnTargetActorChanged.Broadcast(nullptr);
	}
}