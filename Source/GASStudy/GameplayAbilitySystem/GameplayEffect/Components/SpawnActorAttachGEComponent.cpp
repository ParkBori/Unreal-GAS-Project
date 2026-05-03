// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayEffect/Components/SpawnActorAttachGEComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


FSpawnAttachedNiagaraParameters::FSpawnAttachedNiagaraParameters()
	: mAttachType(EAttachmentRule::SnapToTarget, false)
{
}

USpawnNiagaraAttachGEComponent::USpawnNiagaraAttachGEComponent()
{
}

bool USpawnNiagaraAttachGEComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	// 에셋 유효성 검사 (TSoftObjectPtr 사용 가정)
	if (mParameters.mNiagaraAsset.IsNull())
	{
		return true;
	}
	
	// 타겟 액터 확보
	UAbilitySystemComponent* TargetASC = ActiveGE.Handle.GetOwningAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetOwner() : nullptr;
	if (!TargetActor)
	{
		return true;
	}

	// 이펙트 핸들을 캡처하여 로드 완료 시점에 이펙트가 여전히 살아있는지 확인용으로 사용
	FActiveGameplayEffectHandle GEHandle = ActiveGE.Handle;
	
	// 파라미터 복사 (람다 내부에서 사용하기 위함)
	FSpawnAttachedNiagaraParameters LocalParams = mParameters;

	// 비동기 로드 요청
	TSoftObjectPtr<UNiagaraSystem> NiagaraSoftPtr = LocalParams.mNiagaraAsset;
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		NiagaraSoftPtr.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([GEHandle, NiagaraSoftPtr, TargetActor, LocalParams]()
		{
                if (!IsValid(TargetActor)) 
                {
                    return;
                }

                UAbilitySystemComponent* ASC = GEHandle.GetOwningAbilitySystemComponent();
                if (!ASC)
                {
                    return;
                }
                // 현재 이펙트 인스턴스를 핸들로 찾음
                const FActiveGameplayEffect* CurrentGE = ASC->GetActiveGameplayEffect(GEHandle);
                if (!CurrentGE)
                {
                    return;
                }

                UNiagaraSystem* LoadedSystem = NiagaraSoftPtr.Get();
                if (!LoadedSystem)
                {
                    return;
                }

                // 2. 나이아가라 컴포넌트 생성 및 부착
                UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
                    LoadedSystem,
                    TargetActor->GetRootComponent(),
                    LocalParams.mSocketName,
                    FVector::ZeroVector,
                    FRotator::ZeroRotator,
                    EAttachLocation::KeepRelativeOffset,
                    true
                );

                if (NiagaraComponent)
                {
                    // FActiveGameplayEffect의 bInhibited 플래그를 직접 확인합니다.
                    if (CurrentGE->bIsInhibited)
                    {
                        NiagaraComponent->Deactivate();
                    }

                    // 3. 이벤트 바인딩 (이전과 동일하게 처리)
                    FActiveGameplayEffect* MutableGE = const_cast<FActiveGameplayEffect*>(CurrentGE);

                    MutableGE->EventSet.OnEffectRemoved.AddWeakLambda(NiagaraComponent, [NiagaraComponent](const FGameplayEffectRemovalInfo& RemovalInfo)
                        {
                            if (IsValid(NiagaraComponent)) NiagaraComponent->DestroyComponent();
                        });

                    MutableGE->EventSet.OnInhibitionChanged.AddWeakLambda(NiagaraComponent, [NiagaraComponent](FActiveGameplayEffectHandle Handle, bool bIsInhibited)
                        {
                            if (IsValid(NiagaraComponent))
                            {
                                if (bIsInhibited) NiagaraComponent->Deactivate();
                                else NiagaraComponent->Activate(true);
                            }
                        });
                }
		})
	);

	return true;
}

void USpawnNiagaraAttachGEComponent::SetParameters(const FSpawnAttachedNiagaraParameters& Parameters)
{
	mParameters = Parameters;
}


