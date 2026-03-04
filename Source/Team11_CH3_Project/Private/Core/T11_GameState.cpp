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
#include "Subsystems/ItemDropSubsystem.h"
#include "MainPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Characters/InventoryComponent.h"

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

    UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetGameInstance());
    if (IsValid(GI) == false) return;
    CurrentStageIndex = GI->CurrentStageIndex;
    FString MapName;
    if (GI->CurrentDifficulty == 0) MapName = CurrentWorld->GetName() + "_Easy";
    else if (GI->CurrentDifficulty == 1) MapName = CurrentWorld->GetName() + "_Hard";
    else if (GI->CurrentDifficulty == 2) MapName = CurrentWorld->GetName();
    if (CurrentStageIndex < GI->MaxStageCount && CurrentStageIndex != GI->MidBossStage)
    {
        if (CurrentWorld && MapDataConfigs.Contains(MapName))
        {
            WaveData = MapDataConfigs[MapName];
        
            if (!WaveData)
            {
                return;
            }
            MaxWave = WaveData->GetRowMap().Num();
            UE_LOG(LogTemp, Warning, TEXT("Level Start"));

            FTimerHandle TempHandle;
            GetWorldTimerManager().SetTimer(TempHandle, this, &AT11_GameState::LvlStartedBroadCast, 0.2f, false);
       
            StartNextWave();
        }
    }
    else
    {
        if (CurrentWorld && BossMapDataConfig.Contains(MapName))
        {
            WaveData = BossMapDataConfig[MapName];

            if (!WaveData)
            {
                return;
            }
            MaxWave = WaveData->GetRowMap().Num();
            UE_LOG(LogTemp, Warning, TEXT("Level Start"));

            FTimerHandle TempHandle;
            GetWorldTimerManager().SetTimer(TempHandle, this, &AT11_GameState::LvlStartedBroadCast, 0.2f, false);

            StartNextWave();
        }
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
    FTimerHandle TempHandle;
    GetWorldTimerManager().SetTimer(TempHandle, this, &AT11_GameState::WaveStartedBroadCast, 0.2f, false);

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

    ActivatePortals();
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
            UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetGameInstance());
            if (IsValid(GI) == false) return;

            if (GI->CurrentStageIndex + 1 < GI->MaxStageCount && CurrentStageIndex + 1 != GI->MidBossStage && PortalClass->Difficulty != "Boss")
            {
                SetPortalLevel(PortalClass);
                PortalClass->SetPortalActive(true);
            }
            else if (CurrentStageIndex + 1 == GI->MidBossStage && PortalClass->Difficulty == "Boss")
            {
                TArray<FString> Keys;
                BossMapDataConfig.GetKeys(Keys);
                if (Keys.IsValidIndex(0))
                {
                    FString FirstKey = Keys[0];
                    PortalClass->SetTargetLevel(FirstKey);
                    PortalClass->SetPortalActive(true);
                }
            }
            else if (GI->CurrentStageIndex + 1 >= GI->MaxStageCount && PortalClass->Difficulty == "Boss")
            {
                TArray<FString> Keys;
                BossMapDataConfig.GetKeys(Keys);
                if (Keys.IsValidIndex(0))
                {
                    FString SecondKey = Keys[1];
                    PortalClass->SetTargetLevel(SecondKey);
                    PortalClass->SetPortalActive(true);
                }
            }
            else if (GI->CurrentStageIndex + 1 >= GI->MaxStageCount && PortalClass->Difficulty == "Clear")
            {
                PortalClass->SetTargetLevel("L_GameClear");
                PortalClass->SetPortalActive(true);
            }
        }
    }
}

void AT11_GameState::SetPortalLevel(APortal* Portal)
{
    TSet<FString> UniqueLvl;

    for (auto& Map : MapDataConfigs)
    {
        UniqueLvl.Add(Map.Key.LeftChop(5));
    }
    TArray<FString> LvlArray = UniqueLvl.Array();

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

void AT11_GameState::UsePortal(FString Difficulty, FString TargetLevel)
{
    UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetGameInstance());
    if (IsValid(GI) == false) return;

    if (Difficulty == "Easy") GI->CurrentDifficulty = 0;
    else if (Difficulty == "Hard") GI->CurrentDifficulty = 1;
    else if (Difficulty == "Boss") GI->CurrentDifficulty = 2;
    // 여기서 한번 더 데이터 갱신
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (IsValid(PC) == false || IsValid(PC->GetPawn()) == false)
        return;
    APawn* Pawn = PC->GetPawn();
    GI->SavePlayerData(
        Pawn->FindComponentByClass<UStatComponent>(),
        Pawn->FindComponentByClass<UItemManager>(),
        Pawn->FindComponentByClass<USkillManager>(),
        Pawn->FindComponentByClass<UInventoryComponent>()
    );

    LevelFinished.Broadcast(TargetLevel);
}

void AT11_GameState::CreateSpawnTimer(FString TimerName, float Interval, int32 TotalCount, ASpawnVolume* SpawnVolume)
{
    FSpawnState& NewState = SpawnStates.FindOrAdd(TimerName);
    NewState.WaveName = TimerName;
    NewState.RemainingCount = TotalCount;
    NewState.TargetVolume = SpawnVolume;

    FTimerDelegate SpawnDelegate;
    SpawnDelegate.BindUObject(this, &AT11_GameState::ExecuteSpawn, TimerName, SpawnVolume);

    FTimerHandle& Handle = SpawnTimerHandles.FindOrAdd(TimerName);
    GetWorldTimerManager().SetTimer(Handle, SpawnDelegate, Interval, true);
}

void AT11_GameState::ExecuteSpawn(FString TimerName, ASpawnVolume* SpawnVolume)
{
    FSpawnState* State = SpawnStates.Find(TimerName);
    if (State && State->RemainingCount > 0)
    {
        // 실제 소환 로직
        SpawnVolume->SpawnRandomMonster(CurrentStageIndex);
        State->RemainingCount--;
        SpawnedMonsterCount++;
        MonsterSpawned.Broadcast(SpawnedMonsterCount);

        UE_LOG(LogTemp, Log, TEXT("[%s] 남은 마리수: %d"), *State->WaveName, State->RemainingCount);

        if (State->RemainingCount <= 0)
        {
            if (FTimerHandle* HandlePtr = SpawnTimerHandles.Find(TimerName))
            {
                GetWorldTimerManager().ClearTimer(*HandlePtr);
            }
            SpawnTimerHandles.Remove(TimerName);
            SpawnStates.Remove(TimerName);
        }
    }
}

void AT11_GameState::OnMonsterKilled(FVector DropLocation, int32 ScoreValue)
{
    SpawnedMonsterCount--;
    MonsterKilled.Broadcast(SpawnedMonsterCount);
    // 점수 처리
    UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetGameInstance());
    if (IsValid(GI) == false)
        return;

    int32& StageIndex = GI->CurrentStageIndex;
    int32& TotalScore = GI->TotalScore;
    int32& StageScore = GI->StageScore.FindOrAdd(StageIndex);
    

    StageScore += ScoreValue;
    TotalScore += ScoreValue;
    UE_LOG(LogTemp, Warning, TEXT("StageScore : %d , TotalScore : %d"), StageScore, TotalScore);
    
    // 아이템 드랍
    if (UItemDropSubsystem* ItemDrop = GetWorld()->GetSubsystem<UItemDropSubsystem>())
    {
        ItemDrop->TryDropItem(DropLocation);
    }
    
    if (SpawnedMonsterCount == 0)
    {
        EndWave();
    }
}

void AT11_GameState::LvlStartedBroadCast()
{
    LevelStarted.Broadcast(MaxWave);
}

void AT11_GameState::WaveStartedBroadCast()
{
    WaveStarted.Broadcast(CurrentWaveIndex, MaxWave);
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