// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class GASSTUDY_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void FireInDirection(AActor* ProjectileOwner);

protected:
	UFUNCTION()
	void BoxOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void BoxHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<class UBoxComponent> BoxComponent;
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	// 나이아가라 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<class UNiagaraComponent> NiagaraComp;

	// 블루프린트에서 설정할 수 있도록 나이아가라 시스템 에셋을 노출합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> NiagaraSystemAsset; // UNiagaraSystem 포인터

	UPROPERTY()
	TWeakObjectPtr<AActor> Spawner;
};
