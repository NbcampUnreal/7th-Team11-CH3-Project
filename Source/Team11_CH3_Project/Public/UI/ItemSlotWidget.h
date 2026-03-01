// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UItemInstance;
class UMainInventoryWidget;
class UItemSlot;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(UMainInventoryWidget* InMainInventoryWidget);
	void UpdateSlot(const UItemSlot* InSlot);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
protected:

	void Clear();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Count;
private:
	TWeakObjectPtr<UMainInventoryWidget> MainInventoryWidget;
	TWeakObjectPtr<UItemInstance> ItemInstance;
};
