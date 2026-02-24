#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UIMapDataAsset.generated.h"

class UCommonActivatableWidget;

USTRUCT(BlueprintType)
struct FUIWidgetPolicy
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Policy")
    TSoftClassPtr<UCommonActivatableWidget> WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Policy", meta = (Categories = "UI.Layer"))
    FGameplayTag TargetLayer;
};

UCLASS(BlueprintType)
class TEAM11_CH3_PROJECT_API UUIMapDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "UI Policy", meta = (Categories = "UI.Request"))
    TMap<FGameplayTag, FUIWidgetPolicy> WidgetMap;
};