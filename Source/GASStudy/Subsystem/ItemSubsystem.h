// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"


struct FSlotContext;

UENUM(BlueprintType)
enum class EAddPath : uint8
{
    QuestCompltete,
    Root, 
};

UENUM(BlueprintType)
enum class ERemovePath : uint8
{
    QuestRequired,
    PlayerRemove,
    Sell,
};


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemEvent, const FName& /*ItemID*/, const int32 /*ItemAmount*/);


/**
* 아이템 이벤트 델리게이트
* 아이템 추가 / 제거시 발생
*/
UCLASS()
class GASSTUDY_API UItemSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()


public:
    /**
    * @Interactor 아이템기준 상호작용된 대상(플레이어)
    * @ItemID 아이템 이름
    * @return 성공여부
    * OnItemPickedUp 연동시, 처리 가능
    */
    UFUNCTION(BlueprintCallable)
    bool RequestAddItem(AActor* Interactor, const FName& ItemID, int32 ItemAddAmount);

    /**
    * @Interactor 아이템기준 상호작용된 대상(플레이어)
    * @ItemList 아이템 목록
    * @return 성공여부
    */
    UFUNCTION(BlueprintCallable)
    bool RequestAddItems(AActor* Interactor, EAddPath AddPath, TMap<FName, int32> ItemList);


    /**
    * @Interactor 아이템을 소유를 확인하는 대상
    * @ItemID 아이템 아이디
    * @ItemAmount 소유 개수
    */
    UFUNCTION(BlueprintCallable)
    bool RequestHasItem(AActor* Interactor, const FName& ItemID, int32 ItemNeedAmount);

    /**
    * @Interactor 아이템 제거가 되는 대상
    * @ItemID 아이템 아이디
    * @ItemAmount 삭제개수 개수
    * @bWorldSpawn 삭제시 월드에 스폰시킬 것인지, 말것인지
    */
    UFUNCTION(BlueprintCallable)
    bool RequestRemoveItem(AActor* Interactor, const FName& ItemID, int32 ItemRemoveAmount, bool bWorldSpawn);

    // ==== UI 호출함수 ====
    /**
    * Client 해당 기능을 요청한 액터
    */
    void RequestItemMove(AActor* Client, const FSlotContext& CurrentSlotContext, const FSlotContext& TargetSlotContext);
    /**
    * Client 해당 기능을 요청한 액터
    */
    void RequestClearQuickSlot(AActor* Client, int32 QuickSlotIdx);
    /**
    * 이름이 살짝 애매한데,
    * 슬롯 위에 마우스가 올라오거나, 빠졌을때 호출되는 함수
    * -> 툴팁 Off -> 호버된 아이템과 스텟비교
    */
    void RequestActivateSlotItemInteract(AActor* Client, const FSlotContext& CurrentSlotContext, const FGeometry& InGeometry);
    /**
    * -> 툴팁 Off -> 기존 스탯으로 초기화
    */
    void RequestDeActivateSlotItemInteract(AActor* Client, const FSlotContext& CurrentSlotContext);


public:
    void SetTooltipWidget(class UItemToolTipWidget* TooltipWidget);

private:
    void GetItemStat(AActor* Client, TSubclassOf<class UGameplayEffect> ItemEffect, OUT TMap<struct FGameplayAttribute, float>& ItemOptions);

    const class UBaseItemDataAsset* GetItemData(const FName& ItemID) const;


private:
    UPROPERTY()
    TObjectPtr<class UItemToolTipWidget> mToolTipWidget;

public:
    // 델리게이트: UI나 퀘스트 시스템이 구독
    FOnItemEvent mOnItemPickedUp;
    FOnItemEvent mOnItemDropped;
};
