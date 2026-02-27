// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/ItemContainerType.h"
#include "InventoryWidget.generated.h"

class UGridPanel;
class UUniformGridPanel;
class UEquipmentDetailWidget;
class UItemSlot;
class UItemSlotWidget;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Init(int32 InventorySize);
	UFUNCTION(BlueprintCallable)
	void HandleInventoryItemSlotChanged(const UItemSlot* SlotData, int32 SlotIndex);
	UFUNCTION(BlueprintCallable)
	void HandleEquipmentItemSlotChanged(const UItemSlot* SlotData, EItemContainerType ItemContainerType,
	                                    int32 SlotIndex);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> HeadSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> ChestSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> LegsSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> WeaponSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> HandSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> FeetSlot;
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TArray<TObjectPtr<UItemSlotWidget>> Inventory;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TObjectPtr<UEquipmentDetailWidget> EquipmentDetailWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;
};
