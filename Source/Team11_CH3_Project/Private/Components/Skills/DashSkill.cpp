// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/DashSkill.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/StatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void UDashSkill::Activate(UActiveSkillSlot* InActiveSkillSlot)
{
	Super::Activate(InActiveSkillSlot);
	ChargingTimer = 0.0f;
	bIsDashing = false;
	NavDestination = ActiveSkillSlot->GetTargetLocation();
}



void UDashSkill::Execute()
{
	Super::Execute();
}

void UDashSkill::Tick(float DeltaSeconds)
{
	ChargingTimer += DeltaSeconds;
	UAnimInstance* AnimInstance = nullptr;
	AActor* Owner = ActiveSkillSlot->GetOwner();
	if (ACharacter* Character = Cast<ACharacter>(Owner))
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
		
		UNavigationSystemV1* NavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Owner->GetWorld());
		if (NavigationSystemV1)
		{
			
			FNavLocation ResultLocation;
			FVector NavDashingStartLocation = Owner->GetActorLocation();
			NavDestination = ActiveSkillSlot->GetTargetLocation();
			FVector Extents = FVector(100.0f, 100.0f, 500.0f);
			if (NavigationSystemV1->ProjectPointToNavigation(NavDestination, ResultLocation,Extents))
			{
				NavDestination = ResultLocation.Location; 
			}
			if (NavigationSystemV1->ProjectPointToNavigation(NavDashingStartLocation, ResultLocation,Extents))
			{
				NavDashingStartLocation = ResultLocation.Location; 
			}
			DashingDir = (NavDestination - NavDashingStartLocation).GetSafeNormal();
		}
		// 여기서 VFX 스폰 몬스터에 붙여서
		if (IsValid(DashVFX))
		{
			if (ACharacter* Character = Cast<ACharacter>(Owner))
			{
				USkeletalMeshComponent* MeshComp = Character->GetMesh();
				if (IsValid(MeshComp))
				{
					FRotator DashRotation = (-DashingDir).Rotation();
					UNiagaraFunctionLibrary::SpawnSystemAttached(
						DashVFX,
						MeshComp,
						NAME_None,         
						Owner->GetActorLocation(),
						DashRotation,
						EAttachLocation::KeepWorldPosition,
						true
					);
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Dash"));
		}
		// Destination = ActiveSkillSlot->GetTargetLocation();
		AnimInstance->Montage_JumpToSection(TEXT("Dashing"),SkillMontage);
		return;
	}
	//최대시간 초과 처리
	if (ChargingTimer>Duration)
	{	
		AnimInstance->Montage_Stop(0.2f);
	}
	//적과 충돌
	//TODO Optimization TargetActor

	//Dashing
	if (bIsDashing)
	{
		FVector CurrentLoc = Owner->GetActorLocation();
		
		FVector NextLoc = CurrentLoc + (DashingDir * DashSpeed* DeltaSeconds) ;
		UE_LOG(LogTemp, Warning, TEXT("CurrentLoc Vector: %s, Size: %f"), *CurrentLoc.ToString(), CurrentLoc.Size());
		UE_LOG(LogTemp, Warning, TEXT("Destination Vector: %s, Size: %f"), *NavDestination.ToString(), NavDestination.Size());
		UE_LOG(LogTemp, Warning, TEXT("Dir Vector: %s, Size: %f"), *DashingDir.ToString(), DashingDir.Size());
		UE_LOG(LogTemp, Warning, TEXT("NextLoc Vector: %s, Size: %f"), *NextLoc.ToString(), NextLoc.Size());
		
		FHitResult Hit;
		Owner->SetActorLocation(NextLoc, true, &Hit);

		if (FVector::DistSquared(Owner->GetActorLocation(), ActiveSkillSlot->GetTargetLocation()) < 250000.f)
		{
			APawn* Instigator = Cast<APawn> (ActiveSkillSlot->GetOwner());
			if (!Instigator)
			{
				return;
			}
	
			TArray<AActor*> OverlappingActors;
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // Pawn만 검출

			UKismetSystemLibrary::SphereOverlapActors(
				Instigator->GetWorld(),
				Instigator->GetActorLocation(), 
				200.0f, 
				ObjectTypes, 
				APawn::StaticClass(),
				TArray<AActor*>(), 
				OverlappingActors
			);

			UStatComponent* StatComp = Instigator->FindComponentByClass<UStatComponent>();
			float BaseDamage = Damage;
			if (IsValid(StatComp))
			{
				BaseDamage += StatComp->GetCurrentStat(EStat::AttackDamage);
			}
	
			bool bIsHit = false;
			for (AActor* HitActor : OverlappingActors)
			{
				if (IsValid(HitActor) == false || HitActor == Instigator)
				{
					continue;
				}
				AController* InstigatorController = Instigator->GetController();
				if (InstigatorController == nullptr)
				{
					continue;
				}

				if (const IGenericTeamAgentInterface* InstigatorTeam = Cast<IGenericTeamAgentInterface>(InstigatorController))
				{
					if (InstigatorTeam->GetTeamAttitudeTowards(*HitActor) != ETeamAttitude::Hostile)
					{
						continue;
					}
				}
				bIsHit = true;
				float ActualDamage = BaseDamage;
				if (IsValid(StatComp))
				{
					// 크리티컬 판정
					float CriticalChance = StatComp->GetCurrentStat(EStat::CriticalChance);
					float CriticalMultiplier = StatComp->GetCurrentStat(EStat::CriticalDamage);

					if (FMath::FRandRange(0.0f, 100.0f) < CriticalChance)
					{
						ActualDamage *= CriticalMultiplier;
						UE_LOG(LogTemp, Warning, TEXT("Critical Hit"));
					}
				}
				UGameplayStatics::ApplyDamage(
					HitActor,
					ActualDamage,
					InstigatorController,
					Instigator,
					UDamageType::StaticClass()
				);
				UE_LOG(LogTemp, Warning, TEXT("Hit : %s, Damage : %0.1f"), *HitActor->GetName(), ActualDamage);
			}
			
			if (bIsHit)
			{
				AnimInstance->Montage_Stop(0.2f);
			}
			
		}
		
		
		
		if (FVector::DistSquared(NextLoc, NavDestination) < 250000.0f)
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
