// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Items/PickupActor.h"
#include "Components/Items/ItemDataAsset.h"
#include "Characters/InventoryComponent.h"
#include "Components/ItemManager.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// 콜리전 설정
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;
	OverlapSphere->SetSphereRadius(100.f);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBegin);

	// 스피어 메시 (비주얼)
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(OverlapSphere);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetRelativeScale3D(FVector(0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh.Succeeded())
		ItemMesh->SetStaticMesh(SphereMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(
		TEXT("/Game/Thumbnail/M_PickupThumbnail.M_PickupThumbnail"));
	if (MatFinder.Succeeded())
		ThumbnailMaterial = MatFinder.Object;
}

void APickupActor::Init(UItemDataAsset* InItemDataAsset, int32 InCount)
{
	ItemDataAsset = InItemDataAsset;
	ItemCount = InCount;

	if (IsValid(ThumbnailMaterial) == false || IsValid(ItemDataAsset) == false)
		return;
	UTexture2D* Thumbnail = ItemDataAsset->GetThumbnail();
	if (IsValid(Thumbnail) == false)
		return;
	DynamicMaterial = UMaterialInstanceDynamic::Create(ThumbnailMaterial, this);
	DynamicMaterial->SetTextureParameterValue(TEXT("Thumbnail"), Thumbnail);
	ItemMesh->SetMaterial(0, DynamicMaterial);
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
