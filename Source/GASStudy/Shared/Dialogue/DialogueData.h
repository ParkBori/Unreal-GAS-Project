// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueData.generated.h"


USTRUCT(BlueprintType)
struct FUIChoiceData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ChoiceIdx = 0;

    UPROPERTY()
    FName NextNodeID;

    UPROPERTY()
    FText ChoiceScript;

    FUIChoiceData() {}

    FUIChoiceData(
        int32 InChoiceIdx,
        const FName& InNextNodeID,
        const FText& InChoiceScript) 
    : ChoiceIdx(InChoiceIdx)
    , NextNodeID(InNextNodeID)
    , ChoiceScript(InChoiceScript)
    {

    }
};



USTRUCT(BlueprintType)
struct FUIDialogueData
{
    GENERATED_BODY()

    UPROPERTY()
    FText SpeackerName;

    UPROPERTY()
    FText DialogueText;

    UPROPERTY()
    TArray<FUIChoiceData> Choices;
};