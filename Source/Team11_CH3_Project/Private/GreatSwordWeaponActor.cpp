// Fill out your copyright notice in the Description page of Project Settings.


#include "GreatSwordWeaponActor.h"


// Sets default values
AGreatSwordWeaponActor::AGreatSwordWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionProfileName(FName("OverlapAll"));
}

void AGreatSwordWeaponActor::Init(const FWeaponItemData* WeaponItem, USkeletalMeshComponent* SkeletalMesh)
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
void AGreatSwordWeaponActor::StartAttack(const FVector& TargetLocation,  USkillDataAsset* Skill) 
{
	FVector Direction = TargetLocation - GetActorLocation();
	Skill->Activate(Cast<APawn>(GetOwner()), GetActorLocation(), Direction);
	UE_LOG(LogTemp, Warning, TEXT("StartAttack Called!"));
}
void AGreatSwordWeaponActor::PerformDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("PerformDamage Called!"));
}

void AGreatSwordWeaponActor::EndAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("EndAttack Called!"));
}
