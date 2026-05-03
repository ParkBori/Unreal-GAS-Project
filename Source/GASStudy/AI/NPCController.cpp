// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCController.h"
#include "Perception/AIPerceptionSystem.h"
#include "Character/RPGCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

ANPCController::ANPCController()
{
	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT(""));
	//if (BBAssetRef.Succeeded())
	//{
	//	BBAsset = BBAssetRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT(""));
	//if (BTAssetRef.Succeeded())
	//{
	//	BTAsset = BTAssetRef.Object;
	//}

}

void ANPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void ANPCController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());
}

void ANPCController::RunAI()
{
	UBlackboardComponent* BlackBoardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackBoardPtr))
	{
		bool Result = RunBehaviorTree(BTAsset);
		ensure(Result);
	}
}

void ANPCController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}

	ClearFocus(EAIFocusPriority::Gameplay);
}

ETeamAttitude::Type ANPCController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 폰이 아니면 중립 => 무시한다는 것 
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (nullptr == OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}

	// 여기까지 통과하면 폰인건 확정임

	// 폰인건 당연하고 AI 인식인터페이스가 있어야 인식가능하므로 체크
	auto CharacterTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	// 만약에 AI 인식인터페이스가 컨트롤러에서 구현됐을 가능성 체크 
	class IGenericTeamAgentInterface* ControllerTeamAgent = Cast< IGenericTeamAgentInterface>(OtherPawn->GetController());

	// 즉, 인식가능하게 인터페이스를 폰 혹은 컨트롤러에서 구현한다는 말임
	// 두개 중에 한가지를 골라서 사용함
	// 그래서 아래와 같은 조건식으로 필터링해줌
	if (nullptr == CharacterTeamAgent && nullptr == ControllerTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	// 폰 or 컨트롤러 어느쪽에 구현했는지에 따라서 아래처럼 분기가 나뉨
	FGenericTeamId OtherTeamId = FGenericTeamId();
	if (nullptr != ControllerTeamAgent)
	{
		OtherTeamId = ControllerTeamAgent->GetGenericTeamId();
	}
	else if (nullptr != CharacterTeamAgent)
	{
		OtherTeamId = CharacterTeamAgent->GetGenericTeamId();
	}

	// 시민일 경우 중립
	FGenericTeamId MyTeamId = GetGenericTeamId();
	if (OtherTeamId.GetId() == (uint8)(EFraction::Civilians))
	{
		return ETeamAttitude::Neutral;
	}

	// 나랑 같다면 아군
	if (OtherTeamId == MyTeamId)
	{
		return ETeamAttitude::Friendly;
	}

	// Neutral : 중립 
	// Friendly : 아군
	// Hostile : 적군

	return ETeamAttitude::Hostile;
}
