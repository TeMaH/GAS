// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "GASCharacter.h"

void ACharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (AGASCharacter* GASCharacter = Cast<AGASCharacter>(InPawn))
    {
        GASCharacter->ManageAbilitiesOnPossess();

        GASCharacter->GetAbilitySystemComponent()->RefreshAbilityActorInfo();


        // Try to activate ManualControll ability
        TArray<struct FGameplayAbilitySpec*> MatchingGameplayAbilities;
        FGameplayTagContainer GameplayTagContainer;
        GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.ManualControll")));

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
                        TEXT("%s ACharacterController::OnPossess TryActivateAbilitiesByTag '%s' for the character '%s' failed"),
                        GASCharacter->HasAuthority() ? *FString("Setver ") : *FString("Client "),
                        *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Display,
                        TEXT("%s ACharacterController::OnPossess TryActivateAbilitiesByTag '%s' for the character '%s' OK"),
                        GASCharacter->HasAuthority() ? *FString("Setver ") : *FString("Client "),
                        *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
            }
        }
    }
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