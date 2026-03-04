// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/BossMonster.h"

#include "WeaponActor.h"
#include "Components/SkillComponent.h"
#include "NavigationSystem.h"
#include "Components/EquipmentComponent.h"

// Sets default values
ABossMonster::ABossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeleeRange = 200.0f;
	MiddleRange = 10000.0f;
}

void ABossMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool ABossMonster::TryAttack(AActor* Target)
{
	
	if (!Target || SkillComponent->IsSkillActive())
	{
		return false;
	}

	int32 BestSkillIdx = SkillComponent->GetBestSkill(this,Target);
	if (SkillComponent->GetCooldownRemaining(BestSkillIdx) > 0.0f)
	{
		return false;
	}

	USkillSlot* SkillSlot = SkillComponent->GetSkillSlot(BestSkillIdx);
	FVector TargetLocation = Target->GetActorLocation();

	PerformSkill(SkillSlot, Target);

	return true;
}

void ABossMonster::Init(const FMonsterData* MonsterData)
{
	Super::Init(MonsterData);
	SetActorScale3D({2.f, 2.f, 2.f});
}


// Called to bind functionality to input
void ABossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

