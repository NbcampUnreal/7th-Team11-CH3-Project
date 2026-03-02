// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/ItemContainerType.h"
#include "InventoryWidget.generated.h"

class UItemManager;
class UInventoryComponent;
class UMainInventoryWidget;
class UGridPanel;
class UUniformGridPanel;
class UEquipmentDetailWidget;
class UItemSlot;
class UInteractableItemSlotWidget;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UMainInventoryWidget* InMainInventoryWidget, UInventoryComponent* InventoryComponent, UItemManager* EquipmentComponent);
	UFUNCTION(BlueprintCallable)
	void HandleInventoryItemSlotChanged(UItemSlot* SlotData);
	UFUNCTION(BlueprintCallable)
	void HandleEquipmentItemSlotChanged(UItemSlot* SlotData);
	void HandleSkillGemItemSlotChanged(UItemSlot* ItemSlot);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> HeadSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> ChestSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> LegsSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> WeaponSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> HandSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> FeetSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> SkillGemSlot0;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractableItemSlotWidget> SkillGemSlot1;
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TArray<TObjectPtr<UInteractableItemSlotWidget>> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TObjectPtr<UEquipmentDetailWidget> EquipmentDetailWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TSubclassOf<UInteractableItemSlotWidget> ItemSlotWidgetClass;
private:
	TWeakObjectPtr<UMainInventoryWidget> MainInventoryWidget;
};
