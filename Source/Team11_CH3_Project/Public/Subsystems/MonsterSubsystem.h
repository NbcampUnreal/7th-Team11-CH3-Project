// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Monster/FMonsterData.h"
#include "Subsystems/WorldSubsystem.h"
#include "MonsterSubsystem.generated.h"

class AMonsterBase;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UMonsterSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	UMonsterSubsystem();
	void SpawnMonster(FMonsterData MonsterData, FVector Location);
	void OnMonsterDeath(AMonsterBase* DeadMonster);
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftClassPtr<AMonsterBase> MonsterClass;
private:
	UPROPERTY()
	TArray<TObjectPtr<AMonsterBase>> Monsters;
	int32 CurrentMonsterCount;
	
};
