// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/QuestTargetComponent.h"
#include "Subsystem/QuestTargetRegistrySubsystem.h"
#include "Shared/Macro/Log.h"

#include "Subsystem/QuestSubsystem.h"
#include "Subsystem/DialogueSubsystem.h"

#include "DeveloperSettings/GameDataSettings.h"


// Sets default values for this component's properties
UQuestTargetComponent::UQuestTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UQuestTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	auto* QRS = GetWorld()->GetSubsystem<UQuestTargetRegistrySubsystem>();
	if (!QRS)
	{
		MY_LOG(Error, "UQuestTargetRegistrySubsystem is nullptr");
		return;
	}

	if (mTargetInfo.TargetName.IsValid() && GetOwner())
	{
		QRS->RegisterTarget(mTargetInfo.TargetName, GetOwner());
	}
}

void UQuestTargetComponent::UpdateQuestProgress()
{
	if (!bUseQuestProgressUpdate)
	{
		return;
	}

	auto* QS = GetOwner()->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (IsValid(QS))
	{
		QS->UpdateQuestProgress(mTargetInfo);
	}
}

void UQuestTargetComponent::GiveQuest()
{
	// 소유 퀘스트 가져오기 

	if (mTargetInfo.TargetName.IsNone())
	{
		return;
	}

	const auto* GDS = UGameDataSettings::Get();
	if (!GDS)
	{
		return;
	}

	// 현재 NPC가 소유한 퀘스트 데이터 
	const FQuestTriggerData* Data = GDS->GetDataTableRow<FQuestTriggerData>(EGlobalGameDataTableType::QuestTriggerData, mTargetInfo.TargetName);
	if (!Data)
	{
		return;
	}

	auto* QS = GetOwner()->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!QS)
	{
		return;
	}

	// 퀘스트 소유 목록 
	for (auto QuestName : Data->mQuestList)
	{
		// 이름에 해당 퀘스트 DA 
		if (QS->CanAddQuest(QuestName) ||
			QS->IsActivatedQuest(QuestName))
		{
			mCurrentActivateQuest = QuestName;
			ActivateDialogue(QuestName);
			break;
		}
	}
}

void UQuestTargetComponent::ActiveQuest()
{
	auto* QS = GetOwner()->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!QS)
	{
		return;
	}

	// 현재 활성화 된 퀘스트가 존재 하고 활성화 상태인지 확인 
	if (!mCurrentActivateQuest.IsNone() && QS->IsActivatedQuest(mCurrentActivateQuest))
	{
		ActivateDialogue(mCurrentActivateQuest);
	}

	// mCurrentActivateQuest가 완료 됐거나, None 인경우에는 퀘스트 추가 
	else if (mCurrentActivateQuest.IsNone() || !QS->IsActivatedQuest(mCurrentActivateQuest) || !QS->IsCompletedQuest(mCurrentActivateQuest))
	{
		GiveQuest();
	}
}

void UQuestTargetComponent::ActivateDialogue(FName DialogueName)
{
	auto* DS = GetOwner()->GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
	if (DS)
	{
		DS->Activate_V2(GetOwner(), DialogueName);
	}
}


