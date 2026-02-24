// Fill out your copyright notice in the Description page of Project Settings.


#include "StaffWeaponActor.h"
#include "Components/Skills/SkillDataAsset.h"

// Sets default values
AStaffWeaponActor::AStaffWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionProfileName(FName("OverlapAll"));
}

void AStaffWeaponActor::Init(const FWeaponItemData* WeaponItem, USkeletalMeshComponent* SkeletalMesh)
{
	Super::Init(WeaponItem, SkeletalMesh);
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	StaticMeshComponent->AttachToComponent(SkeletalMesh, AttachRules,TEXT("handslot_r"));
}
//TODO
void AStaffWeaponActor::StartAttack(const FVector& TargetLocation,  USkillDataAsset* Skill) 
{
	Super::StartAttack(TargetLocation, Skill);
	Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	DrawDebugSphere(GetWorld(), TargetLocation, 20.f, 12, FColor::Green, false, 1.f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 20.f, 12, FColor::Green, false, 1.f);
	GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red,FString::Printf(TEXT("%f,%f,%f"),Direction.X,Direction.Y,Direction.Z));
	UE_LOG(LogTemp, Warning, TEXT("StartAttack Called!"));
}
void AStaffWeaponActor::PerformDamage()
{
	if (CurrentSkillData.IsValid())
	{
		CurrentSkillData->Activate(Cast<APawn>(GetOwner()), this,GetActorLocation(), Direction);
	}else
	{
		check(false);
	}
	UE_LOG(LogTemp, Warning, TEXT("PerformDamage Called!"));
}

void AStaffWeaponActor::EndAttack()
{
	//Direction = FVector::Zero();
	UE_LOG(LogTemp, Warning, TEXT("EndAttack Called!"));
}
