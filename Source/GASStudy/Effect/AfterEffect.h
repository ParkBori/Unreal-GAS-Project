// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AfterEffect.generated.h"

UCLASS()
class GASSTUDY_API AAfterEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAfterEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Init(USkeletalMeshComponent* Mesh);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPoseableMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInterface> MeshMaterial;

	float FadeOutWeight = 1.0f;
	const float FadeOutSpeed = 3.f;


	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;
};
