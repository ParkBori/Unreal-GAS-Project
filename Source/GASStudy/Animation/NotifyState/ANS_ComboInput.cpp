// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/ANS_ComboInput.h"

#include "AbilitySystemComponent.h"
#include "Interface/GASCanInputInterface.h"
#include "Character/RPGCharacter.h"
// 어빌리티 삭제 -> 재실행 방식으로 결정 

void UANS_ComboInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UE_LOG(LogTemp, Display, TEXT("NotifyBegin"));
    ARPGCharacter* Owner = Cast<ARPGCharacter>(MeshComp->GetOwner());

    if (Owner)
    {
        UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent();
        if (AbilitySystemComponent)
        {
            // ASC 에 등록된 GA 클래스 정보 얻어오기 
            for (const FGameplayAbilitySpec& ActiveAbility : AbilitySystemComponent->GetActivatableAbilities())
            {
                //  Tag 로 얻어진 GA 필터링 
                if (ActiveAbility.Ability->GetAssetTags()/*AbilityTags*/.HasTag(AbilityTag))
                {
                    // 실제 활성화된 GA 인스턴스 얻어오기
                    GAInstances = ActiveAbility.GetAbilityInstances();
                    for (UGameplayAbility* GAInstance : GAInstances)
                    {
                        IGASCanInputInterface* GAInterface =  Cast<IGASCanInputInterface>(GAInstance); 
                        if (GAInterface)
                        {
                            GAInterface->SetCanInput(true);
                        }
                    }
                    break;
                }
            }
        }
    }
}

/*
인스턴스트를 인터페이스로 캐스팅해서 보관하는게 문제가 있었음 
TArray< TObjectPtr<class IGASCanInputInterface>> 이 일단 잘못된 상태고 
IGASCanInputInterface 는 UObject 가 아니여서 가비지 컬렉션의 이점을 못받음 
그런데 TArray 에 UPROPERTY 로 해놨으니 가비지컬렉션이 접근을 하려함 
이부분에서 터짐 
TObject 로 감싸지 않고 TArray 만 들고있어도 같은 문제임 
*/

void UANS_ComboInput::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_ComboInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

    // 이시점에 이미 종료되었을 수 있음 
    ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());

    if (Owner)
    {
        for (UGameplayAbility* GAInstance : GAInstances)
        {
            if (GAInstance->IsActive())
            {
                IGASCanInputInterface* GASInterface = Cast<IGASCanInputInterface>(GAInstance);
                if (GASInterface)
                {
                    GASInterface->SetCanInput(false);
                }
            }
            else
            {
                UE_LOG(LogTemp, Display, TEXT("Ability is not active"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("NotifyEnd Test"));
    }

	UE_LOG(LogTemp, Display, TEXT("NotifyEnd"));
}
