// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MonsterSubsystem.h"
#include "Types/StatTypes.h"
#include "WeaponActor.h"
#include "Core/T11_GameState.h"
#include "Characters/Monster/MonsterBase.h"

void UMonsterSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	// #pragma region TESTCODE
	// 	FTimerHandle SpawnedMonsterTimer;
	// 	GetWorld()->GetTimerManager().SetTimer(SpawnedMonsterTimer,[this]()
	// 	{
	// 		FMonsterData MonsterData;
	//
	// 		MonsterData.StatData;
	//
	// 		
	// 		MonsterData.AnimBlueprint = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, 
	// 			TEXT("/Game/Characters/Monster/Animations/ABP_Monster.ABP_Monster_C"));
	//
	// 		// 2. 스켈레탈 메시 로드
	// 		MonsterData.SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, 
	// 			TEXT("/Game/KayKit_Fix/KayKit_Skeletons_11_FREE/characters/gltf/Skeleton_Warrior/SkeletalMeshes/Skeleton_Warrior.Skeleton_Warrior")));
	// 		
	// 		MonsterData.WeaponItemData.WeaponActorClass = StaticLoadClass(AWeaponActor::StaticClass(), nullptr, TEXT("/Game/Blueprints/Weapons/BP_StaffWeaponActor.BP_StaffWeaponActor_C"));
	// 		MonsterData.WeaponItemData.StatBonuses.Emplace(EStat::AttackDamage,100.0f);
	// 		MonsterData.WeaponItemData.WeaponType = EWeaponType::Melee;
	// 		MonsterData.StatData.MaxHP = 100.0f;
	// 		MonsterData.StatData.MoveSpeed = 600.0f;
	// 		SpawnMonster(MonsterData, {100,100,100});
	// 	},3,true
	// 	);
	// #pragma endregion
}

void UMonsterSubsystem::Deinitialize()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

UMonsterSubsystem::UMonsterSubsystem()
{
	ConstructorHelpers::FClassFinder<AMonsterBase> MonsterClassFinder(
		TEXT("/Game/Characters/Monster/BP_MonsterBase.BP_MonsterBase_C"));
	if (MonsterClassFinder.Succeeded())
	{
		MonsterClass = MonsterClassFinder.Class;
	}
}

void UMonsterSubsystem::SpawnMonster(FMonsterData* MonsterData, const FVector& Location)
{
	AMonsterBase* Monster = nullptr;
	if (SpawnedMonsterCount < Monsters.Num())
	{
		Monster = Monsters[SpawnedMonsterCount];
	}
	else
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Monster = GetWorld()->SpawnActor<AMonsterBase>(MonsterClass.LoadSynchronous(), SpawnParameters);
		Monsters.Add(Monster);
	}
	if (Monster)
	{
		Monster->SetActorLocation(Location);
		Monster->SetActorHiddenInGame(false);
		Monster->SetActorEnableCollision(true);
		Monster->SetActorTickEnabled(true);

		Monster->Init(MonsterData);
		Monster->SetActorEnableCollision(true);

		SpawnedMonsterCount++;
		AliveMonsterCount++;
	}
}

void UMonsterSubsystem::OnMonsterDeath(AMonsterBase* DeadMonster)
{
	AliveMonsterCount--;
	DeadMonster->SetActorEnableCollision(false);
	FTimerHandle SpawnedMonsterTimer;
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindUFunction(this, FName("DespawnMonster"), DeadMonster);
	GetWorld()->GetTimerManager().SetTimer(
		SpawnedMonsterTimer,
		TimerDelegate,
		5.0f,
		false
	);
	DeadMonster->SetActorEnableCollision(false);
	if (AT11_GameState* GameState = GetWorld()->GetGameState<AT11_GameState>())
	{
		// 아이템 드랍을 위해서 몬스터 위치 매개변수(완료) 및 점수 데이터 추가(예정)
		GameState->OnMonsterKilled(DeadMonster->GetActorLocation(), DeadMonster->GetScoreValue());
	}
}

void UMonsterSubsystem::DespawnMonster(AMonsterBase* DeadMonster)
{
	if (!IsValid(this)) return;

	int32 Index = Monsters.Find(DeadMonster);
	if (Index != INDEX_NONE && Index < SpawnedMonsterCount)
	{
		SpawnedMonsterCount--;
		Monsters.Swap(Index, SpawnedMonsterCount);
		DeadMonster->Clear();
		DeadMonster->SetActorHiddenInGame(true);
		DeadMonster->SetActorEnableCollision(false);
		DeadMonster->SetActorTickEnabled(false);
	}
}
