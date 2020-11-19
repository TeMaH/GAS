#include "GASAIController.h"
#include "GASCharacter.h"

void AGASAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(AGASCharacter* GASCharacter = Cast<AGASCharacter>(InPawn))
    {
        GASCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(GASCharacter, GASCharacter);
        GASCharacter->ManageAbilitiesOnAIPossess();
    }
}
