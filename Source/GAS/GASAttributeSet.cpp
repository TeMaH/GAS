#include "GASAttributeSet.h"

#include <Net/UnrealNetwork.h>
#include <UObject/UnrealType.h>

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, FirstAttr, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, SecondAttr, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, ThirdAttr, COND_None, REPNOTIFY_Always);
}
