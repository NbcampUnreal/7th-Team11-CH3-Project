// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterBase.h"
#include "Characters/Monster/MonsterControllerBase.h"
#include "Components/StatComponent.h"


// Sets default values
AMonsterBase::AMonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AIControllerClass = AMonsterControllerBase::StaticClass();
	SetRootComponent(RootComponent);
	GetMesh()->SetupAttachment(RootComponent);
	StatComponent = CreateDefaultSubobject<UStatComponent>("StatComponent");
	
	//TODO HardCoded
	TeamID = FGenericTeamId(1);
	AttackRange = 100.0f;
	// Mesh->AnimClass = nullptr;
	
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	OriginLocation = GetActorLocation();
}

FVector AMonsterBase::GetOriginLocation() const
{
	return OriginLocation;
}

float AMonsterBase::GetAttackRange() const
{
	return AttackRange;
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGenericTeamId AMonsterBase::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type AMonsterBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamID = Cast<IGenericTeamAgentInterface>(&Other);
	if (OtherTeamID)
	{
		return (TeamID == OtherTeamID->GetGenericTeamId())?ETeamAttitude::Friendly:ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Neutral;
}

