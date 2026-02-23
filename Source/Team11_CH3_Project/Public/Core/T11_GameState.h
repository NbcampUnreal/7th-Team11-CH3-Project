#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "T11_GameState.generated.h"

class ASpawnVolume;

UCLASS()
class TEAM11_CH3_PROJECT_API AT11_GameState : public AGameState
{
	GENERATED_BODY()

	struct FSpawnState
	{
		FString WaveName;      // 타이머 식별 이름
		ASpawnVolume* TargetVolume;
		int32 RemainingCount;  // 남은 소환 마리수
	};

public:

	virtual void BeginPlay() override;

	// 스테이지 시작
	void StartLevel();
	// 웨이브 시작
	void StartNextWave();

	void EndLevel();
	void EndWave();

	void CreateSpawnTimer(FString TimerName, float Interval, int32 TotalCount, ASpawnVolume* SpawnVolume);
	// 아이템 드랍 위치 매개변수 추가
	void OnMonsterKilled(FVector DropLocation, int32 ScoreValue);

	FTimerHandle WaveTimerHandle;
	TMap<FString, FTimerHandle> SpawnTimerHandles;
	TMap<FString, FSpawnState> SpawnStates;

	int CurrentWaveIndex;
	int MaxWave;
	int SpawnedMonsterCount;
	int RemainingMonsterCount;

	UPROPERTY(EditAnywhere)
	TMap<FString, UDataTable*> MapDataConfigs;

	TSoftObjectPtr<UDataTable> WaveData;
};
