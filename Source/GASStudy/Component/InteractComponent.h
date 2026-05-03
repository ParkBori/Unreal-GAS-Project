// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractQuestActor, const FName&, const FName&);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Callback_ControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

public:
	void Interact();
	void ToggleInteractWidget(bool bEnable);

private:
	void CreateInteractWidget(APlayerController* PC);

private:
	UFUNCTION()
	void AddInteractableActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RemoveInteractableActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:


private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> mInteractWidgetClass;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> mWidgetComp;

	UPROPERTY()
	//TArray<TScriptInterface<class IInteractableInterface>> mCanInteractList;
	TArray<TWeakObjectPtr<AActor>> mCanInteractList;


public:
	FOnInteractQuestActor mOnInteractQuestActor;

};
