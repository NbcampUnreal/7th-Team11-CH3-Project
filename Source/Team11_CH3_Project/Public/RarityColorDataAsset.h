// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ItemTypes.h"
#include "UObject/Object.h"
#include "RarityColorDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class  TEAM11_CH3_PROJECT_API URarityColorDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	TMap<EItemRarity, FSlateColor>& GetRarityColorMap(){return RarityColorMap;}
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Monster")
	TMap<EItemRarity, FSlateColor> RarityColorMap;
	
};
