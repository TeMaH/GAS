// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_InputHandler.generated.h"


class AGASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchGASCharacterDelegate, AGASCharacter*, GASCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FApplyAbilityToCharacterDelegate, AGASCharacter*, GASCharacter, FGameplayTag, TagToApply);

/**
 * 
 */
UCLASS()
class GAS_API UAbilityTask_InputHandler : public UAbilityTask
{
    GENERATED_BODY()

    AGASCharacter* GASCharacter;

public:

    void TickTask(float DeltaTime) override;

    UAbilityTask_InputHandler(const FObjectInitializer& InObjectInitializer);

    static UAbilityTask_InputHandler* InputHandler(
        UGameplayAbility* OwningAbility, FName TaskInstanceName, AGASCharacter* GASCharacter);

    UPROPERTY(BlueprintAssignable)
    FSwitchGASCharacterDelegate SwitchGASCharacterDelegate;
    
    UPROPERTY(BlueprintAssignable)
    FApplyAbilityToCharacterDelegate ApplyAbilityToCharacterDelegate;
    
    UFUNCTION()
    void ActivateAbility1();
    UFUNCTION()
    void ActivateAbility2();
    UFUNCTION()
    void SwitchCharacter();
};
