#include "GASAIController.h"
#include "GASCharacter.h"

void AGASAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(AGASCharacter* GASCharacter = Cast<AGASCharacter>(InPawn))
    {
        GASCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(GASCharacter, GASCharacter);
        GASCharacter->ManageAbilitiesOnAIPossess();



        TArray<struct FGameplayAbilitySpec*> MatchingGameplayAbilities;
        FGameplayTagContainer GameplayTagContainer;
        GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.ManualControll")));

        GASCharacter->GetAbilitySystemComponent()->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
            GameplayTagContainer, MatchingGameplayAbilities);

        ensure(MatchingGameplayAbilities.Num() <= 1);
        for (auto MatchingGameplayAbility : MatchingGameplayAbilities)
        {
            if (!MatchingGameplayAbility->IsActive())
            {
                if (!GASCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(GameplayTagContainer))
                {
                    UE_LOG(LogTemp, Warning, TEXT("%s AGASAIController::OnPossess TryActivateAbilitiesByTag '%s' for the character '%s' failed"),
                        GASCharacter->HasAuthority() ? *FString("Setver ") : *FString("Client "), *FString("Ability.ManualControll"), *GASCharacter->GetName());
                }
            }
        }
    }
}
