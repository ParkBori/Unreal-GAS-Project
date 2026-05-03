// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InteractComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Interface/InteractableInterface.h"
#include "Components/WidgetComponent.h"
#include "Subsystem/QuestSubsystem.h"
#include "Shared/Macro/Log.h"


// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// ...
}

void UInteractComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	auto* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar)
	{
		return;
	}

	if (auto* PC = OwnerChar->GetController<APlayerController>())
	{
		if (auto* Capsule = OwnerChar->GetCapsuleComponent())
		{
			if (IsValid(Capsule))
			{
				Capsule->OnComponentBeginOverlap.RemoveAll(this);
				Capsule->OnComponentEndOverlap.RemoveAll(this);

				Capsule->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::AddInteractableActor);
				Capsule->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::RemoveInteractableActor);
			}
		}
		else
		{
			MY_LOG(Error, "플레이어 컨트롤러가 존재하지 않음");
		}

		CreateInteractWidget(PC);
	}
	else
	{
		OwnerChar->ReceiveControllerChangedDelegate.AddDynamic(this, &UInteractComponent::Callback_ControllerChanged);
	}
}

void UInteractComponent::Callback_ControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (NewController)
	{
		if (auto* Capsule = Pawn->FindComponentByClass<UCapsuleComponent>())
		{
			Capsule->OnComponentBeginOverlap.RemoveAll(this);
			Capsule->OnComponentEndOverlap.RemoveAll(this);

			Capsule->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::AddInteractableActor);
			Capsule->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::RemoveInteractableActor);
		}

		// 위젯 생성 시에도 NewController를 직접 캐스팅해서 사용
		if (APlayerController* PC = Cast<APlayerController>(NewController))
		{
			CreateInteractWidget(PC);
		}
	}
}

void UInteractComponent::Interact()
{
	if (mCanInteractList.IsValidIndex(0))
	{
		//IInteractableInterface::Execute_Interact(mCanInteractList[0].GetObject(), GetOwner());
		//AActor* Actor = Cast<AActor>(mCanInteractList[0].GetObject());

		AActor* Actor = mCanInteractList[0].Get();
		if (Actor && Actor->Implements<UInteractableInterface>())
		{
			ToggleInteractWidget(true);
			IInteractableInterface::Execute_Interact(Actor, GetOwner());
			//UQuestActorComponent* Cmp = mCanInteractList[0]->FindComponentByClass<UQuestActorComponent>();
			//if (Cmp)
			//{
			//	mOnInteractQuestActor.Broadcast(Cmp->GetQuestID(), Cmp->GetObjectiveID());
			//}
			MY_LOG(Error, "인터렉트 성공");
		}
		else
		{
			MY_LOG(Error, "액터가 존재하지 않거나, 인터렉션 인터페이스 정의가 되어 있지 않음");
		}
	}
	else
	{
		MY_LOG(Error, "상호작용 물체 0번이 존재하지 않음");
	}
}

void UInteractComponent::CreateInteractWidget(APlayerController* PC)
{
	if (mInteractWidgetClass)
	{
		mWidgetComp = Cast<UWidgetComponent>(GetOwner()->AddComponentByClass(UWidgetComponent::StaticClass(), true, FTransform::Identity, false));
		if (IsValid(mWidgetComp))
		{
			auto InteractWidget = CreateWidget<UUserWidget>(PC, mInteractWidgetClass);
			if (IsValid(InteractWidget))
			{
				mWidgetComp->SetWidget(InteractWidget);
				mWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
				mWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mWidgetComp->SetVisibility(false, true);
			}
		}
	}
}

void UInteractComponent::ToggleInteractWidget(bool bEnable)
{
	if (IsValid(mWidgetComp))
	{
		if (bEnable && mCanInteractList.Num())
		{
			const FVector& Location = mCanInteractList[0]->GetActorLocation();
			mWidgetComp->SetWorldLocation(Location);
			mWidgetComp->SetVisibility(true, true);
		}
		else
		{
			mWidgetComp->SetVisibility(false, true);
		}
	}
}

void UInteractComponent::AddInteractableActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UInteractableInterface>())
	{
		/*auto ScriptInterface = TScriptInterface<IInteractableInterface>(OtherActor);
		if (ScriptInterface)
		{
			mCanInteractList.Add(ScriptInterface);
		}*/
		mCanInteractList.Add(OtherActor);
		IInteractableInterface::Execute_ToggleOutline(mCanInteractList[0].Get(), true);
	}

	ToggleInteractWidget(true);
}

void UInteractComponent::RemoveInteractableActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 현재 오버랩 해제된 액터가 상호작용 중인 아이템이라면, 아웃라이너 해제 
	if (mCanInteractList.Num() && mCanInteractList[0].Get() == OtherActor)
	{
		IInteractableInterface::Execute_ToggleOutline(mCanInteractList[0].Get(), false);
	}

	mCanInteractList.RemoveAll([OtherActor](const TWeakObjectPtr<AActor>& Ptr) {
			return Ptr.Get() == OtherActor;
		});

	// 상호작용 리스트에 액터가 있으면, 해당 0번째 엑터에게 아웃라이너 적용 
	if (mCanInteractList.Num())
	{
		IInteractableInterface::Execute_ToggleOutline(mCanInteractList[0].Get(), true);
	}

	ToggleInteractWidget(true);
}
