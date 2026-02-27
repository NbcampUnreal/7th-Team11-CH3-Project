#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Items/Equipments/EquipmentItemData.h"
#include "Components/Items/Equipments/ItemInstance.h"
#include "Types/ItemContainerType.h"
#include "InventoryComponent.generated.h"


class UItemSlot;
class UEquipmentSlot;
class UItemInstance;





UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM11_CH3_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	virtual void InitializeComponent() override;
	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemInstance* ItemInstance, int32 Amount = 1);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 Index, int32 Amount);
	UPROPERTY(BlueprintAssignable)
	FOnItemSlotChanged OnInventorySlotChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<TObjectPtr<UItemSlot>> InventorySlots;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TMap<FName,int32> Indexes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 InventorySize = 20;
};
