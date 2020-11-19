// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Abilities/GameplayAbility.h"

#include "CharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

public:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> AbilitiesMap;

    virtual void ClientRestart_Implementation(class APawn* NewPawn) override;
	
};
