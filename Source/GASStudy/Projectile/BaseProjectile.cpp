// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }
    
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    BoxComponent->SetupAttachment(RootComponent);
    //BoxComponent->SetCollisionProfileName()


    // 이 컴포넌트를 사용하여 이 발사체의 이동을 주도합니다.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    //ProjectileMovementComponent->bRotationFollowsVelocity = true;
    //rojectileMovementComponent->bShouldBounce = true;
    //ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagarComponent"));
    NiagaraComp->SetupAttachment(BoxComponent); // 루트 컴포넌트에 부착하거나 다른 컴포넌트에 부착
    NiagaraComp->bAutoActivate = true; // 액터 스폰 시 자동으로 활성화 (이펙트 재생)
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
    //BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::BoxOverlapCallback);
    BoxComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::BoxHitCallback);
    NiagaraComp->bAutoActivate = true; // 액터 스폰 시 자동으로 활성화 (이펙트 재생)

}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //E_LOG(LogTemp, Warning, TEXT("InitSpeed : %f   /   Velocity : %s"), ProjectileMovementComponent->InitialSpeed, *ProjectileMovementComponent->Velocity.ToString());
}

void ABaseProjectile::FireInDirection(AActor* ProjectileOwner)
{
    Spawner = ProjectileOwner;
    FVector test = Spawner->GetActorForwardVector();
    ProjectileMovementComponent->Velocity = test * ProjectileMovementComponent->InitialSpeed;
}

void ABaseProjectile::BoxOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *OtherActor->GetName());
}

void ABaseProjectile::BoxHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *OtherActor->GetName());

    BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NiagaraComp->Deactivate();

    FTimerHandle DestroyTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, 
        [&]() 
        {
            Destroy();
        }
    , 3.0f, false);
}

