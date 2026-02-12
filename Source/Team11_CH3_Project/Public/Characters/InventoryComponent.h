#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FSkillSlotData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	FSkillSlotData GetSkillSlot(int32 Index);
	void UseConsumable(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool HasWeaponEquipped() const;
};
