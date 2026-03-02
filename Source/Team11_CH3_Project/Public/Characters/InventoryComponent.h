#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "Components/ActorComponent.h"
#include "Types/ItemContainerType.h"
#include "InventoryComponent.generated.h"


class UItemDataAsset;
class UItemSlot;
class UEquipmentSlot;
class UItemInstance;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM11_CH3_PROJECT_API UInventoryComponent : public UActorComponent, public IItemContainer
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	virtual void InitializeComponent() override;
	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemDataAsset* ItemDataAsset, int32 Count = 1);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 Index, int32 Amount);


	TArray<TObjectPtr<UItemSlot>>& GetInventorySlots() { return InventorySlots; }

	UPROPERTY(BlueprintAssignable)
	FOnItemSlotChanged OnInventorySlotChanged;

	virtual EItemContainerType GetItemContainerType() const override;
	virtual UItemInstance* GetItem(int32 TargetIndex) override;
	virtual bool SetItemAt(UItemInstance* ItemInstance, int32 Index) override;
	virtual bool CanReceiveItem(UItemInstance* ItemInstance, int32 TargetIndex) override;
	virtual bool SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<TObjectPtr<UItemSlot>> InventorySlots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	EItemContainerType ContainerType = EItemContainerType::Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 InventorySize = 20;
};
