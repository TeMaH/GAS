// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_InputHandler.h"
#include "../GASCharacter.h"
#include "DrawDebugHelpers.h"

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

    // I want to save time and nerves, so I will not proceed input in tick, but jus bind input actions to the corresponded methods
    Instance->GASCharacter->ApplyAbilityToCharacterDelegate.AddDynamic(Instance, &UAbilityTask_InputHandler::ActivateAbility);
    Instance->GASCharacter->SwitchGASCharacterDelegate.AddDynamic(Instance, &UAbilityTask_InputHandler::SwitchCharacter);

    return Instance;
}

void UAbilityTask_InputHandler::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    if (IsValid(GASCharacter))
    {
        if (GASCharacter->HasAuthority())
        {
            FString Name = GASCharacter->GetName();
        }
        else
        {
            FString Name = GASCharacter->GetName();
        }
    }
}

void UAbilityTask_InputHandler::ActivateAbility(AGASCharacter* InGASCharacter, FGameplayTag TagToApply)
{
    ApplyAbilityToCharacterDelegate.Broadcast(InGASCharacter, TagToApply);
}

void UAbilityTask_InputHandler::SwitchCharacter(AGASCharacter* InGASCharacter)
{
    SwitchGASCharacterDelegate.Broadcast(InGASCharacter);
}

void UAbilityTask_InputHandler::OnDestroy(bool bInOwnerFinished)
{
    Super::OnDestroy(bInOwnerFinished);
}
void UAbilityTask_InputHandler::BeginDestroy()
{
    Super::BeginDestroy();
}