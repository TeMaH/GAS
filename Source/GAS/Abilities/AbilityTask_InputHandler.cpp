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

    return Instance;
}

void UAbilityTask_InputHandler::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

}