#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/ItemContainerType.h"
#include "InventoryComponent.generated.h"


class UItemDataAsset;
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
	bool AddItem(UItemDataAsset* ItemDataAsset, int32 Count);
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
