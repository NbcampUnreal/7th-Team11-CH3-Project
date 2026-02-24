#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Blueprintable, Abstract)
class TEAM11_CH3_PROJECT_API UPrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerTag);

protected:
	UFUNCTION(BlueprintCallable, Category = "Layer", meta = (Categories = "UI.Layer"))
	void RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);

	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;

	virtual void NativeConstruct() override;
	
};
