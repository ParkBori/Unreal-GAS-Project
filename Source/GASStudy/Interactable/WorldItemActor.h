// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "WorldItemActor.generated.h"

/**
 * 월드에 배치되는 아이템 액터들
 * 나이아가라로 표현
 */
UCLASS()
class GASSTUDY_API AWorldItemActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	
protected:
	/**
	* FName 이 바뀌면, 물체의 외형이 바뀌게 된다.
	* 월드에 배치시킬 아이템을 편하게 관리하기 위한 함수
	*/
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

public:
	/**
	* 인터페이스를 상속받는 상호작용함수
	*/
	void Interact_Implementation(AActor* Actor);

private:
	UPROPERTY(EditAnywhere, Category = "Item|Mesh")
	bool bUpdateMeshByID;

	UPROPERTY(EditAnywhere, Category = "Item|Mesh")
	FName mItemID;

	UPROPERTY(EditAnywhere, Category = "Item|Effect")
	TObjectPtr<UNiagaraSystem> mNiagara;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> mNiagaraComp;
};
