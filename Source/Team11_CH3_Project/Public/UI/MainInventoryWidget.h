// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInventoryWidget.generated.h"

class UItemOverlayWidget;
class UEquipmentDetailWidget;
class UInventoryWidget;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UMainInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Init(int32 InventorySize);
	UFUNCTION(BlueprintCallable)
	void ToggleEquipmentDetailWidget();
	UFUNCTION(BlueprintCallable)
	void HandleItemSlotChanged (const UItemSlot* SlotData, EItemContainerType ItemContainerType, int32 SlotIndex);
	UFUNCTION(BlueprintCallable)
	UItemOverlayWidget* GetItemOverlayWidget(){return ItemOverlayWidget;}
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryWidget> InventoryWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEquipmentDetailWidget> EquipmentDetailWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemOverlayWidget> ItemOverlayWidget;
};
