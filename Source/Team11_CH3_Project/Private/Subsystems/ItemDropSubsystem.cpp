// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ItemDropSubsystem.h"
#include "Components/Items/DropTableTypes.h"
#include "Components/Items/PickupActor.h"
#include "Components/Items/ItemDataAsset.h"
#include "Core/T11_GameInstance.h"

void UItemDropSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 마스터 DataTable 로드
	MasterDropTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_MasterDropTable"));
	if (IsValid(MasterDropTable) == false)
		return;

	static const FString Context(TEXT("MasterDropTable"));
	TArray<FDropTableRow*> AllRows;
	MasterDropTable->GetAllRows(Context, AllRows);

	for (FDropTableRow* Row : AllRows)
	{
		if (Row == nullptr || IsValid(Row->DropTable) == false)
			continue;

		CachedDropTables.Add({ Row->StageIndex, Row->Difficulty }, Row->DropTable);
	}
}

void UItemDropSubsystem::TryDropItem(FVector DropLocation)
{
	UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GI) == false)
		return;
	// 현재 스테이지 + 난이도에 맞는 서브 DT 조회
	UDataTable* DropTable = CachedDropTables.FindRef({ GI->CurrentStageIndex, GI->CurrentDifficulty });
	if (IsValid(DropTable) == false)
		return;
	// 서브 DT 전체 행 가져오기
	static const FString Context(TEXT("DropTable")); 
	TArray<FDropItemRow*> AllRows;
	DropTable->GetAllRows(Context, AllRows);
	if (AllRows.IsEmpty())
		return;
	// 가중치 합산
	float TotalChance = 0.f;
	for (FDropItemRow* Row : AllRows)
	{
		TotalChance += Row->DropChance;
	}
	// 랜덤 선택
	float Rand = FMath::FRandRange(0.f, TotalChance);
	float Accumulated = 0.f;
	FDropItemRow* SelectedRow = nullptr;
	for (FDropItemRow* Row : AllRows)
	{
		Accumulated += Row->DropChance;
		if (Rand <= Accumulated)
		{
			SelectedRow = Row;
			break;
		}
	}
	
	if (SelectedRow == nullptr)
		return;
	if (SelectedRow->ItemDataAsset.IsNull())
		return;

	// Soft 참조 로드
	UItemDataAsset* ItemAsset = SelectedRow->ItemDataAsset.LoadSynchronous();
	if (IsValid(ItemAsset) == false)
		return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APickupActor* Pickup = GetWorld()->SpawnActor<APickupActor>(
		APickupActor::StaticClass(),
		DropLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);
	
	if (IsValid(Pickup) == false)
		return;
	
	Pickup->Init(ItemAsset, 1);

	UE_LOG(LogTemp, Log, TEXT("아이템 드랍"));
}