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


void CharacterSelector::SwapControllers(AGASCharacter* FromCharacter, AGASCharacter* ToCharacter, UWorld* World)
{
    const auto FromController = FromCharacter->GetController();
    const auto ToController = ToCharacter->GetController();

    if (!FromController || !ToController)
    {
        UE_LOG(LogTemp, Error, TEXT("%s (FromCharacter) CharacterSelector::SwitchCharacter One of controllers is null"),
            FromCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "));

        return;
    }

    FromController->Possess(ToCharacter);
    ToController->Possess(FromCharacter);

    UE_LOG(LogTemp, Display, TEXT("%s (FromCharacter) %s (ToCharacter) CharacterSelector::SwitchCharacter Controllers switched"),
        FromCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "),
        ToCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "));
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

        if (AGASAIController* GASAIController = Cast<AGASAIController>(TempCharacter->GetController()))
        {
            UE_LOG(LogTemp, Display, TEXT(
                    "%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s' (AGASAIController)"),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), 
                *GASCharacter->GetName(), *TempCharacter->GetName());

            SwapControllers(GASCharacter, TempCharacter, GASCharacter->GetWorld());

            break;
        }
        else if (ACharacterController* CharacterController = Cast<ACharacterController>(TempCharacter->GetController()))
        {
            UE_LOG(LogTemp, Display, TEXT("%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s' (ACharacterController)"),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *GASCharacter->GetName(),
                *TempCharacter->GetName());

            SwapControllers(GASCharacter, TempCharacter, GASCharacter->GetWorld());

            break;
        }
        else
        {
            UE_LOG(LogTemp, Display,
                TEXT("%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s' "),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *GASCharacter->GetName(),
                *TempCharacter->GetName());

            SwapControllers(GASCharacter, TempCharacter, GASCharacter->GetWorld());

            break;
        }

        
    }
}