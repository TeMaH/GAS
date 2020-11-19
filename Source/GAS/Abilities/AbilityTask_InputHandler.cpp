// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_InputHandler.h"
#include "../GASCharacter.h"

UAbilityTask_InputHandler::UAbilityTask_InputHandler(const FObjectInitializer& InObjectInitializer)
    : Super(InObjectInitializer)
{
    bTickingTask = true;
    bSimulatedTask = true;
}

UAbilityTask_InputHandler* UAbilityTask_InputHandler::InputHandler(
    UGameplayAbility* OwningAbility, FName TaskInstanceName, AGASCharacter* GASCharacter)
{
    auto* Instance = NewAbilityTask<UAbilityTask_InputHandler>(OwningAbility, TaskInstanceName);
    Instance->GASCharacter = GASCharacter;


    // I want to save time and nerves, so I will not proceed input in tick, but jus bind input to the corresponded methods
    ensure(IsValid(GASCharacter->InputComponent));

    GASCharacter->InputComponent->BindAction(
        "ActivateAbility1", IE_Pressed, Instance, &UAbilityTask_InputHandler::ActivateAbility1);
    GASCharacter->InputComponent->BindAction(
        "ActivateAbility2", IE_Pressed, Instance, &UAbilityTask_InputHandler::ActivateAbility2);
    GASCharacter->InputComponent->BindAction("SwitchCharacter", IE_Pressed, Instance, &UAbilityTask_InputHandler::SwitchCharacter);


    return Instance;
}

void UAbilityTask_InputHandler::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);
    
    //GASCharacter->InputComponent->
    
}

void UAbilityTask_InputHandler::ActivateAbility1()
{
    ApplyAbilityToCharacterDelegate.Broadcast(GASCharacter, FGameplayTag::RequestGameplayTag(TEXT("AbilityAbility.GA1")));
}

void UAbilityTask_InputHandler::ActivateAbility2()
{
    ApplyAbilityToCharacterDelegate.Broadcast(GASCharacter, FGameplayTag::RequestGameplayTag(TEXT("AbilityAbility.GA2")));
}

void UAbilityTask_InputHandler::SwitchCharacter()
{
    SwitchGASCharacterDelegate.Broadcast(GASCharacter);
}