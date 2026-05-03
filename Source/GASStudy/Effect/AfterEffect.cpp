// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/AfterEffect.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
AAfterEffect::AAfterEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMeshComp"));


	
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Script/Engine.Material'/Game/Material/M_AtferEffect.M_AtferEffect'"));
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Script/Engine.Material'/Game/Characters/UEFN_Mannequin/Materials/M_UEFN_Mannequin_AffterEffect.M_UEFN_Mannequin_AffterEffect'"));
	//if (MaterialAsset.Succeeded())
	//{
	//	MeshMaterial = MaterialAsset.Object;
	//}
	
}

// Called when the game starts or when spawned
void AAfterEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAfterEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FadeOutWeight -= DeltaTime / FadeOutSpeed;

	if (FadeOutWeight <= 0)
	{
		FadeOutWeight = 0;
		Destroy();
	}

	else
	{
		for (auto& Material : DynamicMaterials)
		{
			Material->SetScalarParameterValue(FName("Opacitiy"), FadeOutWeight);
		}
	}
}

void AAfterEffect::Init(USkeletalMeshComponent* CopyMesh)
{
	//Mesh->SetSkeletalMesh(CopyMesh->GetSkeletalMeshAsset());
	Mesh->SetSkinnedAssetAndUpdate(CopyMesh->GetSkeletalMeshAsset());

	// Dests Component 의 Pose 정보를 복제한 Mesh 를 만들어낸다.
	Mesh->CopyPoseFromSkeletalComponent(CopyMesh);


	// 몬스터의 Material 을 DynamiceMaterialInstance 로 생성해준다.
	int32 Count = Mesh->GetNumMaterials();

	for (int32 i = 0; i < Count; ++i)
	{
		auto* Mtrl = Mesh->CreateDynamicMaterialInstance(i/*, MeshMaterial*/);

		Mtrl->BlendMode = EBlendMode::BLEND_Translucent;
		DynamicMaterials.Add(Mtrl);
	}
}


