// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "GASCharacter.h"

ACharacterController::ACharacterController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bAutoManageActiveCameraTarget = false;
}

void ACharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void ACharacterController::OnUnPossess()
{
    if (AGASCharacter* GASCharacter = Cast<AGASCharacter>(GetCharacter()))
    {
        if (auto GASCharacterAbilitySystemComponent = GASCharacter->GetAbilitySystemComponent())
        {
        }
    }

    Super::OnUnPossess();
}