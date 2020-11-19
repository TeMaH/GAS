// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "GASCharacter.h"

void ACharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (AGASCharacter* GASCharacter = Cast<AGASCharacter>(GetCharacter()))
    {
        GASCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(this, GASCharacter);
        GASCharacter->ManageAbilitiesOnPossess();
    }
}

void ACharacterController::OnUnPossess()
{
    if (AGASCharacter* GASCharacter = Cast<AGASCharacter>(GetCharacter()))
    {
        
    }

    Super::OnUnPossess();
}