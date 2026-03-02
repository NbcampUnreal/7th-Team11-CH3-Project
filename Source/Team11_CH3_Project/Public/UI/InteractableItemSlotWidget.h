// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlotWidget.h"
#include "InteractableItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UInteractableItemSlotWidget : public UItemSlotWidget
{
	GENERATED_BODY()
public:
	virtual void Init(UMainInventoryWidget* InMainInventoryWidget, UItemSlot* InSlot) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
private:
	bool bIsDraging = false;
};
