// Fill out your copyright notice in the Description page of Project Settings.


#include "DualwieldWeaponActor.h"


// Sets default values
ADualwieldWeaponActor::ADualwieldWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponentL = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponentL"));
	StaticMeshComponentL->SetupAttachment(GetRootComponent());
	StaticMeshComponentL->SetCollisionProfileName(FName("OverlapAll"));
	
	StaticMeshComponentR = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponentR"));
	StaticMeshComponentR->SetupAttachment(GetRootComponent());
	StaticMeshComponentR->SetCollisionProfileName(FName("OverlapAll"));
	
}

void ADualwieldWeaponActor::Init(UWeaponItemDataAsset* WeaponItemDataAsset, USkeletalMeshComponent* SkeletalMesh)
{
	Super::Init(WeaponItemDataAsset, SkeletalMesh);
	
	Super::Init(WeaponItemDataAsset, SkeletalMesh);
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	StaticMeshComponentL->AttachToComponent(SkeletalMesh, AttachRules,TEXT("handslot_l"));
	StaticMeshComponentR->AttachToComponent(SkeletalMesh, AttachRules,TEXT("handslot_r"));
}

