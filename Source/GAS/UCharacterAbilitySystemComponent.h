// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "UCharacterAbilitySystemComponent.generated.h"

/**
 *
 */
UCLASS()
class GAS_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void BP_ClientTryActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

    UFUNCTION(Client, Reliable)
    void ClientTryActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);
};
