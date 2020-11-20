// Fill out your copyright notice in the Description page of Project Settings.


#include "UCharacterAbilitySystemComponent.h"


UCharacterAbilitySystemComponent::UCharacterAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UCharacterAbilitySystemComponent::BP_ClientTryActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
    ClientTryActivateAbilityByClass(AbilityClass);
}

void UCharacterAbilitySystemComponent::ClientTryActivateAbilityByClass_Implementation(TSubclassOf<UGameplayAbility> AbilityClass)
{
    UE_LOG(LogTemp, Warning, TEXT("ClientTryActivateAbilityByClass [%s - %s]"), *GetNameSafe(GetOwner()), *GetNameSafe(AvatarActor));
    const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
    ENetRole NetMode = ROLE_SimulatedProxy; 
    if (APlayerController* PC = ActorInfo->PlayerController.Get())
    {
        NetMode = PC->GetLocalRole();
        UE_LOG(LogTemp, Log, TEXT("PC [%s] - %s"), *GetNameSafe(PC), *UEnum::GetValueAsString<ENetRole>(NetMode));
    }
    else if (ActorInfo->AvatarActor.IsValid())
    {
        NetMode = ActorInfo->AvatarActor->GetLocalRole();
        UE_LOG(LogTemp, Log, TEXT("ActorInfo->AvatarActor [%s] - %s"), *GetNameSafe(ActorInfo->AvatarActor.Get()), *UEnum::GetValueAsString<ENetRole>(NetMode));
    }
    TryActivateAbilityByClass(AbilityClass, true);
}

