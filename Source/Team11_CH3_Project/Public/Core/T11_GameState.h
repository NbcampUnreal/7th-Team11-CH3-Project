#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "T11_GameState.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AT11_GameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	// 스테이지 시작
	void StartLevel();
	// 웨이브 시작
	void StartNextWave();

	void EndLevel();
	void EndWave();

	void OnMonsterKilled();

	FTimerHandle WaveTimerHandle;

	int CurrentWaveIndex;
	int MaxWave;
	int SpawnedMonsterCount;

	UPROPERTY(EditAnywhere)
	TMap<FString, TSoftObjectPtr<UDataTable>> MapDataConfigs;

	TSoftObjectPtr<UDataTable> WaveData;
};
