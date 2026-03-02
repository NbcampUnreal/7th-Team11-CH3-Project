// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Items/ItemSlot.h"
#include "EquipmentDetailWidget.generated.h"

class UStatRowWidget;
class UMainInventoryWidget;
class UVerticalBox;
class UInteractableItemSlotWidget;
class UHorizontalBox;
class UScrollBox;
class UTextBlock;
class UItemSlotWidget;
class UImage;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentDetailWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(UMainInventoryWidget* InMainInventoryWidget);
	void Update(UItemSlot* ItemSlot);
	void HandlePartsSlotChanged(UItemSlot* SlotData);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> EquipmentThumbnail;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatBox;
	TArray<TObjectPtr<UStatRowWidget>> StatRows;
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DESC_Title;	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DESC_Text;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> GemBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TArray<TObjectPtr<UInteractableItemSlotWidget>> GemSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TSubclassOf<UInteractableItemSlotWidget> ItemSlotWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TSubclassOf<UStatRowWidget> StatRowWidgetClass;
private:
	void UpdateStatBox(const TMap<EStat, float>& Stats);
	TWeakObjectPtr<UMainInventoryWidget> MainInventoryWidget;
	TWeakObjectPtr<UEquipmentInstance> CurrentEquipment;
};
