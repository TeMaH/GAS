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

	static void SwitchCharacter(AGASCharacter* AGASCharacter, UWorld* World);
};
