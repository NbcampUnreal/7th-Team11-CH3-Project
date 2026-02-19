// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterBase.h"

#include "BrainComponent.h"
#include "WeaponActor.h"
#include "Characters/Monster/MonsterControllerBase.h"
#include "Components/SkillManager.h"
#include "Components/StatComponent.h"
#include "Components/Skills/SkillSlot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Subsystems/MonsterSubsystem.h"


// Sets default values
AMonsterBase::AMonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AIControllerClass = AMonsterControllerBase::StaticClass();
	SetRootComponent(RootComponent);
	GetMesh()->SetupAttachment(RootComponent);
	StatComponent = CreateDefaultSubobject<UStatComponent>("StatComponent");
	SkillComponent = CreateDefaultSubobject<USkillManager>("SkillComponent");

	ConstructorHelpers::FClassFinder<AMonsterControllerBase> MonsterControllerFinder(
		TEXT("/Game/Characters/Monster/BP_MonsterControllerBase.BP_MonsterControllerBase_C"));
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
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->ResumeLogic(TEXT("Init"));
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	WeaponActor = GetWorld()->SpawnActor<AWeaponActor>(MonsterData.WeaponItemData.WeaponActorClass.LoadSynchronous(),
	                                                   SpawnInfo);
	if (WeaponActor)
	{
		WeaponActor->Init(MonsterData.WeaponItemData, GetMesh());
	}
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetAnimInstanceClass(MonsterData.AnimBlueprint.LoadSynchronous());
		StopAnimMontage();
		SkeletalMeshComponent->GetAnimInstance()->InitializeAnimation();
	}
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetBaseStat(EStat::MoveSpeed);
	BlackboardUpdate();
}

void AMonsterBase::Clear()
{
	StopAnimMontage();
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->PauseLogic(TEXT("Death"));
	}
	WeaponActor->Destroy();
}

float AMonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                               class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead())
	{
		return ActualDamage;
	}
	StatComponent->TakeDamage(ActualDamage);
	
	
	if (IsDead())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, *GetName().Append(TEXT("DEAD")) );
		
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->GetBrainComponent()->PauseLogic(TEXT("Death"));
		}
		StopAnimMontage();
		PlayAnimMontage(MonsterDieAnimMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMonsterBase::OnDieMontageEnded);
		if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
		{
			if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
			{
				AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterDieAnimMontage);
			}
		}
	};
	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f"), StatComponent->GetCurrentHP()));
	return ActualDamage;
}


void AMonsterBase::OnDieMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (UWorld* World = GetWorld())
	{
		World->GetSubsystem<UMonsterSubsystem>()->OnMonsterDeath(this);
	}

}


void AMonsterBase::BlackboardUpdate()
{
	if (AMonsterControllerBase* MonsterControllerBase = Cast<AMonsterControllerBase>(GetController()))
	{
		MonsterControllerBase->BlackboardUpdate();
	}
}

bool AMonsterBase::IsDead() const
{
	
	
	if (StatComponent)
	{
		return StatComponent->IsDead();
	}
	return true;
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

UStatComponent* AMonsterBase::GetStatComponent() const
{
	return StatComponent;
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//Toilet
bool AMonsterBase::TryAttack(AActor* Target)
{
	if (!Target || bIsAttacking)
	{
		return false;
	}

	TArray<int32> Indexes = SkillComponent->FindReadySlotIndexes();
	if (Indexes.Num() == 0)
	{
		return false;
	}
	int32 Index = Indexes[FMath::RandRange(0, Indexes.Num() - 1)];
	if (SkillComponent->GetCooldownRemaining(Index) > 0.0f)
	{
		return false;
	}

	//	SkillComponent
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

	SkillComponent->StartSkillCooldown(Index);
	WeaponActor->StartAttack(Target->GetActorLocation(),
	                         SkillComponent->GetSkillSlot(Index)->GetEquippedSkill());

	return true;
}

//CallByAnimNotify
void AMonsterBase::DealDamage()
{
	if (IsValid(this) && WeaponActor){
		WeaponActor->PerformDamage();
	}
}


void AMonsterBase::OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsAttacking = false;
	OnAttackFinished.Broadcast();
	if (WeaponActor)
	{
		WeaponActor->EndAttack();
	}
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
}
