// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "UObject/Object.h"
#include "EquipmentInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UEquipmentInstance : public UItemInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> GemSlots;
	
};
