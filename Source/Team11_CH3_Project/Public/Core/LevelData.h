#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelData.generated.h"

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MonsterClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterSpawnCount;
};

USTRUCT(BlueprintType)
class TEAM11_CH3_PROJECT_API FLevelData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FWaveData> WaveData;
};
