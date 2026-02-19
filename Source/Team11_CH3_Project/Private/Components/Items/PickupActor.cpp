// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/PickupActor.h"
#include "Components/ItemManager.h"
#include "Components/SphereComponent.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;
	OverlapSphere->SetSphereRadius(100.f);
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBegin);
}

void APickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* ItemManager = OtherActor->FindComponentByClass<UItemManager>();
	if (IsValid(ItemManager))
	{
		ItemManager->UseItem(RowName, ItemType, Index);
		Destroy();
	}
}