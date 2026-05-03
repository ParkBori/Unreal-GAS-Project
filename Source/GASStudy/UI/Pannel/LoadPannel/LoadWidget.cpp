// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pannel/LoadPannel/LoadWidget.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Pannel/LoadPannel/LoadEntityWidget.h"
#include "UI/Pannel/ButtonPannel/BaseButtonWidget.h"

#include "Subsystem/SaveSubsystem.h"
#include "Subsystem/WorldTransitionSubsystem.h"
// 현재 여기서 세이브 시스템에 대한 정보처리가 좀 이상한데, 
// 여기엤는 기능들이 세이브서브시스템으로 이동후에, 거기서부터 정보를얻어오는 방식으로 변경해야함

void ULoadWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	USaveSubsystem* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (SS)
	{
		SS->mOnCompleteMetaSaveGameLoad.AddUObject(this, &ThisClass::RefreshSaveList);
	}

	if (Btn_Back)
	{
		Btn_Back->OnClicked().AddLambda([&]() { mOnClickedBackButn.Broadcast(); });
	}

	if (Btn_Load)
	{
		Btn_Load->OnClicked().AddUObject(this, &ThisClass::Callback_ClickedLoadBtn);
	}

	if (Btn_Remove)
	{
		Btn_Remove->OnClicked().AddUObject(this, &ThisClass::Callback_ClickedRemoveBtn);
	}

	/**
	* 세이브가 로드 된 이후, 로드 위젯을 열면 콜백함수가 호출되지 않아
	* 세이브 목록이 업데이트 되지 않을 가능성이 존재
	* 그렇기때문에 수동으로 리프레시 시켜줘야 한다.
	*/
	RefreshSaveList();
}

void ULoadWidget::Reset()
{
	mSelectedSaveSlotName = "";

	for (const auto& pEntity : mCachedLoadEntities)
	{
		pEntity.Value->SetSelected(false);
	}
}

void ULoadWidget::RefreshSaveList()
{
	if (!SB_LoadEntityList)
	{
		return;
	}
	
	USaveSubsystem* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}

	// 세이브 파일 정보 가져오기
	TMap<FString, FString> SaveFileData;
	SS->GetSaveFileData(SaveFileData);

	SB_LoadEntityList->ClearChildren();
	mCachedLoadEntities.Empty();

	int32 Idx = 0;
	for (const auto& SFD : SaveFileData)
	{
		ULoadEntityWidget* LE = CreateWidget<ULoadEntityWidget>(GetOwningPlayer(), mLoadEntityClass);
		if (LE)
		{
			LE->mOnEntitySelected.AddUObject(this, &ThisClass::Callback_EntitySelected);
			
			// UTC를 로컬 시간으로 변환 (사용자 PC 설정 기준)
			LE->Init(Idx++, SFD.Key, SFD.Value);
			if (auto* SBS = Cast<UScrollBoxSlot>(SB_LoadEntityList->AddChild(LE)))
			{
				SBS->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
			}
			
			mCachedLoadEntities.Add(*SFD.Key, LE);
		}
	}
}


void ULoadWidget::Callback_EntitySelected(const FName& SelectEntityName)
{
	if (SelectEntityName.IsNone())
	{
		return;
	}

	// 선택 된 맵 캐싱
	mSelectedSaveSlotName = SelectEntityName;

	for (const auto& pEntity : mCachedLoadEntities)
	{
		if (pEntity.Key != SelectEntityName)
		{
			pEntity.Value->SetSelected(false);
		}
		else
		{
			pEntity.Value->SetSelected(true);
		}
	}
}

void ULoadWidget::Callback_ClickedLoadBtn()
{
	if (mSelectedSaveSlotName.IsNone())
	{
		return;
	}

	USaveSubsystem* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}

	SS->SelectSavedGameBySlotName(mSelectedSaveSlotName);

	auto* WTS = GetGameInstance()->GetSubsystem<UWorldTransitionSubsystem>();
	if (!WTS)
	{
		return;
	}

	FTransitionRequest Request;
	Request.LevelName = TEXT("Lvl_Temple");
	Request.WorldTransitionType = EWorldTransitionType::OpenLevel;
	Request.IsNewGame = false;

	WTS->RequestTransition(Request);
}

void ULoadWidget::Callback_ClickedRemoveBtn()
{
	if (mSelectedSaveSlotName.IsNone())
	{
		return;
	}

	USaveSubsystem* SS = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!SS)
	{
		return;
	}
	
	SS->RequestRemoveSaveFile(mSelectedSaveSlotName);

	RefreshSaveList();
}
