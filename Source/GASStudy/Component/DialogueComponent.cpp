// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/DialogueComponent.h"
#include "Blueprint/UserWidget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Subsystem/DialogueSubsystem.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
