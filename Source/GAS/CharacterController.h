// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:

	ACharacterController(const FObjectInitializer& ObjectInitializer);

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
	
};
