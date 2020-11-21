// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelector.h"
#include "GASCharacter.h"
#include "CharacterController.h"
#include "GASAIController.h"

#include "Kismet/GameplayStatics.h"

CharacterSelector::CharacterSelector()
{
}

CharacterSelector::~CharacterSelector()
{
}

void CharacterSelector::SwitchCharacter(AGASCharacter* GASCharacter, UWorld* World)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AGASCharacter::StaticClass(), FoundActors);

    // Select Character to switch to
    for (auto Actor : FoundActors)
    {
        AGASCharacter* TempCharacter = Cast<AGASCharacter>(Actor);

        UE_LOG(LogTemp, Display, TEXT("%s CharacterSelector::SwitchCharacter character '%s'"),
        GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), 
        *TempCharacter->GetName());

        if (TempCharacter == GASCharacter)
        {
            continue;
        }

        AController* Controller = (TempCharacter->GetController());
        ACharacterController* TCharacterController = Cast<ACharacterController>(Controller);

        if (AGASAIController* CharacterController = Cast<AGASAIController>(TempCharacter->GetController()))
        {
            UE_LOG(LogTemp, Display, TEXT(
                    "%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s'"),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), 
                *GASCharacter->GetName(), *TempCharacter->GetName());

            
            const auto FromController = GASCharacter->GetController();
            const auto ToController = TempCharacter->GetController();

            if (!FromController || !ToController)
            {
                UE_LOG(LogTemp, Error, TEXT("%s CharacterSelector::SwitchCharacter One of controllers is null"),
                    GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "));

                return;
            }

            FromController->Possess(TempCharacter);
            ToController->Possess(GASCharacter);

            UE_LOG(LogTemp, Display, TEXT("%s CharacterSelector::SwitchCharacter Controllers switched"),
            GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "));

            break;
        }

        
    }
}