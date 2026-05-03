// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GC_StaticBase.generated.h"

/**
 * 타겟 위치에서 작동하는 GC
 */
UCLASS()
class GASSTUDY_API UGC_TargetLocation : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
protected:
	UGC_TargetLocation();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<UParticleSystem> mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem>	mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<USoundBase>	mSound;
};


/**
* HitResult ImpactPoint에서 작동하는 GC
*/
UCLASS()
class GASSTUDY_API UGC_ImpactPoint : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

protected:
	UGC_ImpactPoint();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<UParticleSystem> mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem>	mNiagara;

	// 이방식 말고 사운드큐쪽에서 사운드를 넣어놓고 랜덤으로 재생시키는 방법이 있던거같은데..
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TArray<TObjectPtr<USoundBase>>	mSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UCameraShakeBase> mCameraShake;

};


UCLASS()
class GASSTUDY_API UGC_Impact_AllInOne : public UGameplayCueNotify_Static
{
    GENERATED_BODY()

public:

    virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:

    /* =========================
       옵션 토글 (핵심 ⭐)
    ========================= */

    UPROPERTY(EditDefaultsOnly, Category = "Options")
    bool bUseEffect = true;

    UPROPERTY(EditDefaultsOnly, Category = "Options")
    bool bUseSound = true;

    UPROPERTY(EditDefaultsOnly, Category = "Options")
    bool bUseCameraShake = true;

    UPROPERTY(EditDefaultsOnly, Category = "Options")
    bool bUseHitStop = true;

    UPROPERTY(EditDefaultsOnly, Category = "Options")
    bool bUseSlowMotion = false;

    /* =========================
       이펙트
    ========================= */

    UPROPERTY(EditDefaultsOnly)
    UParticleSystem* mParticle;

    UPROPERTY(EditDefaultsOnly)
    UNiagaraSystem* mNiagara;

    /* =========================
       사운드
    ========================= */

    UPROPERTY(EditDefaultsOnly)
    TArray<USoundBase*> mSounds;

    /* =========================
       카메라 쉐이크
    ========================= */

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UCameraShakeBase> mCameraShake;

    /* =========================
       튜닝 값
    ========================= */

    UPROPERTY(EditDefaultsOnly)
    float HitStopTime = 0.03f;

    UPROPERTY(EditDefaultsOnly)
    float SlowMotionTime = 0.2f;

    UPROPERTY(EditDefaultsOnly)
    float SlowMotionScale = 0.2f;

    UPROPERTY(EditDefaultsOnly)
    float ShakeScale = 1.0f;
};