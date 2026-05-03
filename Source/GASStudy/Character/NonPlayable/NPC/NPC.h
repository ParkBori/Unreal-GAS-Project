// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NonPlayable/NonPlayableCharacter.h"
#include "Interface/InteractableInterface.h"
#include "NPC.generated.h"

/**
 * 
 */
UCLASS()
class GASSTUDY_API ANPC 
	: public ANonPlayableCharacter
	, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ANPC();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDialogueComponent> mDialogueComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> mTriggerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> mSpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> mCameraComponent;

};
