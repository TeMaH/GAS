// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "GASCharacter.h"
#include "CharacterController.h"
#include "GASAIController.h"

#include "Kismet/GameplayStatics.h"


ACharacterController::ACharacterController(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
}

void ACharacterController::ActivateManualControll(APawn* InPawn)
{
    AGASCharacter* GASCharacter = Cast<AGASCharacter>(InPawn);
    if (IsValid(GASCharacter))
    {
        // Try to activate ManualControll ability
        TArray<struct FGameplayAbilitySpec*> MatchingGameplayAbilities;
        FGameplayTagContainer GameplayTagContainer;
        GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("ManualControllAbility")));

        GASCharacter->GetAbilitySystemComponent()->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
            GameplayTagContainer, MatchingGameplayAbilities);

        ensure(MatchingGameplayAbilities.Num() <= 1);
        for (auto MatchingGameplayAbility : MatchingGameplayAbilities)
        {
            if (!MatchingGameplayAbility->IsActive())
            {
                if (!GASCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(GameplayTagContainer))
                {
                    UE_LOG(LogTemp, Warning,
                        TEXT("%s ACharacterController::ActivateManualControll TryActivateAbilitiesByTag '%s' for the character '%s' failed"),
                        GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "),
                        *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Display,
                        TEXT("%s ACharacterController::ActivateManualControll TryActivateAbilitiesByTag '%s' for the character '%s' OK"),
                        GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "),
                        *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
            }
        }
    }
}

void ACharacterController::EmptyMethod()
{
    // This method does nothing
}

void ACharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);


    UE_LOG(LogTemp, Display, TEXT("%s ACharacterController::OnPossess before timer '%s' for the character '%s'"),
        InPawn->HasAuthority() ? *FString("Server ") : *FString("Client "), *FString("Ability.ManualControll"),
        *InPawn->GetName());
    
    // We need any delay here before call ActivateManualControll.
    // Create Delegate to pass the arguments to ActivateManualControll
    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("ActivateManualControll"), InPawn);
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1, false);

    UE_LOG(LogTemp, Display, TEXT("%s ACharacterController::OnPossess after timer'%s' for the character '%s'"),
        InPawn->HasAuthority() ? *FString("Server ") : *FString("Client "), *FString("Ability.ManualControll"), *InPawn->GetName());
}

void ACharacterController::OnUnPossess()
{
    if (AGASCharacter* GASCharacter = Cast<AGASCharacter>(GetCharacter()))
    {

    }

    Super::OnUnPossess();
}

void ACharacterController::ClientRestart_Implementation(class APawn* NewPawn)
{
    APlayerController::ClientRestart_Implementation(NewPawn);

    AGASCharacter* GASCharacter = Cast<AGASCharacter>(NewPawn);
    if (IsValid(GASCharacter))
    {
        UAbilitySystemComponent* AbilitySystemComponent = GASCharacter->GetAbilitySystemComponent();
        if (IsValid(AbilitySystemComponent))
        {
            AbilitySystemComponent->RefreshAbilityActorInfo();
        }
    }
}

void ACharacterController::SwapControllers_Implementation(AGASCharacter* FromCharacter, AGASCharacter* ToCharacter)
{
    const auto FromController = FromCharacter->GetController();
    const auto ToController = ToCharacter->GetController();

    if (!FromController || !ToController)
    {
        UE_LOG(LogTemp, Error, TEXT("%s (FromCharacter) CharacterSelector::SwitchCharacter One of controllers is null"),
            FromCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "));

        return;
    }

    FromController->Possess(ToCharacter);
    ToController->Possess(FromCharacter);

    UE_LOG(LogTemp, Display, TEXT("%s (FromCharacter) %s (ToCharacter) CharacterSelector::SwitchCharacter Controllers switched"),
        FromCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "),
        ToCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "));
}

void ACharacterController::SwitchCharacter_Implementation(AGASCharacter* GASCharacter)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGASCharacter::StaticClass(), FoundActors);

    // Select Character to switch to
    for (auto Actor : FoundActors)
    {
        AGASCharacter* TempCharacter = Cast<AGASCharacter>(Actor);

        UE_LOG(LogTemp, Display, TEXT("%s CharacterSelector::SwitchCharacter character '%s'"),
            GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *TempCharacter->GetName());

        if (TempCharacter == GASCharacter)
        {
            continue;
        }

        AController* Controller = (TempCharacter->GetController());
        ACharacterController* TCharacterController = Cast<ACharacterController>(Controller);

        if (AGASAIController* GASAIController = Cast<AGASAIController>(TempCharacter->GetController()))
        {
            UE_LOG(LogTemp, Display,
                TEXT("%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s' (AGASAIController)"),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *GASCharacter->GetName(),
                *TempCharacter->GetName());

            SwapControllers(GASCharacter, TempCharacter);

            break;
        }
        else if (ACharacterController* CharacterController = Cast<ACharacterController>(TempCharacter->GetController()))
        {
            UE_LOG(LogTemp, Display,
                TEXT("%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s' (ACharacterController)"),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *GASCharacter->GetName(),
                *TempCharacter->GetName());

            SwapControllers(GASCharacter, TempCharacter);

            break;
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("%s CharacterSelector::SwitchCharacter Switch controller from '%s' to '%s' "),
                GASCharacter->HasAuthority() ? *FString("Server ") : *FString("Client "), *GASCharacter->GetName(),
                *TempCharacter->GetName());

            SwapControllers(GASCharacter, TempCharacter);

            break;
        }
    }
}