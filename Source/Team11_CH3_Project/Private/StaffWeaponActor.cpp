// Fill out your copyright notice in the Description page of Project Settings.


#include "StaffWeaponActor.h"


// Sets default values
AStaffWeaponActor::AStaffWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionProfileName(FName("OverlapAll"));
}

void AStaffWeaponActor::Init(const FWeaponItemData& WeaponItem, USkeletalMeshComponent* SkeletalMesh)
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
void AStaffWeaponActor::StartAttack(AActor* Target/*, SKill skill*/)
{
	UE_LOG(LogTemp, Warning, TEXT("StartAttack Called!"));
}
void AStaffWeaponActor::PerformDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("PerformDamage Called!"));
}

void AStaffWeaponActor::EndAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("EndAttack Called!"));
}
