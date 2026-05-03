// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_ApplyGameplayTagToEnemy.generated.h"

/**
 * 현재 Player 과 Enemy 모두 동일한 애니메이션이 겹쳐서 사용하고 있다. 
 * 이때 플레이어가 애니메이션을 작동시킬때 적에게 태그를 부여해줘야 하는 상황이 생긴다
 * 이 ANS 는 플레이어가 애니메이션을 재생시킬때 타겟에게 태그를 부여한다.
 * 
 * ex) Player 과 Enemy 둘 다 사용하는 공격 애니메이션에서 
 * Player 가 공격할때 Enemy 에게 이 구간에 Block 이 가능함을 태그로 부여한다. 
 * 
 * 즉, 플레이어가 해당 애니메이션 재생시킬때, 적에게 붙여줘야하는 태그
 */




UCLASS()
class GASSTUDY_API UANS_ApplyGameplayTagToEnemy : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_ApplyGameplayTagToEnemy();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "GameplayTag")
	FGameplayTag ApplyGameplayTag;
};
