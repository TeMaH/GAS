// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AGASCharacter;

/**
 * 
 */
class GAS_API CharacterSelector
{
public:
	CharacterSelector();
	~CharacterSelector();

	UFUNCTION(Server, Reliable /*, WithValidation*/)
	static void SwitchCharacter(AGASCharacter* AGASCharacter, UWorld* World);

	UFUNCTION(Server, Reliable /*, WithValidation*/)
    static void SwapControllers(AGASCharacter* FromCharacter, AGASCharacter* ToCharacter, UWorld* World);
};
