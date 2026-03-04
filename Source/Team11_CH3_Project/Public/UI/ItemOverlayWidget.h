// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/StatTypes.h"
#include "ItemOverlayWidget.generated.h"

class URarityColorDataAsset;
class UImage;
class UEquipmentInstance;
class UStatRowWidget;
class UItemInstance;
class UMainInventoryWidget;
class UHorizontalBox;
class UItemSlotWidget;
class UVerticalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(UMainInventoryWidget* InMainInventoryWidget);
	void UpdateOverlayWidget(FVector2D ScreenPosition, UItemInstance* ItemInstance);
	void ClearOverlayWidget();
private:
	void UpdateStatBox(UEquipmentInstance* EquipmentInstance);
	void UpdateSocketBox(UEquipmentInstance* EquipmentInstance);
	void CollapseStatBox();
	void CollapseSocketBox();
	void UpdateStatBoxFromStats(const TMap<EStat, float>& Stats);
	void UpdateItemRarity(UItemInstance* EquipmentInstance);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemType;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DESC_Text;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatBox;
	TArray<TObjectPtr<UStatRowWidget>> StatRows;
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> PartsBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SocketBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TArray<TObjectPtr<UItemSlotWidget>> Sockets;
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemRateBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemTypeImage;
	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TSubclassOf<UStatRowWidget> StatRowWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InventoryWidget")
	TObjectPtr<URarityColorDataAsset> RarityColorDataAsset;


private:
	TWeakObjectPtr<UMainInventoryWidget> MainInventoryWidget;
};
