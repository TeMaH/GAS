// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_ManualControl.generated.h"

class AGASCharacter;
/**
 * 
 */
UCLASS()
class GAS_API UGameplayAbility_ManualControl : public UGameplayAbility
{
	GENERATED_BODY()
public:
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    
    UFUNCTION()
    void SwitchGASCharacter(AGASCharacter* FromAGASCharacter);
    UFUNCTION()
    void ApplyAbilityToCharacter(AGASCharacter* GASCharacter, FGameplayTag TagToApply);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> AbilitiesMap;
};
