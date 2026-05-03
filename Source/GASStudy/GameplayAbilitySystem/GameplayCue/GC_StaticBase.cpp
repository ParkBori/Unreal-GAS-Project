// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GameplayCue/GC_StaticBase.h"
#include "Kismet/GameplayStatics.h"

UGC_TargetLocation::UGC_TargetLocation()
{
}

bool UGC_TargetLocation::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	if (!Target)
	{
		return false;
	}

	if (IsValid(mParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mParticle,
			Target->GetActorLocation());
	}

	if (IsValid(mNiagara))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagara,
			Target->GetActorLocation());
	}

	if (IsValid(mSound))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mSound,
			Target->GetActorLocation());
	}

	return true;
}

UGC_ImpactPoint::UGC_ImpactPoint()
{
}

bool UGC_ImpactPoint::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (!HitResult)
	{
		return false;
	}

	const FVector& ImpacPoint = HitResult->ImpactPoint;

	if (IsValid(mParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mParticle,
			ImpacPoint);
	}

	if (IsValid(mNiagara))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagara,
			ImpacPoint);
	}

	if (!mSound.IsEmpty())
	{
		int Size = mSound.Num();
		int Idx = FMath::RandRange(0, Size - 1);

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mSound[Idx],
			ImpacPoint);
	}

	if (mCameraShake)
	{
		if (APawn* Pawn = Cast<APawn>(Parameters.GetInstigator()))
		{
			if (Pawn->IsLocallyControlled())
			{
				if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
				{
					if (HitResult)
					{
						// 1. 히트 방향 계산
						FVector HitDir = -HitResult->ImpactNormal;
						HitDir.Normalize();

						// 2. 카메라 기준 벡터
						FRotator CamRot = PC->PlayerCameraManager->GetCameraRotation();
						FVector CamForward = CamRot.Vector();
						FVector CamRight = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);

						// 3. 방향 분해
						float ForwardDot = FVector::DotProduct(CamForward, HitDir);
						float RightDot = FVector::DotProduct(CamRight, HitDir);

						// 4. 각도 변환 (튜닝값)
						const float MaxYaw = 10.f;
						const float MaxPitch = 8.f;

						float YawOffset = RightDot * MaxYaw;
						float PitchOffset = -ForwardDot * MaxPitch;

						// 5. 쉐이크 시작
						PC->ClientStartCameraShake(mCameraShake);

						// 6. 카메라 즉시 오프셋 (임팩트 느낌 강화)
						FRotator NewRot = CamRot;
						NewRot.Yaw += YawOffset;
						NewRot.Pitch += PitchOffset;

						PC->SetControlRotation(NewRot);
					}
					else
					{
						PC->ClientStartCameraShake(mCameraShake);
					}
				}
			}
		}
	}

	return true;
}


//현재 플레이 공격이 막혔을때만 카메라 쉐이크가 발생하는데
//이부분을 반대로 수정해야함


bool UGC_Impact_AllInOne::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
    if (!Target) return false;

    const FHitResult* Hit = Parameters.EffectContext.GetHitResult();
    if (!Hit) return false;

    const FVector ImpactPoint = Hit->ImpactPoint;
    AActor* Instigator = Parameters.GetInstigator();

    /* =========================
       1. 이펙트
    ========================= */
    if (bUseEffect)
    {
        if (mParticle)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mParticle, ImpactPoint);
        }

        if (mNiagara)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagara, ImpactPoint);
        }
    }

    /* =========================
       2. 사운드
    ========================= */
    if (bUseSound && mSounds.Num() > 0)
    {
        int32 Idx = FMath::RandRange(0, mSounds.Num() - 1);
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mSounds[Idx], ImpactPoint);
    }

    /* =========================
       3. 히트스탑
    ========================= */
    if (bUseHitStop)
    {
        auto ApplyHitStop = [&](AActor* Actor)
            {
                if (!Actor) return;

                Actor->CustomTimeDilation = 0.f;

                FTimerHandle Timer;
                GetWorld()->GetTimerManager().SetTimer(Timer, [Actor]()
                    {
                        Actor->CustomTimeDilation = 1.f;
                    }, HitStopTime, false);
            };

        ApplyHitStop(Target);
        ApplyHitStop(Instigator);
    }

    /* =========================
       4. 슬로우모션 (글로벌)
    ========================= */
    if (bUseSlowMotion)
    {
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMotionScale);

        FTimerHandle Timer;
        GetWorld()->GetTimerManager().SetTimer(Timer, [World = GetWorld()]()
            {
                UGameplayStatics::SetGlobalTimeDilation(World, 1.f);
            }, SlowMotionTime, false);
    }

    /* =========================
       5. 카메라 쉐이크 (방향 기반 ⭐)
    ========================= */
    if (bUseCameraShake && mCameraShake && Target)
    {
        if (APawn* Pawn = Cast<APawn>(Target))
        {
            if (Pawn->IsLocallyControlled())
            {
                if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
                {
                    // 방향 계산
                    FVector HitDir = -Hit->ImpactNormal;
                    HitDir.Normalize();

                    FRotator CamRot = PC->PlayerCameraManager->GetCameraRotation();
                    FVector CamForward = CamRot.Vector();
                    FVector CamRight = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);

                    float ForwardDot = FVector::DotProduct(CamForward, HitDir);
                    float RightDot = FVector::DotProduct(CamRight, HitDir);

                    const float MaxYaw = 10.f;
                    const float MaxPitch = 8.f;

                    float YawOffset = RightDot * MaxYaw;
                    float PitchOffset = -ForwardDot * MaxPitch;

                    // 쉐이크
                    PC->ClientStartCameraShake(mCameraShake, ShakeScale);

                    // 방향 반응
                    FRotator NewRot = CamRot;
                    NewRot.Yaw += YawOffset;
                    NewRot.Pitch += PitchOffset;

                    PC->SetControlRotation(NewRot);
                }
            }
        }
    }

    return true;
}