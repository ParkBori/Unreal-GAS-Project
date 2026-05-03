// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/UIInfo.h"
#include "UIHelperFunctionLibrary.generated.h"

class UPopupWidget;
/**
 * 
 */
UCLASS()
class GASSTUDY_API UUIHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	* @param InBrush : 유아의 이미지의 머티리얼을 소유하는 개체
	* @param InOuter : 생성되는 다이나믹 머티리얼 소유하는 객체
	* 이미지가 머티리얼리라면, 다이나믹 머티리일로 만들어서 반환
	*/
	UFUNCTION(BlueprintCallable)
	static UMaterialInstanceDynamic* CreateDynamicMaterilFrom(const FSlateBrush& InBrush, UObject* InOuter);

	/**
	* @param Widget : 애니메이션을 소유한 UI 
	* @param AnimName : 위젯 애니메이션 이름 
	* 동일한 이름의 애니메이션 반환
	*/
	UFUNCTION(BlueprintCallable)
	static UWidgetAnimation* FindAnimation(UUserWidget* Widget, const FString& AnimName);

	/**
	* @param AnimName : 재생시킬 애니메이션 이름
	* @param FunctionName : 애니메이션 종료시점에 호출되야 하는 함수 이름
	* 애니메이션 재생 + 종료시점에 콜백함수 바인딩을 한번에 처리하는 함수
	*/
	UFUNCTION(BlueprintCallable)
	static UWidgetAnimation* PlayWidgetAnimation(UUserWidget* Widget, const FString& AnimName, float PlayStartTime = 0.f,
		float PlaySpeed = 1.f, bool Forward = true, bool RestoreState = false,
		int32 LoopCount = 1);

	UFUNCTION(BlueprintCallable)
	static UPopupWidget* CreatePopup(FText Title, FText Script);

	UFUNCTION(BlueprintCallable)
	static UPopupWidget* CreatePopupTimer(FText Title, FText Script, float Time);
};

