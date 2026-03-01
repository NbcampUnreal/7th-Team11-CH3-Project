// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/StatTypes.h"
#include "StatRowWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UStatRowWidget : public UUserWidget
{
	GENERATED_BODY()
	void Update(EStat Stat, float Amount);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatAmount;
};
