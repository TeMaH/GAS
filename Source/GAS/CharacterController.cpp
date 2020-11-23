// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "GASCharacter.h"
#include "CharacterController.h"
#include "GASAIController.h"

#include "Kismet/GameplayStatics.h"


ACharacterController::ACharacterController(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
}

void ACharacterController::ActivateManualControll(APawn* InPawn)
{
    AGASCharacter* GASCharacter = Cast<AGASCharacter>(InPawn);
    if (IsValid(GASCharacter))
    {

        UE_LOG(LogTemp, Display,
        TEXT("%s ACharacterController::ActivateManualControll for the character '%s'"),
        GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *GASCharacter->GetName());

        // Try to activate ManualControll ability
        TArray<struct FGameplayAbilitySpec*> MatchingGameplayAbilities;
        FGameplayTagContainer GameplayTagContainer;
        GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("ManualControllAbility")));

        GASCharacter->GetAbilitySystemComponent()->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
            GameplayTagContainer, MatchingGameplayAbilities);

        ensure(MatchingGameplayAbilities.Num() <= 1);
        for (auto MatchingGameplayAbility : MatchingGameplayAbilities)
        {
            if (!MatchingGameplayAbility->IsActive())
            {
                if (!GASCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(GameplayTagContainer))
                {
                    UE_LOG(LogTemp, Warning,
                        TEXT("%s ACharacterController::ActivateManualControll TryActivateAbilitiesByTag '%s' for the character '%s' failed"),
                        GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "),
                        *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Display,
                        TEXT("%s ACharacterController::ActivateManualControll TryActivateAbilitiesByTag '%s' for the character '%s' OK"),
                        GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "),
                        *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Display,
                    TEXT(
                        "%s ACharacterController::ActivateManualControll GameplayAbility with tag '%s' is active for '%s'"),
                    GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *FString("Ability.ManualControll"),
                    *GASCharacter->GetName());
            }
        }
    }
}

void ACharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);


    UE_LOG(LogTemp, Display, TEXT("%s ACharacterController::OnPossess before timer '%s' for the character '%s'"),
        InPawn->HasAuthority() ? *FString("Server ") : *FString("Client "), *FString("Ability.ManualControll"),
        *InPawn->GetName());
    
    // We need any delay here before call ActivateManualControll.
    // Is there any way to activate the ability
    // Create Delegate to pass the arguments to ActivateManualControll
    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("ActivateManualControll"), InPawn);
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1, false);

    UE_LOG(LogTemp, Display, TEXT("%s ACharacterController::OnPossess after timer'%s' for the character '%s'"),
        InPawn->HasAuthority() ? *FString("Server ") : *FString("Client "), *FString("Ability.ManualControll"), *InPawn->GetName());
}

void ACharacterController::OnUnPossess()
{
    if (AGASCharacter* GASCharacter = Cast<AGASCharacter>(GetCharacter()))
    {

    }

    Super::OnUnPossess();
}

void ACharacterController::ClientRestart_Implementation(class APawn* NewPawn)
{
    APlayerController::ClientRestart_Implementation(NewPawn);

    AGASCharacter* GASCharacter = Cast<AGASCharacter>(NewPawn);
    if (IsValid(GASCharacter))
    {
        UAbilitySystemComponent* AbilitySystemComponent = GASCharacter->GetAbilitySystemComponent();
        if (IsValid(AbilitySystemComponent))
        {
            AbilitySystemComponent->RefreshAbilityActorInfo();
        }
    }
}

