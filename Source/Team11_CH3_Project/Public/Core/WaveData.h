// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/SpawnVolume.h"
#include "WaveData.generated.h"


USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FWaveData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> MonsterCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDuration;
};