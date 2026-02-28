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


void AGreatSwordWeaponActor::Init(UWeaponItemDataAsset* WeaponItemDataAsset, USkeletalMeshComponent* SkeletalMesh)
{
	Super::Init(WeaponItemDataAsset, SkeletalMesh);
	FAttachmentTransformRules AttachRules(
	EAttachmentRule::SnapToTarget,
	EAttachmentRule::SnapToTarget,
	EAttachmentRule::KeepWorld,
	true
);
	StaticMeshComponent->AttachToComponent(SkeletalMesh, AttachRules,TEXT("handslot_r"));
}
