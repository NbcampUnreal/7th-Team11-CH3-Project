// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/PickupActor.h"

#include "Characters/InventoryComponent.h"
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
	
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBegin);
}

void APickupActor::Init(UItemDataAsset* InItemDataAsset, int32 InCount)
{
	ItemDataAsset = InItemDataAsset;
	ItemCount = InCount;
}

void APickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UInventoryComponent* InventoryComponent = OtherActor->FindComponentByClass<UInventoryComponent>();
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->AddItem(ItemDataAsset,ItemCount);
		Destroy();
	}
}
