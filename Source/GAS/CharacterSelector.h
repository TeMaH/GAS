// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterSelector.generated.h"

class AGASCharacter;

/**
 * 
 */
UCLASS()
class GAS_API UCharacterSelector : public UActorComponent
{
    GENERATED_BODY()

public:
    static FName ComponentName;

    void SwitchCharacter(AGASCharacter* AGASCharacter);

    void SwapControllers(AGASCharacter* FromCharacter, AGASCharacter* ToCharacter);
};
