// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/ItemContainerType.h"
#include "Characters/InventoryComponent.h"
#include "Components/ItemManager.h"
#include "Input/Reply.h"
#include "MainInventoryWidget.generated.h"

class UItemSlotWidget;
class UItemManager;
class UInventoryComponent;
class UEquipmentInstance;
class UEquipmentSlot;
class UItemSlot;
class UItemOverlayWidget;
class UEquipmentDetailWidget;
class UInventoryWidget;
class UStatDetailWidget;
class UStatComponent;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UMainInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(int32 InventorySize, UInventoryComponent* InInventoryComponent, UItemManager* InEquipmentComponent, UStatComponent* InStatComponent);

	UFUNCTION(BlueprintCallable)
	void UpdateEquipmentDetailWidget(UItemSlot* EquipmentSlot);

	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	UFUNCTION(BlueprintCallable)
	void HandleItemSlotChanged(UItemSlot* SlotData);

	UFUNCTION(BlueprintCallable)
	void HandleStatChanged(UStatComponent* StatComp);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable)
	UItemOverlayWidget* GetItemOverlayWidget() { return ItemOverlayWidget; }

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() { return InventoryComponent.Get(); }

	UFUNCTION(BlueprintCallable)
	UItemManager* GetEquipmentComponent() { return EquipmentComponent.Get(); }

	const TSubclassOf<UItemSlotWidget>& GetDragItemSLotWidgetClass() const { return DragItemSlotWidgetClass; }

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryWidget> InventoryWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEquipmentDetailWidget> EquipmentDetailWidget;

	UPROPERTY(VisibleAnywhere, Category="ItemContainer")
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	UPROPERTY(VisibleAnywhere, Category="ItemContainer")
	TWeakObjectPtr<UItemManager> EquipmentComponent;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemOverlayWidget> ItemOverlayWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UStatDetailWidget> StatDetailWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemSlotWidget> DragItemSlotWidgetClass;
};
