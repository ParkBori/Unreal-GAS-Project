// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Shared/Interact/InteractInfo.h"
#include "InteractableActor.generated.h"

/**
* 인터렉트 및 워들와의 상호작용
*/
UCLASS(Abstract)
class GASSTUDY_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	/**
	* 에디터에서 인터렉션 타입을 지정변경하면 반영하는 함수
	* 월드에 배치시킬 아이템을 편하게 관리하기 위한 함수
	*/
	virtual void OnConstruction(const FTransform& Transform) override;


	/**
	* 인터페이스를 상속받는 상호작용함수
	*/
	void ToggleOutline_Implementation(bool bActivate);

	UFUNCTION(BlueprintCallable)
	void SetStencilValue();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Item Settings")
	TObjectPtr<class UBoxComponent> mCollision;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings", meta = (AllowPrivateAccess = true, Tooltip = "타입에 따른 인터렉트시 아웃라인 색상 변경"))
	EInteractCustomStencil mInteractCustomStencil;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "DefualtSceneRoot"))
	TObjectPtr<USceneComponent> mDefualtSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Settings")
	TObjectPtr<UStaticMeshComponent> mStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StencilMesh")
	TArray<TObjectPtr<UStaticMeshComponent>> mApplyStencilMesh;

	/**
	* 더이상 상호작용 하지 않는 경우 해당 변수 활성화
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Setting", meta = (AllowPrivateAccess = true, Tooltip = "인터렉트를 더 이상 못하게 막고싶을때 사용하는 변수"))
	bool bCanInteract = true;
};
