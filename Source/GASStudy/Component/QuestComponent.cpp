// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/QuestComponent.h"
#include "Shared/Macro/Log.h"

// ==== 공용 데이터 ====
#include "DeveloperSettings/GameDataSettings.h"

// ==== 유틸 ====
#include "Builder/QuestProgressBuilder.h"

// ==== 아이템 ===== 
#include "Shared/Item/ItemInfo.h"
#include "Subsystem/ItemSubsystem.h"

// ==== 플레이어 세이브 ====
#include "Save/Player/PlayerSaveGame.h"
#include "Subsystem/SaveSubsystem.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UQuestComponent::CanAddNewQuest(const FName& QuestID)
{
	// 퀘스트 상태 확인 
	if (IsActiveQuest(QuestID) || IsCompleteQuest(QuestID))
	{
		return false;
	}

	// 퀘스트 수락 태그 소유확인 
	const auto* Quest = GetQuest(QuestID);
	if (!Quest)
	{
		//MY_LOG(Error, "Can't find quest from DT");
		return false;
	}

	UQuestDataAsset* DA = Quest->QuestDataAsset;
	if (!IsValid(DA))
	{
		return false;
	}

	// 퀘스트 받기 위한 필요 태그들 (주로 선행퀘 클리어로 부여받은 태그)
	if (!mGrantedTagsByQuestComplete.HasAllExact(DA->QuestAcceptanceConditions))
	{
		return false;
	}

	return true;
}

bool UQuestComponent::AddNewQuest(const FName& QuestID)
{
	if (!CanAddNewQuest(QuestID))
	{
		return false;
	}

	// FQuestProgress 구조체를 정리해줘야 함
	const auto* Quest = GetQuest(QuestID);
	if (!Quest)
	{
		//MY_LOG(Error, "Can't find quest from DT");
		return false;
	}

	UQuestDataAsset* DA = Quest->QuestDataAsset;
	if (!IsValid(DA))
	{
		return false;
	}

	FQuestProgress QP = QuestProgressBuilder(DA).Build();
	mCurrentProgressQuests.Add(QuestID, QP);
	mCurrentTrackingQuest = QuestID;

	//MY_LOG(Warning, "AddedQuest : %s", *mCurrentTrackingQuest.ToString());


	// 퀘스트 정보 업데이트
	mOnUpdatedTrackingQuest.Broadcast(MakeQuestSnapshot(mCurrentTrackingQuest));

	// 현재 퀘스트 목록 업데이트
	mOnUpdatedQuestList.Broadcast(MakeQuestListSnapshot());

	USaveSubsystem* SS = GetOwner()->GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (IsValid(SS))
	{
		SS->SaveGameSync();
	}


	return true;
}

void UQuestComponent::UpdateQuestProgress(const FQuestTargetInfo& QuestTargetInfo)
{
	bool bUpdateCompleted = false;
	for (auto& PQ : mCurrentProgressQuests)
	{
		bUpdateCompleted = PQ.Value.Update(QuestTargetInfo.TargetName, QuestTargetInfo.Amount);
		if (PQ.Value.IsComplete())
		{
			// -> 더이상 스테이지가 없다면, 컴플리트로 변경
			// CompleteQuest(PQ.Key);
		}
	}

	// UI 업데이트
	mOnUpdatedTrackingQuest.Broadcast(MakeQuestSnapshot(mCurrentTrackingQuest));
}

void UQuestComponent::UpdateTrackingQuest(const FName& QuestID)
{
	if (mCurrentTrackingQuest == QuestID)
	{
		return;
	}

	mCurrentTrackingQuest = QuestID;
	mOnUpdatedTrackingQuest.Broadcast(MakeQuestSnapshot(mCurrentTrackingQuest));
}

bool UQuestComponent::CompleteQuest(const FName& QuestID, bool& bLastQuest)
{
	if (!mCurrentProgressQuests.Contains(QuestID))
	{
		return false;
	}
	
	FQuestProgress& Progress = mCurrentProgressQuests[QuestID];

	if (Progress.State != EQuestState::Completed)
	{
		return false;
	}
	
	const auto* Quest = GetQuest(QuestID);
	if (!Quest)
	{
		//MY_LOG(Error, "Can't find quest from DT");
		return false;
	}

	UQuestDataAsset* QuestDA = Quest->QuestDataAsset;
	if (!IsValid(QuestDA))
	{
		return false;
	}

	// 1. 보상 지급
	if (!GiveQuestReward(QuestDA->Reward))
	{
		// TODO : 팝업 생성 + 다이얼로그 강제 추가 or 분기 나누기가 필요함
		MY_LOG(Warning, "인벤토리 꽉 차서 아이템 받기 추가, ")
		return false;
	}

	GrantQuestFlags(QuestDA->GrantedFlags);
	// 2. 플래그 지급

	// 4. 완료 목록 이동

	mCompletedQuests.Add(QuestID);
	mCurrentProgressQuests.Remove(QuestID);

	if (!mCurrentProgressQuests.IsEmpty())
	{
		mCurrentTrackingQuest = mCurrentProgressQuests.begin()->Key;
	}
	else
	{
		mCurrentTrackingQuest = "";
	}

	// 퀘스트 정보 업데이트 
	mOnUpdatedTrackingQuest.Broadcast(MakeQuestSnapshot(mCurrentTrackingQuest));
	// 퀘스트 목록 업데이트
	mOnUpdatedQuestList.Broadcast(MakeQuestListSnapshot());

	bLastQuest = Quest->bIsLastQuest;

	USaveSubsystem* SS = GetOwner()->GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (IsValid(SS))
	{
		SS->SaveGameSync();
	}

	return true;
}

void UQuestComponent::RequestQuestChangeTrackingQuest(const FName& QuestID)
{
	// 완료된 퀘스트라면 업데이트하지 않는다.
	if (mCompletedQuests.Contains(QuestID))
	{
		return;
	}

	FName RequestedID = (!QuestID.IsNone()) ? QuestID : mCurrentTrackingQuest;
	UpdateTrackingQuest(RequestedID);
}

void UQuestComponent::RequestQuestData(const FName& QuestID, bool bChangeTrackingQuest)
{
	// 완료된 퀘스트라면 업데이트하지 않는다.
	// 완료된 퀘스트 체크 
	bool bActiveQuest = false ;

	if (mCompletedQuests.Contains(QuestID))
	{
		bActiveQuest = false;
	}
	else if (mCurrentProgressQuests.Contains(QuestID))
	{
		bActiveQuest = true;
	}
	else
	{
		return;
	}
	
	//MY_LOG(Warning, "");

	UpdateQuest(QuestID, (bChangeTrackingQuest && bActiveQuest));
}


EQuestState UQuestComponent::GetQuestState(const FName& QuestID) const
{
	return mCurrentProgressQuests.Contains(QuestID) ? mCurrentProgressQuests[QuestID].State : EQuestState::None;
}

void UQuestComponent::UpdateQuest(const FName& QuestID, bool bChangeTrackingQuest)
{
	if (bChangeTrackingQuest)
	{
		mCurrentTrackingQuest = QuestID;
	}

	mOnUpdatedTrackingQuest.Broadcast(MakeQuestSnapshot(QuestID));
	
	mOnUpdatedShowingQuest.Broadcast(QuestID, bChangeTrackingQuest);
}

bool UQuestComponent::GiveQuestReward(const FQuestReward& Reward)
{
	// 아이템 목록으로 체크하는 기능이 필요하네

	// Reward.Exp;
	if (Reward.Items.IsEmpty())
	{
		return true;
	}

	auto* SS = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (SS)
	{
		return SS->RequestAddItems(GetOwner(), EAddPath::QuestCompltete, Reward.Items);
	}

	return false;
}

void UQuestComponent::GrantQuestFlags(const FGameplayTagContainer& TagContainer)
{
	mGrantedTagsByQuestComplete.AppendTags(TagContainer);
}

bool UQuestComponent::IsCompleteQuest(const FName& QuestID) const
{
	return mCompletedQuests.Contains(QuestID);
}

bool UQuestComponent::IsActiveQuest(const FName& QuestID) const
{
	return mCurrentProgressQuests.Contains(QuestID);
}

const FQuest* UQuestComponent::GetQuest(const FName& QuestID) const
{
	UWorld* World = GetWorld();
	if (World)
	{
		const FQuest* Quest = 
			UGameDataSettings::Get()->GetDataTableRow<FQuest>(EGlobalGameDataTableType::QuestData, QuestID);
		return Quest;
	}

	return nullptr;
}

FQuestSnapshot UQuestComponent::MakeQuestSnapshot(const FName& QuestID) const
{
	FQuestSnapshot QS;

	// 퀘스트 정보 얻기
	const auto* Quest = GetQuest(QuestID);
	//MY_LOG(Error, "Can't find quest from DT");
	if (!Quest)
	{
		return FQuestSnapshot();
	}

	UQuestDataAsset* DA = Quest->QuestDataAsset;
	if (!IsValid(DA))
	{
		return FQuestSnapshot();
	}

	// 퀘스트 이름
	QS.QuestName = DA->QuestName;
	
	// 퀘스트 설명
	QS.QuestDescript = DA->QuestDescript;

	// 퀘스트 타입 설정MakeQuestSnapshot
	// QS.QuestType = Quest->QuestType;


	// 퀘스트 상세 정보
	/*
	퀘스트 상세 정보를 표시할거면, 굳이 DA 쪽을 참조할 필요가 없네
	왜냐면, 디테일한 내용은 현재 진행정보 쪽에 있으니까

	mCurrentProgressQuests 가 소유하는 FQuestProgress 는
	모든 스테이지를 소유하고 

	내부에 모든 스테이지의 이름 타겟 정보를 소유중임 
	*/

	if (mCurrentProgressQuests.Contains(QuestID))
	{
		const FQuestProgress& CurrnetQuestProgress = mCurrentProgressQuests[QuestID];

		// 퀘스트 진행정보
		const TArray<FStageProgress>& StageProgressList = CurrnetQuestProgress.StageProgressList;

		int32 MaxIndex = FMath::Min(
			CurrnetQuestProgress.CurrentStageIndex,
			StageProgressList.Num() - 1);

		for (int32 i = 0; i <= MaxIndex; ++i)
		{
			const FStageProgress& Stage = StageProgressList[i];

			FQuestUIProgressData Data;

			if (i == CurrnetQuestProgress.CurrentStageIndex)
			{
				Data.bCurrent = true;
			}

			Data.StageName = FText::FromName(Stage.StageName);

			for (const auto& StageProgress : Stage.ObjectiveProgress)
			{
				FQuestUIDetailData DetailData;

				DetailData.Description = StageProgress.Value.Description;
				DetailData.DetailProgress = FText::FromString(StageProgress.Key.ToString() + TEXT(" ") + StageProgress.Value.ToString());
				DetailData.bCompleted = StageProgress.Value.IsCompleted();
				
				Data.QuestUIDetailDataList.Add(DetailData);
			}

			QS.QuestProgress.Add(Data);

			//MY_LOG(Warning, "%s", *Stage.StageName.ToString());
		}


		if (QuestID == mCurrentTrackingQuest)
		{
			QS.QuestState = FText::FromString(TEXT("추적중"));
		}
		else
		{
			QS.QuestState = FText::FromString(TEXT("진행중"));
		}
	}

	if (mCompletedQuests.Contains(QuestID))
	{
		for (const auto& Stage : DA->Stages)
		{
			FQuestUIProgressData Data;
			Data.StageName = FText::FromName(Stage.StageName);
			QS.QuestProgress.Add(Data);
		}

		QS.bIsCompletedQuest = true;
		QS.QuestState = FText::FromString(TEXT("완료"));
	}

	// 퀘스트 보상
	const FQuestReward& QR = DA->Reward;
	QS.Reward.Exp = QR.Exp;

	for (const auto& Item : QR.Items)
	{
		const FName& ItemName = Item.Key;
		const FItemInfoBase* ItemInfo = UGameDataSettings::Get()->GetDataTableRow<FItemInfoBase>(EGlobalGameDataTableType::ItemData, ItemName);
		if (ItemInfo)
		{
			const UBaseItemDataAsset* ItemDA = ItemInfo->ItemData;
			ItemDA->DisplayName;
			ItemDA->Icon;
			Item.Value;

			QS.Reward.Items.Add(ItemDA->DisplayName, FQuestRewardItemInfo(ItemDA->Icon, Item.Value));
		}
	}

	QS.bIsTrackingQuest = (QuestID == mCurrentTrackingQuest);

	return QS;
}

FQuestListSnapshot UQuestComponent::MakeQuestListSnapshot() const
{
	FQuestListSnapshot QS;

	QS.CurrentTrackingQuest = mCurrentTrackingQuest;
	mCurrentProgressQuests.GetKeys(QS.ActiveQuestList);
	QS.CompleteQuestList = mCompletedQuests.Array();

	return QS;
}



void UQuestComponent::MakeSaveData(UPlayerSaveGame* SaveGame) const
{
	if (!SaveGame)
	{
		return;
	}
	 
	SaveGame->QuestSaveData.CurrentProgressQuests.Empty();
	SaveGame->QuestSaveData.CurrentProgressQuests = mCurrentProgressQuests;

	SaveGame->QuestSaveData.CompletedQuests.Empty();
	SaveGame->QuestSaveData.CompletedQuests = mCompletedQuests;

	SaveGame->QuestSaveData.GrantedTagsByQuestComplete = mGrantedTagsByQuestComplete;

	SaveGame->QuestSaveData.CurrentTrackingQuest = mCurrentTrackingQuest;
}

void UQuestComponent::LoadFromSaveData(const UPlayerSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return;
	}

	mCurrentProgressQuests.Empty();
	mCurrentProgressQuests = SaveGame->QuestSaveData.CurrentProgressQuests;

	mCompletedQuests.Empty();
	mCompletedQuests = SaveGame->QuestSaveData.CompletedQuests;


	//mGrantedTagsByQuestComplete = FGameplayTagContainer::EmptyContainer;
	mGrantedTagsByQuestComplete.Reset();
	mGrantedTagsByQuestComplete = SaveGame->QuestSaveData.GrantedTagsByQuestComplete;


	mCurrentTrackingQuest = SaveGame->QuestSaveData.CurrentTrackingQuest;

	mOnUpdatedQuestList.Broadcast(MakeQuestListSnapshot());

	// UI 업데이트 
	if (!mCurrentTrackingQuest.IsNone())
	{
		mOnUpdatedTrackingQuest.Broadcast(MakeQuestSnapshot(mCurrentTrackingQuest));
	}
}

