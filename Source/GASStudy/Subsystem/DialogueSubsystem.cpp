// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DialogueSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/QuestSubsystem.h"

#include "Shared/Macro/Log.h"

// Quest 
#include "Subsystem/QuestSubsystem.h"
#include "Subsystem/ItemSubsystem.h"

// JSON 
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"



void UDialogueSubsystem::Activate_V2(AActor* Interactor, const FName& FileName)
{
	if (!mDialogueList.Contains(FileName) || mCurrentDialogue.Key != FileName || !Interactor)
	{
		if (false == LoadDialogueFile(FileName.ToString()))
		{
			MY_LOG(Error, "json convert fail");
			return;
		}
	}

	// 재생할 다이얼로그 노드 이름
	FName ActivateNodeName;
	/*if (!mDialogueList.IsEmpty() && mDialogueList.Contains(mCurrentDialogue.Key))
	{
		ActivateNodeName = mDialogueList[mCurrentDialogue.Key];
	}
	else
	{
		ActivateNodeName = TEXT("Start");
	}*/


	ActivateNodeName = TEXT("Start");

	if (!mDialogueNodeCachedMap.Contains(ActivateNodeName))
	{
		MY_LOG(Error, "json convert fail");
		return;
	}

	mInteractor = Interactor;

	// 다이얼로그 활성화 이벤트 호출
	mOnStartDialogueSystem.Broadcast(Interactor);

	// 활성화된 다이얼로그 재생 정보 업데이트
	if (!mDialogueList.IsEmpty() && mDialogueList.Contains(FileName))
	{
		mDialogueList[FileName] = ActivateNodeName;
	}
	else
	{
		mDialogueList.Add(FileName, ActivateNodeName);
	}

	

	EnterNode_V2(ActivateNodeName);
}

void UDialogueSubsystem::SelectChoice(int32 ChoiceIdx)
{
	// 선택된 초이스가 소유한 
	if (mDialogueList.Contains(mCurrentDialogue.Key))
	{
		const FName& CurrentNodeID = mDialogueList[mCurrentDialogue.Key];

		if (mDialogueNodeCachedMap.Contains(CurrentNodeID))
		{
			if (mDialogueNodeCachedMap[CurrentNodeID].Choices.IsValidIndex(ChoiceIdx))
			{
				const FDialogueChoice& SelectedChoice = mDialogueNodeCachedMap[CurrentNodeID].Choices[ChoiceIdx];
				
				// 다이얼로그 보상 (퀘스트 - 완료 + 부여 / 아이템 지금 / 골드 / 태그)

				auto* QS =  GetGameInstance()->GetSubsystem<UQuestSubsystem>();
				if (QS)
				{
					// 완료처리 시킬 퀘스트 
					if (!SelectedChoice.Reward.CompleteQuest.IsNone())
					{
						QS->RequestQuestComplete(SelectedChoice.Reward.CompleteQuest);
						//MY_LOG(Warning, "CompleteQuest");
					}

					// 새롭게 시작할 퀘스트 
					if (!SelectedChoice.Reward.StartQuest.IsNone())
					{
						QS->AddNewQuest(SelectedChoice.Reward.StartQuest);
						//MY_LOG(Warning, "AddQuest");
					}
				}

				auto* IS = GetGameInstance()->GetSubsystem<UItemSubsystem>();
				if (IS)
				{
					if (!SelectedChoice.Reward.GiveItemID.IsNone())
					{
						IS->RequestAddItem(mInteractor.Get(), SelectedChoice.Reward.GiveItemID, 1);
					}
				}

				if (!SelectedChoice.NextNodeID.IsNone())
				{
					// 다음 다이얼로그 재생 
					mDialogueList[mCurrentDialogue.Key] = SelectedChoice.NextNodeID;
					EnterNode_V2(SelectedChoice.NextNodeID);
				}
				else
				{
					// 다이얼로그 종료
					mOnEndDialogueSystem.Broadcast();
				}
			}
		}
	}
}

void UDialogueSubsystem::Next()
{
	if (mDialogueList.Contains(mCurrentDialogue.Key))
	{
		const FName& CurrentNodeID = mDialogueList[mCurrentDialogue.Key];

		if (mDialogueNodeCachedMap.Contains(CurrentNodeID))
		{
			const FName& DefaultNextNodedID = mDialogueNodeCachedMap[CurrentNodeID].DefaultNextNodeID;
			if (!mDialogueNodeCachedMap[CurrentNodeID].DefaultNextNodeID.IsNone())
			{
				mDialogueList[mCurrentDialogue.Key] = DefaultNextNodedID;
				EnterNode_V2(DefaultNextNodedID);
			}
			else
			{
				// 다이얼로그 종료
				mOnEndDialogueSystem.Broadcast();
			}
		}
	}
	else
	{
		// 다이얼로그 종료
		mOnEndDialogueSystem.Broadcast();
	}
}

void UDialogueSubsystem::EnterNode_V2(const FName& NodeID)
{
	// 해당 다이얼로그 완전히 재생시킨 경우
	if (DialogueEndNode  == NodeID)
	{
		CompletedDialogues.Add(mCurrentDialogue.Key);
		// Save 요청

		//이정도가 현재 남은 거같고.. 적어도 2월에서 3월 말에는 완전히 끝낼수 있어보이고
		//여기서 이제 퀘스트쪽 업데이트만 하고, UI 살짝만 손본다음에

		// 마커시스템
		/*생각보다 간단할 수 있는게, 마커는 보통 퀘스트나, 길잡이용으로 쓰이니깐
			마커컴보넌트와 마커서브시스템을 만들어서
			마커컴포넌트 소유 액터들을 마커서브시스템에서 관리.
			이렇게하면, 퀘스트 시작시 마커서브시스템에 등록됐다면 활성화
			마커컴포넌트는 위젯을 소유하게 만들면 됨

		또 놀라운점은, 별도의 오브젝트로 관리할 필요없이 월드에 배치된 물체들에 컴포넌트를 바로 부착할 수 있음!
		이거는 좀 놀라운데? 퀘스트 오브젝트들을 월드에 배치된 얘들에 그
			QuestTargetCompoentn  -> 여기얘들에 마커 등록해도 될지도? */

		//미니맵 넘어가기 


		//전투 시스템 설계

		return;
	}

	const FDialogueNode& ActivateNode = mDialogueNodeCachedMap[NodeID];
	FUIDialogueData UIData;

	// 화자 
	UIData.SpeackerName = FText::FromString(ActivateNode.SpeakerName);

	// 내용
	FString DialogueText = ActivateNode.DialogueText;
	for (const FDialogueVariant& Variant : ActivateNode.Variants)
	{
		if (CheckCondition(Variant.Condition))  // 🔥 이 함수 재사용
		{
			DialogueText = Variant.DialogueText;
			break; // 첫 번째로 만족한 Variant만 사용
		}
	}
	UIData.DialogueText = FText::FromString(DialogueText);

	// 선택지 있는 경우
	if (!ActivateNode.Choices.IsEmpty())
	{
		// 퀘스트 서브시스템 가져오기
		auto* QS = GetGameInstance()->GetSubsystem<UQuestSubsystem>();

		int32 Cnt = 0;
		for (const auto& Choice : ActivateNode.Choices)
		{
			if (CheckCondition(Choice.Condition))
			{
				UIData.Choices.Add(FUIChoiceData(Cnt, Choice.NextNodeID, FText::FromString(Choice.ChoiceText)));
			}
			// Cnt 는 ActivateNode.Choices 의 개수만큼 증가해야하므로, 
			// 관계없이 항상 증가시킨다.
			Cnt++;
		}
	}

	// 이런 정보 만들어서 UI 업데이트시키기 
	mOnUpdateDialogue.Broadcast(UIData);
}

bool UDialogueSubsystem::LoadDialogueFile(const FString& FileName)
{
	// 1. 경로 설정
	FString RelativePath = FString::Printf(TEXT("Data/%s.json"), *FileName);
	FString FullPath = FPaths::ProjectContentDir() + RelativePath;

	FString JsonString;

	// 2. 파일 읽기
	if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
	{
		UE_LOG(LogTemp, Error, TEXT("다이얼로그 파일을 읽지 못했습니다: %s"), *FullPath);
		return false;
	}

	// 3. JSON → USTRUCT 변환
	mCurrentDialogue.Clear();
	if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &mCurrentDialogue.Value, 0, 0))
	{
		mDialogueNodeCachedMap.Empty();
		mCurrentDialogue.Key = *FileName;
		for (const FDialogueNode& Node : mCurrentDialogue.Value.Nodes)
		{
			mDialogueNodeCachedMap.Add(Node.NodeID, Node);
		}

		UE_LOG(LogTemp, Log, TEXT("다이얼로그 로드 완료: %d 개의 노드"), mDialogueNodeCachedMap.Num());
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패: %s"), *FileName);
	return false;
}

bool UDialogueSubsystem::GetDialogueNode(FName NodeID, FDialogueNode& OutNode)
{
	if (const FDialogueNode* FoundNode = mDialogueNodeCachedMap.Find(NodeID))
	{
		OutNode = *FoundNode;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("노드를 찾지 못했습니다: %s"), *NodeID.ToString());
	return false;
}

bool UDialogueSubsystem::CheckCondition(const FDialogueCondition& Condition)
{
	if (Condition.QuestID.IsNone())
	{
		return true;
	}

	auto* QS = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!QS)
	{
		return false;
	}

	// 현재 퀘스트 상태
	EQuestState CurrentState = QS->GetQuestState(Condition.QuestID);

	// Failed 처리
	if (CurrentState == EQuestState::Failed && Condition.bHideIfFailed)
	{
		return false;
	}

	// 상태 비교
	return CurrentState == Condition.RequiredState;
}
