// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/DashSkill.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

void UDashSkill::Activate(APawn* Instigator, AWeaponActor* WeaponActor, const FVector& Origin,
                          const FVector& TargetLocation)
{
	Super::Activate(Instigator, WeaponActor, Origin, TargetLocation);
}

void UDashSkill::Enter(AActor* Actor, const FVector& TargetLocation)
{
	Super::Enter(Actor,TargetLocation);
	ChargingTimer = 0.0f;
	bIsDashing = false;
	Destination = TargetLocation;
	
}

void UDashSkill::Execute()
{
	Super::Execute();
}

void UDashSkill::Tick(float DeltaSeconds, AActor* Actor, UActiveSkillSlot* ActiveSkillSlot)
{
	ChargingTimer += DeltaSeconds;
	UAnimInstance* AnimInstance = nullptr;
	if (ACharacter* Character = Cast<ACharacter>(Actor))
	{
		if (USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
		{
			AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		}
	}
	if (AnimInstance == nullptr)
	{
		check(false);
	}	
	
	if (bIsDashing == false && ChargingTimer > ChargingTime)
	{
		bIsDashing = true;
		ActiveSkillSlot->OnExecute();
		Destination = ActiveSkillSlot->GetTargetLocation();
		AnimInstance->Montage_JumpToSection(TEXT("Dashing"),SkillMontage);
		return;
	}
	//최대시간 초과 처리
	if (ChargingTimer>Duration)
	{	
		AnimInstance->Montage_Stop(0.2f);
	}
	//가까우면 처리
	if (FVector::DistSquared(Actor->GetActorLocation(), Destination) < 10.f)
	{
		AnimInstance->Montage_Stop(0.2f);
	}
	//Dashing
	if (bIsDashing)
	{
		FVector CurrentLoc = Actor->GetActorLocation();
		FVector NextLoc = FMath::VInterpTo(CurrentLoc, Destination, DeltaSeconds, DashSpeed);

		FHitResult Hit;
		Actor->SetActorLocation(NextLoc, true, &Hit);

		if (FVector::DistSquared(NextLoc, Destination) < 10.0f)
		{
			AnimInstance->Montage_Stop(0.2f, SkillMontage);
			return; 
		}
		// if (Hit.bBlockingHit)
		// {
		// 	AnimInstance->Montage_Stop(0.2f, SkillMontage);
		// 	return;
		// }
	}
	
}

void UDashSkill::OnExit()
{
	Super::OnExit();
}

float UDashSkill::GetScore(const AActor* Actor, const AActor* Target) const
{
	UNavigationSystemV1* NavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Actor->GetWorld());
	FVector HitResult;
	NavigationSystemV1->NavigationRaycast(Actor->GetWorld(), Actor->GetActorLocation(), Target->GetActorLocation(),
	                                      HitResult);
	if (FVector::DistSquared(HitResult, Target->GetActorLocation()) < 10.0f)
	{
		return 100.0f;
	}
	return -1.0f;
}
