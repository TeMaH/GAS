// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GASGameMode.generated.h"

UCLASS(minimalapi)
class AGASGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGASGameMode();

protected:
	void RestartPlayer(AController* NewPlayer) override;

	AActor* FindAIStart(int32 index = 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoints")
	TArray<AActor*> StartPoints;
};



