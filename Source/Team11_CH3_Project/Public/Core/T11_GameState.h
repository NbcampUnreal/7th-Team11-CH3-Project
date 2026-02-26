#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "T11_GameState.generated.h"

class ASpawnVolume;
class APortal;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelStart, int32, MaxWaveCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaveStart, int32, CurrentWaveCount, int32, MaxWaveCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonsterSpawn, int32, CurrentMonsterCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonsterKilled, int32, CurrentMonsterCount);

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

	void ActivatePortals();
	void SetPortalLevel(APortal* Portal);

	void CreateSpawnTimer(FString TimerName, float Interval, int32 TotalCount, ASpawnVolume* SpawnVolume);
	// 아이템 드랍 위치 매개변수 추가
	void OnMonsterKilled(FVector DropLocation, int32 ScoreValue);

	FTimerHandle WaveTimerHandle;
	TMap<FString, FTimerHandle> SpawnTimerHandles;
	TMap<FString, FSpawnState> SpawnStates;

	int CurrentWaveIndex;
	int MaxWave;
	int SpawnedMonsterCount;

	UPROPERTY(EditAnywhere)
	TMap<FString, UDataTable*> MapDataConfigs;

	TSoftObjectPtr<UDataTable> WaveData;

	UPROPERTY(BlueprintAssignable)
	FLevelStart LevelStarted;
	UPROPERTY(BlueprintAssignable)
	FWaveStart WaveStarted;
	UPROPERTY(BlueprintAssignable)
	FMonsterSpawn MonsterSpawned;
	UPROPERTY(BlueprintAssignable)
	FMonsterKilled MonsterKilled;

	void LvlStartedBroadCast();
	void WaveStartedBroadCast();
};
