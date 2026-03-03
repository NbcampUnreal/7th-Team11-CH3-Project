// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/StatTypes.h"
#include "StatDetailWidget.generated.h"

class UStatRowWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UStatDetailWidget : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryWidget")
	TSubclassOf<UStatRowWidget> StatRowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatBox;
	TArray<TObjectPtr<UStatRowWidget>> StatRows;

public:
	void UpdateStatBox(TMap<EStat, float> Stats);
};
