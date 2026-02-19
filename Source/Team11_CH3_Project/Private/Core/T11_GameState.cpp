#include "Core/T11_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Core/WaveData.h"
#include "Core/SpawnVolume.h"

void AT11_GameState::BeginPlay()
{
    Super::BeginPlay();

    StartLevel();
}

void AT11_GameState::StartLevel()
{
    SpawnedMonsterCount = 0;
    CurrentWaveIndex = 1;
    TSoftObjectPtr<UWorld> CurrentWorld = GetWorld();
    UE_LOG(LogTemp, Warning, TEXT("Map Name: %s"), *CurrentWorld->GetName());
    if (CurrentWorld && MapDataConfigs.Contains(CurrentWorld->GetName()))
    {
        WaveData = MapDataConfigs[CurrentWorld->GetName()];
        
        
        if (!WaveData)
        {
            return;
        }
        MaxWave = WaveData->GetRowMap().Num();
        UE_LOG(LogTemp, Warning, TEXT("Level Start"));
        StartNextWave();
    }
    
}

void AT11_GameState::StartNextWave()
{
    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
    
    FWaveData* CurrentWaveData;
    static const FString ContextString(TEXT("Wave Data Reading"));
    CurrentWaveData = WaveData->FindRow<FWaveData>(FName(*FString::Printf(TEXT("Wave_%d"), CurrentWaveIndex)), ContextString);
    UE_LOG(LogTemp, Warning, TEXT("Wave Start"));
    if (CurrentWaveData->WaveNum == CurrentWaveIndex)
    {
        ASpawnVolume* SpawnVolume;
        float WaveDuration = CurrentWaveData->WaveDuration;
        int32 MonsterToSpawn;
        for (TPair<FName, int32>& VolumePair : CurrentWaveData->MonsterCount)
        {
            for (AActor* Volume : FoundVolumes)
            {
                if (Volume->ActorHasTag(VolumePair.Key))
                {

                    SpawnVolume = Cast<ASpawnVolume>(Volume);
                    MonsterToSpawn = VolumePair.Value;
                    SpawnedMonsterCount += MonsterToSpawn;
            
                    for (int32 i = 0; i < MonsterToSpawn; i++)
                    {
                        SpawnVolume->SpawnRandomMonster();
                    }
                }
            }
        }

        if (CurrentWaveIndex <= MaxWave)
        {
            GetWorldTimerManager().SetTimer(
                WaveTimerHandle,
                this,
                &AT11_GameState::EndWave,
                WaveDuration,
                false
            );
        }
    }
}

void AT11_GameState::EndLevel()
{
    // 임의로 다음 스테이지로 이동
    UGameplayStatics::OpenLevel(GetWorld(), FName("L_TestStage_2"));
}

void AT11_GameState::EndWave()
{
    GetWorldTimerManager().ClearTimer(WaveTimerHandle);
    UE_LOG(LogTemp, Warning, TEXT("Wave end"));
    if (CurrentWaveIndex >= MaxWave)
    {
        EndLevel();
    }
    else
    {
        CurrentWaveIndex++;
        StartNextWave();
    }
}

void AT11_GameState::OnMonsterKilled()
{
    SpawnedMonsterCount--;
    if (SpawnedMonsterCount == 0)
    {
        EndWave();
    }
}

/*

스테이지 시작

웨이브 시작
웨이브 데이터에 있는 첫번째 웨이브 행
각 웨이브 스폰 볼륨의 몬스터
몬스터 수
다음 웨이브까지의 시간

웨이브의 모든 몬스터 처치 or 시간 종료

다음 웨이브 시작

모든 웨이브 끝나면 포탈 생성

*/