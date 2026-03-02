// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ItemContainerType.h"
#include "Types/ItemTypes.h"
#include "UObject/Interface.h"
#include "ItemContainer.generated.h"

class UItemInstance;
// This class does not need to be modified.
UINTERFACE()
class UItemContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAM11_CH3_PROJECT_API IItemContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EItemContainerType GetItemContainerType()const=0;
	virtual UItemInstance* GetItem(int32 Index)=0;
	virtual bool SetItemAt(UItemInstance* ItemInstance, int32 Index)=0;
	virtual bool CanReceiveItem(UItemInstance* ItemInstance, int32 TargetIndex)=0;
	virtual bool SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex) = 0;
};
