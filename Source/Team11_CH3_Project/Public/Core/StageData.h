#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FStageData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> StageLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNum;
};
