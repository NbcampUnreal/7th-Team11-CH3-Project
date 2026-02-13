// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MonsterSubsystem.h"
#include "Types/StatTypes.h"
#include "WeaponActor.h"
#include "Characters/Monster/MonsterBase.h"

void UMonsterSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
#pragma region TESTCODE
	FMonsterData MonsterData;

	MonsterData.StatData;

	
	MonsterData.AnimBlueprint = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, 
		TEXT("/Game/Characters/Monster/Animations/ABP_Monster.ABP_Monster_C"));

	// 2. 스켈레탈 메시 로드
	MonsterData.SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, 
		TEXT("/Game/KayKit_Fix/KayKit_Skeletons_11_FREE/characters/gltf/Skeleton_Warrior/SkeletalMeshes/Skeleton_Warrior.Skeleton_Warrior")));
	
	MonsterData.WeaponItemData.WeaponActorClass = StaticLoadClass(AWeaponActor::StaticClass(), nullptr, TEXT("/Game/Blueprints/Weapons/TEST_WEAPON.TEST_WEAPON_C"));
	MonsterData.WeaponItemData.StatBonuses.Add(EStat::AttackDamage, 100.0f);
	SpawnMonster(MonsterData, FVector::ZeroVector);
#pragma endregion
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

void UMonsterSubsystem::SpawnMonster(FMonsterData MonsterData, FVector Location)
{
	AMonsterBase* Monster = nullptr;
	if (CurrentMonsterCount < Monsters.Num())
	{
		Monster = Monsters[CurrentMonsterCount];
	}
	else
	{
		Monster = GetWorld()->SpawnActor<AMonsterBase>();
		Monsters.Add(Monster);
	}
	if (Monster)
	{
		Monster->SetActorLocation(Location);
		Monster->SetActorHiddenInGame(false);
		Monster->SetActorEnableCollision(true);
		Monster->SetActorTickEnabled(true);

		Monster->Init(MonsterData);

		CurrentMonsterCount++;
	}
}

void UMonsterSubsystem::OnMonsterDeath(AMonsterBase* DeadMonster)
{
	int32 Index = Monsters.Find(DeadMonster);
	if (Index != INDEX_NONE && Index < CurrentMonsterCount)
	{
		CurrentMonsterCount--;
		Monsters.Swap(Index, CurrentMonsterCount);
		DeadMonster->SetActorHiddenInGame(true);
		DeadMonster->SetActorEnableCollision(false);
		DeadMonster->SetActorTickEnabled(false);
	}
}
