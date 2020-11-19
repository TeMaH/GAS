// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_ManualControl.h"

#include "AbilityTask_InputHandler.h"
#include "../GASCharacter.h"

void UGameplayAbility_ManualControl::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    if (auto GASCharacter = Cast<AGASCharacter>(ActorInfo->AvatarActor.Get()))
    {
        if (IsLocallyControlled())
        {
            UAbilityTask_InputHandler* Task = UAbilityTask_InputHandler::InputHandler(this, FName(), GASCharacter);
            Task->SwitchGASCharacterDelegate.AddDynamic(this, &ThisClass::SwitchGASCharacter);
            Task->ApplyAbilityToCharacterDelegate.AddDynamic(this, &ThisClass::ApplyAbilityToCharacter);

            Task->ReadyForActivation();
        }
    }
}

void UGameplayAbility_ManualControl::SwitchGASCharacter(AGASCharacter* FromAGASCharacter)
{
    ensureMsgf(false, TEXT("Not implemented!"));
}

void UGameplayAbility_ManualControl::ApplyAbilityToCharacter(AGASCharacter* GASCharacter, FGameplayTag TagToApply)
{
    FGameplayTagContainer GameplayTagContainer;
    GameplayTagContainer.AddTag(TagToApply);
    if (!GASCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(GameplayTagContainer))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s UGameplayAbility_ManualControl::ApplyAbilityToCharacter TryActivateAbilitiesByTag '%s' for the character '%s' failed"),
            GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client ") , *TagToApply.ToString(),
            *GASCharacter->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Display,
            TEXT("%s UGameplayAbility_ManualControl::ApplyAbilityToCharacter TryActivateAbilitiesByTag '%s' for the character '%s' OK"),
            GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *TagToApply.ToString(),
            *GASCharacter->GetName());
    }
}