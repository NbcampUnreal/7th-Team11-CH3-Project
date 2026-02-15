// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterBase.h"

#include "WeaponActor.h"
#include "Characters/Monster/MonsterControllerBase.h"
#include "Components/StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMonsterBase::AMonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AIControllerClass = AMonsterControllerBase::StaticClass();
	SetRootComponent(RootComponent);
	GetMesh()->SetupAttachment(RootComponent);
	StatComponent = CreateDefaultSubobject<UStatComponent>("StatComponent");
	ConstructorHelpers::FClassFinder<AMonsterControllerBase> MonsterControllerFinder(TEXT("/Game/Characters/Monster/BP_MonsterControllerBase.BP_MonsterControllerBase_C"));
	if (MonsterControllerFinder.Succeeded())
	{
		AIControllerClass = MonsterControllerFinder.Class;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintFinder(
		TEXT("/Game/Characters/Monster/Animations/ABP_Monster.ABP_Monster_C"));
	if (AnimBlueprintFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBlueprintFinder.Class);
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
}


void AMonsterBase::Init(const FMonsterData& MonsterData)
{
	StatComponent->InitStat(MonsterData.StatData);
	OriginLocation = GetActorLocation();
	GetMesh()->SetSkeletalMesh(MonsterData.SkeletalMesh.LoadSynchronous());
	bIsAttacking = false;
	//	TODO
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	WeaponActor = GetWorld()->SpawnActor<AWeaponActor>(MonsterData.WeaponItemData.WeaponActorClass.LoadSynchronous(),SpawnInfo);
	if (WeaponActor)
	{
		WeaponActor->Init(MonsterData.WeaponItemData, GetMesh());
		
	}
	
	GetMesh()->SetAnimInstanceClass(MonsterData.AnimBlueprint.LoadSynchronous());
	
	BlackboardUpdate();
}

void AMonsterBase::Clear()
{
	WeaponActor->Destroy();
}

void AMonsterBase::BlackboardUpdate()
{	
	if (AMonsterControllerBase* MonsterControllerBase = Cast<AMonsterControllerBase> (GetController()))
	{
		MonsterControllerBase->BlackboardUpdate();
	}
}

FVector AMonsterBase::GetOriginLocation() const
{
	return OriginLocation;
}

float AMonsterBase::GetAttackRange() const
{
	if (!WeaponActor)
	{
		return 0.0f;
	}
	return WeaponActor->GetAttackRange();
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


bool AMonsterBase::TryAttack(AActor* Target)
{
	if (!Target || bIsAttacking)
	{
		return false;
	}

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->bOrientRotationToMovement = false;
	}
	bIsAttacking = true;
	PlayAnimMontage(WeaponActor->GetAttackMontage());
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMonsterBase::OnAttackMontageEnded);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, WeaponActor->GetAttackMontage());
	
	// TODO
	// SKill = SkillSet[WeaponActor->GetWeaponType()][RandomIndex]
	WeaponActor->StartAttack(Target/*, SKill*/);
	
	
	return true;
}

//CallByAnimNotify
void AMonsterBase::DealDamage()
{
	WeaponActor->PerformDamage();
}



void AMonsterBase::OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsAttacking = false;
	OnAttackFinished.Broadcast();
	WeaponActor->EndAttack();
	
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
	
}
