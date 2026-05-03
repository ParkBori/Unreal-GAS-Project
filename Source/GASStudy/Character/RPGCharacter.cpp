// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGCharacter.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GameplayAbilitySystem/Attribute/AttributeSetDefault.h"

#include "AbilitySystemBlueprintLibrary.h"


#include "Kismet/GameplayStatics.h"

#include "Components/TimelineComponent.h"

#include "UI/HUD/PlayerHUD.h"
#include "UI/UIManager.h"
#include "Component/FootIKComponent.h"

#include "GASStudy.h"

#include "Shared/Tag/SharedTag.h"

#include "GameplayAbilitySystem/GASStudyAbilitySystemGlobals.h"
#include "Shared/GAS/GASData.h"


// Sets default values
ARPGCharacter::ARPGCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName("GASCharacter");

	// Configure character movement
	auto* CM = GetCharacterMovement();
	if (CM)
	{
		CM->bOrientRotationToMovement = true; 	
		CM->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 
	}

	SlowmotionTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	// void SetFloatCurve(UCurveFloat * NewFloatCurve, FName FloatTrackName);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveAsset(TEXT("/Script/Engine.CurveFloat'/Game/SlowmotionCurve.SlowmotionCurve'"));
	if (CurveAsset.Succeeded())
	{
		SlowmotionCurve = CurveAsset.Object;
	}
	SlowmotionTimelineComponent->SetFloatCurve(SlowmotionCurve, TEXT("Dilation"));
	SlowmotionTimelineComponent->SetTimelineLength(0.5);
	SlowmotionTimelineComponent->SetIgnoreTimeDilation(true);


	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	mDefaultAttributeSet = CreateDefaultSubobject<UDefaultAttributeSet>(TEXT("DefaultAttributeSet"));

	CharacterLevel = 1;
}

FGenericTeamId ARPGCharacter::GetGenericTeamId() const
{
	return TeamID;
}

bool ARPGCharacter::ActivateAbilityWithTags(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent)
	{
		return false;	
	}

	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation);
}


void ARPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TeamID = FGenericTeamId(Fraction);

	if (!AbilitySystemComponent /*|| !AttributeSetBase*/)
	{
		MY_LOG(Error, "어빌리티시스템 or 어트리뷰트셋")
		return;
	}
	
	auto Aset = GetAbilitySystemComponent()->GetSet<UDefaultAttributeSet>();
	auto asdf = GetAbilitySystemComponent()->GetSet<UEnemyAttributeSet>();


	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (EnableTestAbilites)
	{
		SetTestAbilites();
	}


	Aset = GetAbilitySystemComponent()->GetSet<UDefaultAttributeSet>();
	asdf = GetAbilitySystemComponent()->GetSet<UEnemyAttributeSet>();

	TArray<FGameplayAttribute> OwningAttributeSet;
	GetAbilitySystemComponent()->GetAllAttributes(OwningAttributeSet);

	// 스텟 초기화 
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(AbilitySystemComponent, GetCharacterName(), GetCharacterLevel(), true);

	// 디폴트 GE 적용
	ApplyDefaultsGE();

	// 어빌리티시스템 관련 델리게이트 바인드
	BindDelegateToAbilitySystem();
}

void ARPGCharacter::ApplyDefaultsGE()
{
	const auto* DT = UGASStudyAbilitySystemGlobals::Get().GetDefaultAbilityDataTable();
	if (DT)
	{
		const auto* RowData = DT->FindRow<FGASLevelData>(GetCharacterName(), "");
		if (RowData)
		{
			FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
			Context.AddSourceObject(this);

			for (auto& DefaulttEffect : RowData->DefaultEffects)
			{
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaulttEffect, GetCharacterLevel(), Context);
				if (SpecHandle.IsValid())
				{
					// 게임플레이 이펙트가 성공적으로 적용되면 FActiveGameplayEffectHandle 에 값이 저장된다.
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
				}
			}
		}
	}
}

void ARPGCharacter::RemoveDefaultAttributeEffects()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);
}

void ARPGCharacter::SetGait(EGait NewGait)
{
	if (NewGait == CurrentGait)
	{
		return;
	}

	UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (CMC)
	{
		switch (NewGait)
		{
		case EGait::Jog:
			WantSpeed = 150;
			CMC->MaxWalkSpeed = 150.f;
			CMC->BrakingDecelerationWalking = 500.0f;
			break;
		case EGait::Run:
			CMC->MaxWalkSpeed = 1000.f;
			//CMC->BrakingDecelerationWalking = 1000.f;
			break;
		}
		CurrentGait = NewGait;
	}
}

void ARPGCharacter::SetStance(EStance NewStance)
{
	if (NewStance == CurrentStance)
	{
		return;
	}

	CurrentStance = NewStance;
}

void ARPGCharacter::UpdateCharacterSpeed(float DeltaTime)
{
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	
	if (CMC)
	{
		GetCharacterMovement()->MaxWalkSpeed =
			FMath::FInterpTo(CMC->MaxWalkSpeed, WantSpeed, DeltaTime, SpeedInterpSpeed);
	}
}

void ARPGCharacter::BindDelegateToAbilitySystem()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(State::State_Dead, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ARPGCharacter::EventDeath);

	// 모든 태그 감지
	AbilitySystemComponent->RegisterGenericGameplayTagEvent()
		.AddUObject(this, &ARPGCharacter::Callback_AnyTagChanged);
}

void ARPGCharacter::EventDeath(const FGameplayTag Tag, int32 NewCount)
{
	mOnDead.Broadcast();
}

void ARPGCharacter::SetMovementModeAndCollision(EMovementMode NewMovementMode, ECollisionEnabled::Type NewCollisionEnabled)
{
	GetCharacterMovement()->SetMovementMode(NewMovementMode);
	GetCapsuleComponent()->SetCollisionEnabled(NewCollisionEnabled);
}

void ARPGCharacter::SetMeleeAttackAbility()
{
	if (!AbilitySystemComponent || MeleeAttackAbility == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("MeleeAttackAbility Isn't"));
		return;
	}			
	MeleeAttackAbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MeleeAttackAbility, GetCharacterLevel(), INDEX_NONE, this));
}

bool ARPGCharacter::ActivateMeleeAttackAbility(bool AllowRemoteActivate)
{
	if (!AbilitySystemComponent || !MeleeAttackAbilitySpecHandle.IsValid())
	{
		return false;
	}

	return AbilitySystemComponent->TryActivateAbility(MeleeAttackAbilitySpecHandle);
}

void ARPGCharacter::GetActiveAbilitiesWitTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	TArray<FGameplayAbilitySpec*> MatcihngAbilities;
	//AbilitySystemComponent->GetActivatableAbilities();
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, MatcihngAbilities, MatchExactTag);
	for (FGameplayAbilitySpec* Spec : MatcihngAbilities)
	{
		TArray<UGameplayAbility*>  AbilityInstance =  Spec->GetAbilityInstances();
		for (UGameplayAbility* ActiveAbility : AbilityInstance)
		{
			if (ActiveAbility->IsActive())
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}
}

void ARPGCharacter::ApplyGameplayEffect(TSubclassOf<class UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

bool ARPGCharacter::CanApplyGameplayEffect(TSubclassOf<class UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return false;
	}
	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);
	// 여기 레벨이 동적으로 적용되어야한다
	// TODO: 여기서 1레벨이 아닌 실제 레벨을 얻어와야한다. 아마 GetSetByCaller 을 사용
	return AbilitySystemComponent->CanApplyAttributeModifiers(GameplayEffect.GetDefaultObject(), GetCharacterLevel(), Context);
}

bool ARPGCharacter::ActivateAbilityBySlot(TEnumAsByte<EAbilitySlot> AbilitySlot, bool AllowRemoteActivate)
{
	if (!AbilitySystemComponent || !SlotAbilityHandles.Contains(AbilitySlot))
	{
		return false;
	}

	FGameplayAbilitySpecHandle* SpecHandle = SlotAbilityHandles.Find(AbilitySlot);
	return AbilitySystemComponent->TryActivateAbility(FGameplayAbilitySpecHandle(*SpecHandle), AllowRemoteActivate);
}

void ARPGCharacter::ClearAbilitySlot(TEnumAsByte<EAbilitySlot> AbilitySlot)
{
	if (SlotAbilityHandles.IsEmpty())
	{
		return;
	}

	if (SlotAbilityHandles.Contains (AbilitySlot))
	{
		FGameplayAbilitySpecHandle* SpecHandle = SlotAbilityHandles.Find(AbilitySlot);
		AbilitySystemComponent->ClearAbility(*SpecHandle);
		SlotAbilityHandles.Remove(AbilitySlot);
	}
}

void ARPGCharacter::AddAbilitySlot(TSubclassOf<UGameplayAbility> NewAbility, TEnumAsByte<EAbilitySlot> AbilitySlot)
{
	if ((!AbilitySystemComponent) || nullptr == NewAbility)
	{
		return;		
	}

	FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NewAbility, 1, INDEX_NONE, this));
	if (SpecHandle.IsValid())
	{
		SlotAbilityHandles.Add(AbilitySlot, SpecHandle);
	}
}

void ARPGCharacter::SetTestAbilites()
{
	if (!AbilitySystemComponent)
	{
		return;	
	}
	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ARPGCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	}
}

void ARPGCharacter::ChangeOutline(ECharacterOutlineType::Type OutlineType)
{
	TArray<USkeletalMeshComponent*> SkeletalMeshes;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshes);

	TArray<UStaticMeshComponent*> StaticMeshes;
	GetComponents<UStaticMeshComponent>(StaticMeshes);

	switch (OutlineType)
	{
	case ECharacterOutlineType::None:
		for (auto SM : SkeletalMeshes)
		{
			SM->SetRenderCustomDepth(false);
		}
		for (auto SM : StaticMeshes)
		{
			SM->SetRenderCustomDepth(false);
		}
		break;
	case ECharacterOutlineType::Critical:
		for (auto SM : SkeletalMeshes)
		{
			SM->SetRenderCustomDepth(true);
			SM->SetCustomDepthStencilValue(ECharacterOutlineType::Critical);
		}
		for (auto SM : StaticMeshes)
		{
			SM->SetRenderCustomDepth(true);
			SM->SetCustomDepthStencilValue(ECharacterOutlineType::Critical);
		}
		break;
	case ECharacterOutlineType::SuperArmor:
		for (auto SM : SkeletalMeshes)
		{
			SM->SetRenderCustomDepth(true);
			SM->SetCustomDepthStencilValue(ECharacterOutlineType::SuperArmor);
		}
		for (auto SM : StaticMeshes)
		{
			SM->SetRenderCustomDepth(true);
			SM->SetCustomDepthStencilValue(ECharacterOutlineType::SuperArmor);
		}
		break;
	case ECharacterOutlineType::Groggy:
	{
		for (auto SM : SkeletalMeshes)
		{
			SM->SetRenderCustomDepth(true);
			SM->SetCustomDepthStencilValue(ECharacterOutlineType::Groggy);
		}
		for (auto SM : StaticMeshes)
		{
			SM->SetRenderCustomDepth(true);
			SM->SetCustomDepthStencilValue(ECharacterOutlineType::Groggy);
		}
		break;
	}
	}
}

void ARPGCharacter::Callback_AnyTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (Tag.MatchesTagExact(State::State_SuperArmor))
		{
			ChangeOutline(ECharacterOutlineType::SuperArmor);
		}
		else if (Tag.MatchesTagExact(State::State_Critical))
		{
			ChangeOutline(ECharacterOutlineType::Critical);
		}
		else if (Tag.MatchesTagExact(State::State_Groggy))
		{
			ChangeOutline(ECharacterOutlineType::Groggy);
		}
	}
	else
	{
		if (Tag.MatchesTagExact(State::State_SuperArmor)
			|| Tag.MatchesTagExact(State::State_Critical)
			|| Tag.MatchesTagExact(State::State_Groggy))
		{
			ChangeOutline(ECharacterOutlineType::None);
		}
	}
}


