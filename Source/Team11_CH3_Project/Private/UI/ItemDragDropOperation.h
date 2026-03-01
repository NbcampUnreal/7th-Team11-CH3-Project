// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Types/ItemContainerType.h"
#include "ItemDragDropOperation.generated.h"

class UItemInstance;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	EItemContainerType ItemContainerType;
	int32 Index;
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;
};
