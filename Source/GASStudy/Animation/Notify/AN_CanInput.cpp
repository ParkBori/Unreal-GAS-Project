// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AN_CanInput.h"
#include "AbilitySystemComponent.h"
#include "Interface/GASCanInputInterface.h"
#include "Character/RPGCharacter.h"

void UAN_CanInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 여기서 인터페이스를 만들고 

	// 인터페이스를 상속받는 어빌리티를 만들고 
	// 인터페이스를 상속받는 어빌리티들은 모드 재입력 타이밍이 필요한 어빌리티로 

	// 추가로 생각해볼게
	// 여기서 다시 생각해볼게 입력이 뭔가 끊기는 느낌임 
	// 이럴경우에는 고려해볼만한게 역시 버퍼시스템임 

	// 입력버퍼가 있다면 노티파이 지점에서 끝내버리고 다시 액터를 활성화시키는거임 


	ARPGCharacter* Owner = Cast<ARPGCharacter>(MeshComp->GetOwner());
	if (Owner)
	{
		UAbilitySystemComponent* OwnerASC = Owner->GetAbilitySystemComponent();
		if (OwnerASC)
		{
			TArray<FGameplayAbilitySpec> AbilityContainer = OwnerASC->GetActivatableAbilities();
			for (FGameplayAbilitySpec AbilitySpec : AbilityContainer)
			{
				if (AbilitySpec.Ability->GetAssetTags().HasTag(SearchTag))
				{
					for (UGameplayAbility* AbilityInstance : AbilitySpec.GetAbilityInstances())
					{
						// 애니메이션이 종료될때 SetCanInput(false) 를 호출하여서 현재 구조가 현재까진 괜찮음
						IGASCanInputInterface* InputInterface = Cast<IGASCanInputInterface>(AbilityInstance);
						/*if (InputInterface)
						{
							InputInterface->SetCanInput(true);
							break;
						}*/
						if (InputInterface->GetReStart())
						{
							
							// 종료시키고 다시실행하기 
							AbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityInstance->GetCurrentActorInfo(), AbilitySpec.GetPrimaryInstance()->GetCurrentActivationInfo(), true);
							//
							//// 여기서 바로 실행시키기 
							OwnerASC->TryActivateAbility(AbilitySpec.Handle);
						}
						// 입력버퍼로 만들기
					}
				}
			}
		}
	}

}
