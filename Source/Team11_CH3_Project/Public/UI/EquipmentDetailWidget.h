// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Items/ItemSlot.h"
#include "EquipmentDetailWidget.generated.h"

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
	void Init();
	void HandlePartsSlotChanged(const UEquipmentSlot* SlotData, int32 Index);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> EquipmentThumbnail;
	
	//TODO HARDCODED
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatName_0;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatAmout_0;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatName_1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatAmout_1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatName_2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatAmout_2;
	
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

};
