// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "NaniteSceneProxy.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionProfileName(FName("OverlapAll"));
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponActor::Init(const FWeaponItemData& WeaponItem, USkeletalMeshComponent* SkeletalMesh)
{
	AttackMontage.LoadSynchronous();
	//TODO
	WeaponItemData = WeaponItem;

	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	
	AttachToComponent(SkeletalMesh, AttachRules, TEXT("handslot_r"));
}

UAnimMontage* AWeaponActor::GetAttackMontage() const
{
	return AttackMontage.Get();
}

float AWeaponActor::GetAttackRange() const
{
	return AttackRange;
}

void AWeaponActor::StartAttack()
{
}

void AWeaponActor::EndAttack()
{
}

TArray<AActor*> AWeaponActor::GetTargets()
{	
	DrawDebugSphere(GetWorld(),GetActorLocation(),10.0f,10.0f,FColor::Red, false,2.0f,0,2.0f);
	return {};
}

EWeaponType AWeaponActor::GetWeaponType()
{
	return WeaponItemData.WeaponType;
}
