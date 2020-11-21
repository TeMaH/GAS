// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_ManualControl.h"

#include "AbilityTask_InputHandler.h"

#include "../GASGameMode.h"

#include "../GASCharacter.h"
#include "../CharacterController.h"
#include "../CharacterSelector.h"



#include "Kismet/GameplayStatics.h"

void UGameplayAbility_ManualControl::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{

    {
        FString Msg = FString::Printf(TEXT("UGameplayAbility_ManualControl::ActivateAbility"));

        if (auto GASCharacter = Cast<AGASCharacter>(ActorInfo->AvatarActor.Get()))
        {
            Msg.Append(" ").Append(GASCharacter->GetName());
            Msg.Append(GASCharacter->HasAuthority() ? FString("Server ") : FString("Client "));
        }

        UE_LOG(LogTemp, Display, TEXT("%s "),*Msg);
    }

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
    //ensureMsgf(false, TEXT("Not implemented yet!"));
    //CharacterSelector::SwitchCharacter(FromAGASCharacter, FromAGASCharacter->GetWorld());

    AController* Controller = FromAGASCharacter->GetController();
    ACharacterController* CharacterController = Cast<ACharacterController>(Controller);
    if (IsValid(CharacterController))
    {
        CharacterController->SwitchCharacter(FromAGASCharacter);
    }
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