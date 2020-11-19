// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "GASAIController.generated.h"

/**
 *
 */
UCLASS()
class GAS_API AGASAIController : public AAIController
{
    GENERATED_BODY()

        void OnPossess(APawn* InPawn) override;
};
