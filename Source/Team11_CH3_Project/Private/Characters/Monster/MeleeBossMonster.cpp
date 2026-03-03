// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MeleeBossMonster.h"

#include "WeaponActor.h"
#include "Components/SkillManager.h"
#include "NavigationSystem.h"
#include "Components/ItemManager.h"

// Sets default values
AMeleeBossMonster::AMeleeBossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeleeRange = 200.0f;
	MiddleRange = 10000.0f;
}

void AMeleeBossMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool AMeleeBossMonster::TryAttack(AActor* Target)
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

	PerformSkill(SkillSlot, TargetLocation);

	return true;
}


// Called to bind functionality to input
void AMeleeBossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

