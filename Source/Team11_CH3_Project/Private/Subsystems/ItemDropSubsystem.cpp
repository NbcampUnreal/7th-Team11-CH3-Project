// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ItemDropSubsystem.h"
#include "Components/Items/DropItemData.h"
#include "Components/Items/PickupActor.h"
#include "Core/T11_GameInstance.h"

void UItemDropSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Game Start -> 난이도 선택 + 조작법 UI 선택 -> 시작
	// 아직 레벨완성이 안돼서 테스트용스테이지 1 2 쉬움만
	DropTables.Add({ 0, 0 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_DropItem")));
	//DropTables.Add({ 0, 1 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	DropTables.Add({ 1, 0 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/DT_DropItem")));
	//DropTables.Add({ 1, 1 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	//DropTables.Add({ 2, 0 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	//DropTables.Add({ 2, 1 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	//DropTables.Add({ 3, 0 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	//DropTables.Add({ 3, 1 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	//DropTables.Add({ 4, 0 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
	//DropTables.Add({ 4, 1 }, LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DT_Drop_Stage1_Normal")));
}

void UItemDropSubsystem::TryDropItem(FVector DropLocation)
{
	UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GI) == false)
		return;

	UDataTable* DropTable = DropTables.FindRef({ GI->CurrentStageIndex, GI->CurrentDifficulty });
	if (IsValid(DropTable) == false)
		return;

	TArray<FDropItemData*> AllRows;
	static const FString Context(TEXT("DropTable"));
	DropTable->GetAllRows(Context, AllRows);
	if (AllRows.IsEmpty())
		return;

	float TotalChance = 0.f;
	for (FDropItemData* Row : AllRows)
	{
		TotalChance += Row->DropChance;
	}

	float Rand = FMath::FRandRange(0.f, TotalChance);
	float Accumulated = 0.f;
	FDropItemData* SelectedItem = nullptr;
	for (FDropItemData* Row : AllRows)
	{
		Accumulated += Row->DropChance;
		if (Rand <= Accumulated)
		{
			SelectedItem = Row;
			break;
		}
	}

	if (SelectedItem == nullptr)
		return;
	if (SelectedItem->PickupActorClass.IsNull())
		return;

	APickupActor* Pickup = GetWorld()->SpawnActor<APickupActor>(
		SelectedItem->PickupActorClass.LoadSynchronous(),
		DropLocation,
		FRotator::ZeroRotator
	);

	if (IsValid(Pickup) == false)
		return;

	Pickup->ItemType = SelectedItem->ItemType;
	Pickup->ItemID = SelectedItem->ItemID;
	Pickup->Thumbnail = SelectedItem->Thumbnail;

	UE_LOG(LogTemp, Log, TEXT("아이템 드랍"));
}