// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/ItemContainerType.h"
#include "InventoryWidget.generated.h"

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
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	void HandleItemSlotChanged (const UItemSlot* SlotData, EItemContainerType ItemContainerType, int32 SlotIndex);
	UFUNCTION(BlueprintCallable)
	void ToggleEquipmentDetailWidget();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> EquipmentGrid;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TArray<TObjectPtr<UItemSlotWidget>> Equipments;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TArray<TObjectPtr<UItemSlotWidget>> Inventory;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TObjectPtr<UEquipmentDetailWidget> EquipmentDetailWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryWidget")
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;
};
