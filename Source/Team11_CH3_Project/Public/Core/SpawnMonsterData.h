#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/StatComponent.h"
#include "SpawnMonsterData.generated.h"

USTRUCT(BlueprintType)
struct TEAM11_CH3_PROJECT_API FSpawnMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* MonsterSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatData MonsterStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MonsterWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterWeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
