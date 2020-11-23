// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGameMode.h"
#include "GASCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "AIController.h"
#include "GASAIController.h"
#include "CharacterSelector.h"

AGASGameMode::AGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

     CharacterSelector = NewObject<UCharacterSelector>(this, UCharacterSelector::ComponentName);
}

void AGASGameMode::RestartPlayer(AController* NewPlayer)
{
	AActor* StartSpot = FindPlayerStart(NewPlayer);

    if (StartSpot == nullptr)
    {
        if (NewPlayer->StartSpot != nullptr)
        {
            StartSpot = NewPlayer->StartSpot.Get();
            UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
        }
    }

    FRotator SpawnRotation = StartSpot->GetActorRotation();

    if (NewPlayer->GetPawn() != nullptr)
    {
        SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
    }
    else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.Instigator = GetInstigator();
        UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
        APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, StartSpot->GetTransform(), SpawnInfo);
        NewPlayer->SetPawn(ResultPawn);
    }

    NewPlayer->Possess(NewPlayer->GetPawn());
    NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

    FRotator NewControllerRot = SpawnRotation;
    NewControllerRot.Roll = 0.f;
    NewPlayer->SetControlRotation(NewControllerRot);

    AGASAIController* FirstAIController = GetWorld()->SpawnActor<AGASAIController>();
    AActor* AIStartSpot = FindPlayerStart(FirstAIController);
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = GetInstigator();
    APawn* AIPawn = GetWorld()->SpawnActor<APawn>(GetDefaultPawnClassForController(NewPlayer), AIStartSpot->GetTransform(), SpawnInfo);
    FirstAIController->SetPawn(AIPawn);
    FirstAIController->Possess(AIPawn);
}

AActor* AGASGameMode::FindAIStart(int32 InIndex /*= 0*/)
{
    if(InIndex >= StartPoints.Num())
    {
        UE_LOG(LogGameMode, Error, TEXT("FindAIStart i = %d Num = %d"), InIndex, StartPoints.Num());
        return nullptr;
    }
    return StartPoints[InIndex];
}
