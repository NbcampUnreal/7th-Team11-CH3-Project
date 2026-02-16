#include "Core/T11_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SpawnVolume.h"

void AT11_GameState::BeginPlay()
{
    Super::BeginPlay();

    StartLevel();
}

void AT11_GameState::StartLevel()
{
    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

    int32 MonsterToSpawn = 2;
    for (int32 i = 0; i < MonsterToSpawn; i++)
    {
        if (FoundVolumes.Num() > 0)
        {
            ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
            if (SpawnVolume)
            {
                SpawnVolume->SpawnRandomMonster();
            }
        }
    }
}

void AT11_GameState::StartNextWave()
{
}
