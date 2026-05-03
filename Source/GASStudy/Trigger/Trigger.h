// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerOverlapped);

/*
*/
UCLASS()
class GASSTUDY_API ATrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrigger();

protected:
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void ActivateTriggerEvent(AActor* OtherActor);
	void ActivateTriggerEvent_Implementation(AActor* OtherActor) {}

private:
	UFUNCTION()
	void Callback_TriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "TriggerBox"))
	TObjectPtr<class UBoxComponent> mBoxComp;
	
	UPROPERTY(BlueprintAssignable)
	FOnTriggerOverlapped mOnTriggerOverlapped;
};
