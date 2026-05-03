// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipComponent.h"
#include "AbilitySystemComponent.h"



#include "Shared/Macro/Log.h"
#include "Shared/Tag/SharedTag.h"

#include "DeveloperSettings/InventorySystemDeveloperSettings.h"
#include "DeveloperSettings/GameDataSettings.h"

#include "Save/Player/PlayerSaveGame.h"

#include "Subsystem/InventoryPreviewSubsystem.h"
#include "Component/EquipVisualComponent.h"

#include "Weapon/WeaponBase.h"
#include "GameFramework/Character.h"

#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}

void UEquipComponent::Init(UAbilitySystemComponent* ASC)
{
	mASC = ASC;
}

void UEquipComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const UInventorySystemDeveloperSettings* Setting = UInventorySystemDeveloperSettings::Get();
	if (nullptr == Setting)
	{
		MY_LOG(Error, "장비슬롯 초기화 실패")
	}

	const auto& EquipTags = Setting->GetEquipSlotTags();
	int32 TagNums = EquipTags.Num();
	mEquipmentSlots.SetNum(TagNums);
	for (int i = 0; i < TagNums; ++i)
	{
		mEquipmentSlots[i].EquipCategoryTag = EquipTags[i];
	}

	if (UEquipVisualComponent* EVC = GetOwner()->FindComponentByClass<UEquipVisualComponent>())
	{
		mOnChangedItem.AddUObject(EVC, &UEquipVisualComponent::ApplyEquipment);
	}

	if (auto* IPS = GetWorld()->GetSubsystem<UInventoryPreviewSubsystem>())
	{
		if (AActor* Preview = IPS->GetPreviewCharacter())
		{
			if (UEquipVisualComponent* EVC = Preview->FindComponentByClass<UEquipVisualComponent>())
			{
				mOnChangedItem.AddUObject(EVC, &UEquipVisualComponent::ApplyEquipment);
			}
		}
	}
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();


	// NOTE : 인벤토리 캐릭터는 BeginPlay 에서 SubSystem 에 본인을 등록시킴
	// 이때 인벤토리 캐릭터를 포제스나, 포스트 인이셜에서 세팅할 수 있지만, 이들역시 InitializeComponent 보다 후의 시점이다.
	// 모든 컴포넌트 초기화 이후이기 때문에 BeginPlay 시점이 제일 안전하다.
	//if (auto* IPS = GetWorld()->GetSubsystem<UInventoryPreviewSubsystem>())
	//{
	//	if (AActor* Preview = IPS->GetPreviewCharacter())
	//	{
	//		if (UEquipVisualComponent* EVC = Preview->FindComponentByClass<UEquipVisualComponent>())
	//		{
	//			mOnChangedItem.AddUObject(EVC, &UEquipVisualComponent::ApplyEquipment);
	//		}
	//	}
	//}
}

bool UEquipComponent::CanEquipToSlot(const FName& NewItem, const FGameplayTag& SlotTag)
{
	const UBaseItemDataAsset* DA = GetItemData(NewItem);
	if (!DA)
	{
		return false;
	}

	if (!DA->CategoryTag.MatchesTagExact(SlotTag))
	{
		return false;
	}

	EquipItem(DA);
	return true;
}

void UEquipComponent::EquipItem(const UBaseItemDataAsset* DA, bool bLoadEquip)
{
	if (false == mASC.IsValid())
	{
		MY_LOG(Error, "mASC is nullptr");
		return;
	}

	const FGameplayTag& SlotCategory = DA->CategoryTag;
	if (!bLoadEquip)
	{
		FEquipmentSlot* ESL = FindEquipmentSlotByTag(SlotCategory);
		if (nullptr == ESL)
		{
			return;
		}

		// 장착하려는 슬롯에 아이템이 존재하는 경우
		if (ESL->bUsing)
		{
			UnEquipItem(SlotCategory);
		}
		
		// 아이템 추가
		ESL->OwningItem = FOwningItemInfo(DA->KeyName, 1, 1);
		ESL->bUsing = true;
		ESL->EquipItemTag = DA->ItemTag;
	}

	if (DA->CategoryTag.MatchesTagExact(Item::Item_Equipment_Weapon))
	{
		SpawnWeaponActor(DA->SpawnWeapon);
	}

	if (DA->ItemEffect)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = mASC->MakeOutgoingSpec(DA->ItemEffect, 1, mASC->MakeEffectContext());
		if (EffectSpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = mASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
			mApplyedEffects.Add(DA->CategoryTag, ActiveGameplayEffectHandle);
		}
	}
	else
	{
		MY_LOG(Warning, "아이템은 존재하지만 아이템 이펙트가 존재하지 않음");
	}

	ApplyChangedItemToCharacterMesh(DA->CategoryTag, DA->SkeletalMesh, DA->StaticMesh);
	mOnEquipSlotsUpdated.Broadcast(MakeEquipSlotsSnapshot());
}

void UEquipComponent::UnEquipItem(const FGameplayTag& SlotTag)
{
	auto ESL = FindEquipmentSlotByTag(SlotTag);
	if (nullptr == ESL || false == ESL->bUsing)
	{
		return;
	}

	// 적용된 GE 제거 
	if (mApplyedEffects.Contains(ESL->EquipCategoryTag))
	{
		mASC->RemoveActiveGameplayEffect(mApplyedEffects[ESL->EquipCategoryTag]);
		mApplyedEffects.Remove(ESL->EquipCategoryTag);
	}

	// 장비제거
	if (SlotTag.MatchesTagExact(Item::Item_Equipment_Weapon))
	{
		if (mCurrentEquipWeapon)
		{
			DestroyWeaponActor();
		}
	}

	ApplyChangedItemToCharacterMesh(ESL->EquipCategoryTag, nullptr, nullptr);

	// 장비슬롯 초기화
	ESL->Clear();

	mOnEquipSlotsUpdated.Broadcast(MakeEquipSlotsSnapshot());
}

FGameplayTag UEquipComponent::GetEquipWeaponTag()
{
	FGameplayTag WeaponCategoryTag = FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"));
	const FEquipmentSlot* ESL = FindEquipmentSlotByTag(WeaponCategoryTag);
	return ESL->EquipItemTag.RequestDirectParent();
}

FName UEquipComponent::GetEquipItemID(const FGameplayTag& SlotTag)
{
	// TODO: 여기에 return 문을 삽입합니다.

	const auto* SL = FindEquipmentSlotByTag(SlotTag);
	if (!SL)
	{
		return FName();
	}

	return SL->OwningItem.ItemID;
}

void UEquipComponent::SpawnWeaponActor(TSubclassOf<class AWeaponBase> SpawWeapon)
{
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());
	mCurrentEquipWeapon = GetWorld()->SpawnActor<AWeaponBase>(SpawWeapon, Params);

	mCurrentEquipWeapon->AttachToComponent(
		GetOwner<ACharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, mCurrentEquipWeapon->GetAttachSocket());
}

void UEquipComponent::DestroyWeaponActor()
{
	if (!mCurrentEquipWeapon)
	{
		return;
	}

	mCurrentEquipWeapon->Destroy();
}

FEquipmentSlot* UEquipComponent::FindEquipmentSlotByTag(const FGameplayTag& Tag)
{
	// TODO: 여기에 return 문을 삽입합니다.
	for (auto& slot : mEquipmentSlots)
	{
		if (slot.EquipCategoryTag == Tag)
		{
			return&slot;
		}
	}

	return nullptr;
}

void UEquipComponent::ApplyChangedItemToCharacterMesh(const FGameplayTag& SlotTag, const USkeletalMesh* SkeletalMesh, const UStaticMesh* StaticMesh)
{
	mOnChangedItem.Broadcast(SlotTag, SkeletalMesh, StaticMesh);
}

const UBaseItemDataAsset* UEquipComponent::GetItemData(const FName& ItemID) const
{
	const FItemInfoBase* ItemInfo  = UGameDataSettings::Get()->GetDataTableRow<FItemInfoBase>(EGlobalGameDataTableType::ItemData, ItemID);
	return ItemInfo->ItemData;
}

TArray<FInventoryUISnapshot> UEquipComponent::MakeEquipSlotsSnapshot() const
{
	TArray<FInventoryUISnapshot> Result;

	for (const FEquipmentSlot& Item : mEquipmentSlots)
	{
		FInventoryUISnapshot Snapshot;

		if (Item.bUsing)
		{
			const UBaseItemDataAsset* DA = GetItemData(Item.OwningItem.ItemID);
			Snapshot.bEmpty = false;
			Snapshot.ItemID = Item.OwningItem.ItemID;
			Snapshot.CurrentAmount = Item.OwningItem.CurrentAmount;
			Snapshot.Icon = DA->Icon;
		}
		else
		{
			Snapshot.bEmpty = true;
		}

		Result.Add(Snapshot);
	}

	return Result;
}

void UEquipComponent::CompareItemStat(const UBaseItemDataAsset* DA, const TMap<FGameplayAttribute, float>& ItemOptions)
{
	TMap<FGameplayAttribute, float> EquipedItemOption;
	if (mApplyedEffects.Contains(DA->CategoryTag))
	{
		const FActiveGameplayEffect* ActiveEffect = mASC->GetActiveGameplayEffect(mApplyedEffects[DA->CategoryTag]);
		if (ActiveEffect)
		{
			const TArray<FGameplayEffectModifiedAttribute> ModifiedAttributes = ActiveEffect->Spec.ModifiedAttributes;

			const TArray<FGameplayModifierInfo>& ModInfos = ActiveEffect->Spec.Def->Modifiers;
			const TArray<FModifierSpec>& ModSpecs = ActiveEffect->Spec.Modifiers;

			for (int32 i = 0; i < ActiveEffect->Spec.Modifiers.Num(); ++i)
			{
				const FModifierSpec& ModSpec = ModSpecs[i];
				const FGameplayModifierInfo& ModInfo = ModInfos[i];

				EquipedItemOption.Add(ModInfo.Attribute, ModSpec.GetEvaluatedMagnitude());
			}
		}
	}

	// 프리뷰아이템 - 현재아이템
	mItemOptionDiff.Empty();
	const TMap<FGameplayAttribute, float>& CompareTemp = (EquipedItemOption.Num() > ItemOptions.Num()) ? EquipedItemOption : ItemOptions;
	// 옵션 차이값과, 차이 옵션이 적용된 능력치를 
	for (const auto& Option : ItemOptions)
	{
		FStatUISnapshot StatUISnapshot;

		const FGameplayAttribute& OptionName = Option.Key;

		float PreviewItem = ItemOptions.Contains(OptionName) ? ItemOptions[OptionName] : 0.f;
		float EquipItemOption = EquipedItemOption.Contains(OptionName) ? EquipedItemOption[OptionName] : 0.f;

		float CurrentValu = mASC->GetNumericAttribute(OptionName);
		float DiffValue = PreviewItem - EquipItemOption;

		StatUISnapshot.bUsePreview = true;
		StatUISnapshot.CurrentValue = CurrentValu;
		StatUISnapshot.DiffValue = PreviewItem - EquipItemOption;
		StatUISnapshot.PreviewValue = CurrentValu + DiffValue;

		mItemOptionDiff.Add(OptionName, StatUISnapshot);
	}

	mOnStatUpdated.Broadcast(mItemOptionDiff);
}

void UEquipComponent::RemovePreviewStat()
{
	// 슬롯에서 마우스가 해제됐음을 알림
	for (auto& IOD : mItemOptionDiff)
	{
		IOD.Value.bUsePreview = false;
	}

	mOnStatUpdated.Broadcast(mItemOptionDiff);
}

AWeaponBase* UEquipComponent::GetCurrentEquipWeapon() const
{
	return mCurrentEquipWeapon;
}

bool UEquipComponent::CheckFullEquipedState() const
{
	for (const auto& Slot : mEquipmentSlots)
	{
		if (Slot.bUsing == false)
		{
			return false;
		}
	}

	return true;
}

void UEquipComponent::MakeSaveData(UPlayerSaveGame* SaveGame) const
{
	SaveGame->EquipSlotData.Empty();
	for (const FEquipmentSlot& Slot : mEquipmentSlots)
	{
		FEquipSlotSaveData Data;
		Data.bUsing = Slot.bUsing;
		Data.EquipCategoryTag = Slot.EquipCategoryTag;

		if (Slot.OwningItem.IsValid())
		{
			Data.ItemID = Slot.OwningItem.ItemID;
			Data.CurrentAmount = Slot.OwningItem.CurrentAmount;
		}

		//MY_LOG(Warning, "Save - %s", *Data.ToString());
		SaveGame->EquipSlotData.Add(Data);
	}
}

void UEquipComponent::LoadFromSaveData(const UPlayerSaveGame* SaveGame)
{
	if (SaveGame->EquipSlotData.IsEmpty())
	{
		return;
	}

	mEquipmentSlots.Empty();

	for (const FEquipSlotSaveData& Data : SaveGame->EquipSlotData)
	{
		FEquipmentSlot Slot;
		Slot.bUsing = Data.bUsing;
		Slot.EquipCategoryTag = Data.EquipCategoryTag;

		if (!Data.ItemID.IsNone() && Slot.bUsing)
		{
			const UBaseItemDataAsset* DA = GetItemData(Data.ItemID);
			if (DA)
			{
				Slot.OwningItem = FOwningItemInfo(Data.ItemID, Data.CurrentAmount, DA->Possessionlimit);
				Slot.EquipItemTag = DA->ItemTag;
				EquipItem(DA, true);


				ApplyChangedItemToCharacterMesh(DA->CategoryTag, DA->SkeletalMesh, DA->StaticMesh);
			}
		}
		
		//MY_LOG(Warning, "Load - %s", *Data.ToString());
		mEquipmentSlots.Add(Slot);
	}

	// AttributeSet 클래스 포인터를 가져옴
	if (UCharacterAttributeSet* AttributeSet = const_cast<UCharacterAttributeSet*>(mASC->GetSet<UCharacterAttributeSet>()))
	{
		AttributeSet->SetStamina(AttributeSet->GetMaxStamina());
	}

	if (UDefaultAttributeSet* AttributeSet = const_cast<UDefaultAttributeSet*>(mASC->GetSet<UDefaultAttributeSet>()))
	{
		AttributeSet->SetHealth(AttributeSet->GetMaxHealth());
	}

	mOnEquipSlotsUpdated.Broadcast(MakeEquipSlotsSnapshot());
}

