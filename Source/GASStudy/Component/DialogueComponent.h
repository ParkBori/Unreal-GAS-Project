// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSTUDY_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue, meta = (Tooltip = "재생시킬 다이얼로그 이름", AllowPrivateAccess = true))
	TArray<FName >mDialogueName;
};


