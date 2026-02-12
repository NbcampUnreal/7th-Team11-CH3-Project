// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FItemDataBase : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	TSoftObjectPtr<UTexture2D> Thumbnail;
	
	
};
