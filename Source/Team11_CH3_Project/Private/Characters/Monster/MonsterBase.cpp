// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterBase.h"

#include "BrainComponent.h"
#include "WeaponActor.h"
#include "WeaponAnimInterface.h"
#include "Characters/Monster/MonsterControllerBase.h"
#include "Components/EquipmentComponent.h"
#include "Components/SkillComponent.h"
#include "Components/StatComponent.h"
#include "Components/Items/WeaponItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"
#include "Components/Skills/SkillSlot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
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
	SkillComponent = CreateDefaultSubobject<USkillComponent>("SkillComponent");
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
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


void AMonsterBase::EquipWeapon(UEquipmentInstance* WeaponItemInstance)
{
	EquipmentComponent->SetItemAt(WeaponItemInstance, 0);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AnimInstance->GetClass()->ImplementsInterface(UWeaponAnimInterface::StaticClass()))
	{
		if (AWeaponActor* WeaponActor = EquipmentComponent->GetCurrentWeapon())
		{
			UAnimSequence* GripAnim = WeaponActor->GetGripAnimation();
			IWeaponAnimInterface::Execute_UpdateGripAnim(AnimInstance, WeaponActor->GetGripAnimation(),
			                                             (GripAnim != nullptr));
		}
	}
}

void AMonsterBase::Init(const FMonsterData* MonsterData)
{
	StatComponent->InitStat(MonsterData->StatData);
	// Monster의 점수 저장
	ScoreValue = MonsterData->ScoreValue;
	if (SkillComponent)
	{
		SkillComponent->Init();
	}
	OriginLocation = GetActorLocation();
	GetMesh()->SetSkeletalMesh(MonsterData->SkeletalMesh.LoadSynchronous());

	//	TODO
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->ResumeLogic(TEXT("Init"));
	}
	if (WeaponItemDataInstance)
	{
		//WeaponItemDataInstance->BeginDestroy();
		WeaponItemDataInstance = nullptr;
	}
	WeaponItemDataInstance = NewObject<UEquipmentInstance>(this);
	
	if (WeaponItemDataInstance)
	{
		WeaponItemDataInstance->Init(MonsterData->DefaultWeaponData.LoadSynchronous(), 1);
		EquipWeapon(WeaponItemDataInstance);
	}
	if (SkillComponent)
	{
		const TArray<TSoftObjectPtr<USkillDataAsset>>& Skills = MonsterData->Skills;

		for (int32 i = 0; i < Skills.Num(); ++i)
		{
			SkillComponent->EquipSkillGem(i + 1, Skills[i].LoadSynchronous());
		}
	}

	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetAnimInstanceClass(MonsterData->AnimBlueprint.LoadSynchronous());
		StopAnimMontage();
		SkeletalMeshComponent->GetAnimInstance()->InitializeAnimation();
	}
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = StatComponent->GetBaseStat(EStat::MoveSpeed);
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
	
	BlackboardUpdate();
	SetActorScale3D({1.f, 1.f, 1.f});
	
}

void AMonsterBase::Clear()
{
	StopAnimMontage();
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->PauseLogic(TEXT("Death"));
	}

	EquipmentComponent->Clear();
	SkillComponent->Clear();
	//WeaponItemDataInstance->BeginDestroy();
	EquipmentComponent->UnequipWeapon();
	WeaponItemDataInstance = nullptr;
	GetWorldTimerManager().ClearTimer(ExecuteTimer);
}

AWeaponActor* AMonsterBase::GetWeaponActor() const
{
	if (EquipmentComponent)
	{
		return EquipmentComponent->GetCurrentWeapon();
	}
	return nullptr;
}

void AMonsterBase::UpdateTargetLocation(const FVector& Vector)
{
	if (SkillComponent)
	{
		if (UActiveSkillSlot* ActiveSkillSlot = SkillComponent->GetActiveSkillSlot())
		{
			ActiveSkillSlot->SetTargetLocation(Vector);
		}
	}
}

USoundBase* AMonsterBase::GetHearingReactSound() const
{
	if (HearingReactSound.IsEmpty())
	{
		return nullptr;
	}
	return HearingReactSound[FMath::RandRange(0, HearingReactSound.Num() - 1)];
}

float AMonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                               class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead())
	{
		return ActualDamage;
	}

	if (IsValid(EventInstigator) == false)
	{
		StatComponent->TakeDamage(ActualDamage);
		return ActualDamage;
	}

	StatComponent->TakeDamage(ActualDamage);
	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,
		EventInstigator->GetPawn(),
		ActualDamage,
		GetActorLocation(),
		GetActorLocation()
	);

	if (IsDead())
	{
		SetActorEnableCollision(false);

		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->GetBrainComponent()->PauseLogic(TEXT("Death"));
			AIController->StopMovement();
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}
		StopAnimMontage();
		PlayAnimMontage(MonsterDieAnimMontage, 1,TEXT("FullBody"));

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

	return ActualDamage;
}

void AMonsterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Clear();
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

bool AMonsterBase::IsAttacking() const
{
	return SkillComponent->IsSkillActive();
}

FVector AMonsterBase::GetOriginLocation() const
{
	return OriginLocation;
}

float AMonsterBase::GetAttackRange() const
{
	if (EquipmentComponent && EquipmentComponent->GetCurrentWeapon())
	{
		return EquipmentComponent->GetCurrentWeapon()->GetAttackRange();
	}
	return 0.0f;
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

void AMonsterBase::PerformSkill(USkillSlot* SkillSlot, const FVector& TargetLocation)
{
	if (SkillComponent->IsSkillActive())
	{
		return;
	}

	if (IsValid(SkillSlot->GetEquippedSkill()) == false)
		return;
	if (IsValid(SkillSlot->GetEquippedSkill()->GetSkillMontage()) == false)
		return;

	//	SkillComponent
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->bOrientRotationToMovement = false;
	}
	if (SkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Aiming)
	{
		TWeakObjectPtr<USkillComponent> WeakSkillComponent = SkillComponent;
		GetWorldTimerManager().SetTimer(
			ExecuteTimer,
			[WeakSkillComponent]()
			{
				if (WeakSkillComponent.IsValid())
				{
					WeakSkillComponent->ExecuteActiveSkill();
				}
			}, 0.5f,
			false);
	}

	SkillComponent->ActiveSkill(this, nullptr, SkillSlot);
}

void AMonsterBase::PerformSkill(USkillSlot* SkillSlot, AActor* Target)
{
	if (SkillComponent->IsSkillActive())
	{
		return;
	}

	if (IsValid(SkillSlot->GetEquippedSkill()) == false)
		return;
	if (IsValid(SkillSlot->GetEquippedSkill()->GetSkillMontage()) == false)
		return;

	//	SkillComponent
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->bOrientRotationToMovement = false;
	}
	if (SkillSlot->GetEquippedSkill()->GetSkillType() == ESkillType::Aiming)
	{
		TWeakObjectPtr<USkillComponent> WeakSkillComponent = SkillComponent;
		GetWorldTimerManager().SetTimer(
			ExecuteTimer,
			[WeakSkillComponent]()
			{
				if (WeakSkillComponent.IsValid())
				{
					WeakSkillComponent->ExecuteActiveSkill();
				}
			}, 0.5f,
			false);
	}

	SkillComponent->ActiveSkill(this, Target, SkillSlot);
}

bool AMonsterBase::TryAttack(AActor* Target)
{
	if (!Target)
	{
		return false;
	}
	if (SkillComponent->IsSkillActive())
	{
		return false;
	}


	int32 BestSkillIdx = SkillComponent->GetBestSkill(this, Target);
	if (SkillComponent->GetCooldownRemaining(BestSkillIdx) > 0.0f)
	{
		return false;
	}

	USkillSlot* SkillSlot = SkillComponent->GetSkillSlot(BestSkillIdx);
	// FVector TargetLocation = Target->GetActorLocation();

	PerformSkill(SkillSlot, Target);

	return true;
}

bool AMonsterBase::CanUseSkill(AActor* Target) const
{
	//TODO Optimization
	int32 BestSkillIdx = SkillComponent->GetBestSkill(this, Target);
	return BestSkillIdx >= 0;
}

//CallByAnimNotify
void AMonsterBase::DealDamage()
{
	SkillComponent->GetActiveSkillSlot()->Notify(TEXT("DealDamage"));
}


void AMonsterBase::OnAttackEnded()
{
	OnAttackFinished.Broadcast();
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
}
