// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/NonPlayable/Enemy/Enemy.h"
#include "Boss.generated.h"


namespace  EBossOutlineType
{
	enum Type : uint8
	{
		None,
		Critical = 1,
		SuperArmor = 3,
	};
}

/**
 * 
 */
UCLASS()
class GASSTUDY_API ABoss : public AEnemy
{
	GENERATED_BODY()
	
public:

public:
	/**
	* 보스전 시작 이벤트 / 
	* ex) 시네마 컷신 재생 or 대화 
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventStartBossFight();
	virtual void EventStartBossFight_Implementation();

	/**
	* 보스전 끝났을때 이벤트
	* ex) 시네마 컷신 재생 or 대화
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventBossFightEnd();
	virtual void EventBossFightEnd_Implementation();
	
	/**
	* 죽었을 때 호출되는 함수
	*/
	virtual void EventDeath(const FGameplayTag Tag, int32 NewCount) override;
};
