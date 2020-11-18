#pragma once

#include "CoreMinimal.h"
#include <AbilitySystemComponent.h>
#include "AttributeSet.h"
#include "GASAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GAS_API UGASAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Base", Replicated)
    FGameplayAttributeData FirstAttr = 0;
    ATTRIBUTE_ACCESSORS(UGASAttributeSet, FirstAttr)

    UPROPERTY(BlueprintReadOnly, Category = "Base", Replicated)
    FGameplayAttributeData SecondAttr = 0;
    ATTRIBUTE_ACCESSORS(UGASAttributeSet, SecondAttr)

    UPROPERTY(BlueprintReadOnly, Category = "Base", Replicated)
    FGameplayAttributeData ThirdAttr = 0;
    ATTRIBUTE_ACCESSORS(UGASAttributeSet, ThirdAttr)
};
