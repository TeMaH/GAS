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

public:
    ACharacterController(const FObjectInitializer& ObjectInitializer);

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

	UFUNCTION()
	void ActivateManualControll(APawn* InPawn);

public:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> AbilitiesMap;

    virtual void ClientRestart_Implementation(class APawn* NewPawn) override;	
};
