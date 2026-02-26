#include "Core/SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Subsystems/MonsterSubsystem.h"
#include "Core/SpawnMonsterData.h"
#include "Characters/Monster/FMonsterData.h"
#include "WeaponActor.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	SpawnMonsterDataTable = nullptr;
}

void ASpawnVolume::SpawnRandomMonster(int32 StageIndex)
{
	UMonsterSubsystem* MonsterSubsystem = GetWorld()->GetSubsystem<UMonsterSubsystem>();

	if (FSpawnMonsterData* SelectedRow = GetRandomMonster())
	{
		FMonsterData MonsterDataRow = *SelectedRow->MonsterData.GetRow<FMonsterData>(TEXT("MonsterData"));
		float LogScale = FMath::Loge((StageIndex - 1) * 2.0f + 1.0f) + 1.0f;
		MonsterDataRow.StatData.MaxHP *= LogScale;
		MonsterDataRow.StatData.DEF *= LogScale;
		MonsterDataRow.StatData.AttackDamage *= LogScale;
		MonsterSubsystem->SpawnMonster(&MonsterDataRow, GetRandomPointInVolume());
	}
}

FVector ASpawnVolume::GetRandomPointInVolume() const 
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

FSpawnMonsterData* ASpawnVolume::GetRandomMonster() const
{
	if (!SpawnMonsterDataTable) return nullptr;

	TArray<FSpawnMonsterData*> AllRows;
	static const FString ContextString(TEXT("MonsterSpawnContext"));
	SpawnMonsterDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FSpawnMonsterData* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FSpawnMonsterData* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

