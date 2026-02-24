#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"

#include "UI/DataAssets/UIMapDataAsset.h"
#include "UIManageSubsystem.generated.h"

class UPrimaryLayout;
class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;

UCLASS(BlueprintType)
class TEAM11_CH3_PROJECT_API UUIManageSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "UI Management")
    UCommonActivatableWidget* ShowWidget(UPARAM(meta = (Categories = "UI.Request")) FGameplayTag RequestTag);

    UFUNCTION(BlueprintPure, Category = "UI Management")
    bool IsLayerBusy(FGameplayTag RequestTag) const;

    UFUNCTION(BlueprintCallable, Category = "UI Management")
    void SetRootLayout(UPrimaryLayout* InRootLayout);

protected:
    void EnsureRootLayout();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSoftClassPtr<UPrimaryLayout> RootLayoutClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI Policy")
    TObjectPtr<UUIMapDataAsset> UIMapDataAsset;

    UPROPERTY(Transient)
    TMap<FGameplayTag, FUIWidgetPolicy> CachedWidgetMap;

    UPROPERTY(Transient)
    TObjectPtr<UPrimaryLayout> RootLayout;
};