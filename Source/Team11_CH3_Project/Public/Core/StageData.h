#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FStageData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MonsterClass;
};
