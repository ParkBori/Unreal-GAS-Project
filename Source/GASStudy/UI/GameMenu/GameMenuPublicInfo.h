// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

//#include "UObject/NoExportTypes.h"
//#include "GameMenuPublicInfo.generated.h"


#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EGameMenuType : uint8
{
	EQUIP,
	INVENTORY,
	SKILL,
	QUEST,
	OPTION,
};