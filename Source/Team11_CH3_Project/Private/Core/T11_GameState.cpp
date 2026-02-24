#include "Core/T11_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Core/WaveData.h"
#include "Core/SpawnVolume.h"
#include "Core/Portal.h"
#include "Characters/PlayerCharacter.h"
#include "Core/T11_GameInstance.h"
#include "Components/StatComponent.h"
#include "Components/ItemManager.h"
#include "Components/SkillManager.h"

void AT11_GameState::BeginPlay()
{
    Super::BeginPlay();

    StartLevel();
}

void AT11_GameState::StartLevel()
{
    SpawnedMonsterCount = 0;
    RemainingMonsterCount = 0;
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
        float WaveDuration = CurrentWaveData->WaveDuration;
        int32 MonsterToSpawn;
        for (TPair<FName, int32>& VolumePair : CurrentWaveData->MonsterCount)
        {
            for (AActor* Volume : FoundVolumes)
            {
                if (Volume->ActorHasTag(VolumePair.Key))
                {
                    ASpawnVolume* CurrentSpawnVolume;
                    CurrentSpawnVolume = Cast<ASpawnVolume>(Volume);
                    MonsterToSpawn = VolumePair.Value;
                    SpawnedMonsterCount += MonsterToSpawn;
                    UE_LOG(LogTemp, Warning, TEXT("Spawn Start"));
                    CreateSpawnTimer(VolumePair.Key.ToString(), 1.0f, MonsterToSpawn, CurrentSpawnVolume);
                }
            }
        }

        if (CurrentWaveIndex < MaxWave)
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
    // 플레이어 데이터 저장
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (IsValid(PC) == false || IsValid(PC->GetPawn()) == false)
        return;
    APawn* Pawn = PC->GetPawn();
    UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetGameInstance());
    if (IsValid(GI) == false)
        return;
    GI->SavePlayerData(
        Pawn->FindComponentByClass<UStatComponent>(),
        Pawn->FindComponentByClass<UItemManager>(),
        Pawn->FindComponentByClass<USkillManager>()
    );

    ActivatePortals();
    // 임의로 다음 스테이지로 이동
    //UGameplayStatics::OpenLevel(GetWorld(), FName("L_Stage2_Test"));
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

void AT11_GameState::ActivatePortals()
{
    TArray<AActor*> FoundPortals;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortal::StaticClass(), FoundPortals);

    for (AActor* Portal : FoundPortals)
    {
        APortal* PortalClass = Cast<APortal>(Portal);
        if (PortalClass)
        {
            SetPortalLevel(PortalClass);
            PortalClass->SetPortalActive(true);
        }
    }
}

void AT11_GameState::SetPortalLevel(APortal* Portal)
{
    TArray<FString> LvlArray;
    MapDataConfigs.GenerateKeyArray(LvlArray);

    FString CurrentLvl = UGameplayStatics::GetCurrentLevelName(GetWorld());

    for (int32 i = 0; i < LvlArray.Num(); i++)
    {
        if (LvlArray[i] == CurrentLvl)
        {
            LvlArray.RemoveAtSwap(i);
            break;
        }
    }

    int32 RandomIndex = FMath::RandRange(0, LvlArray.Num() - 1);

    Portal->SetTargetLevel(LvlArray[RandomIndex]);
}

void AT11_GameState::CreateSpawnTimer(FString TimerName, float Interval, int32 TotalCount, ASpawnVolume* SpawnVolume)
{
    FSpawnState& NewState = SpawnStates.FindOrAdd(TimerName);
    NewState.WaveName = TimerName;
    NewState.RemainingCount = TotalCount;
    NewState.TargetVolume = SpawnVolume;

    FTimerHandle& Handle = SpawnTimerHandles.FindOrAdd(TimerName);

    GetWorldTimerManager().SetTimer(Handle, [this, TimerName, SpawnVolume]()
        {
            FSpawnState* State = SpawnStates.Find(TimerName);
            if (State && State->RemainingCount > 0)
            {
                // 실제 소환 로직
                SpawnVolume->SpawnRandomMonster();
                State->RemainingCount--;

                UE_LOG(LogTemp, Log, TEXT("[%s] 남은 마리수: %d"), *State->WaveName, State->RemainingCount);

                if (State->RemainingCount <= 0)
                {
                    GetWorldTimerManager().ClearTimer(SpawnTimerHandles[TimerName]);
                    SpawnTimerHandles.Remove(TimerName);
                    SpawnStates.Remove(TimerName);
                }
            }
        }, Interval, true);
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