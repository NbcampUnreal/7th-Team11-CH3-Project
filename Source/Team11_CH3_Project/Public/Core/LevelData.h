#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelData.generated.h"

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FLevelData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelNum;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<FStageData> StageData;
};
