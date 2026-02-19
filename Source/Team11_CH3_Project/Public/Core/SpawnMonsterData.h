#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SpawnMonsterData.generated.h"

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FSpawnMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle MonsterData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
