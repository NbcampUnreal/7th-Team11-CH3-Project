#include "Core/Portal.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APortal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComponent = FindComponentByClass<UCapsuleComponent>();

	if (CollisionComponent) 
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

		//CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	}
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	SetPortalActive(false);
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UGameplayStatics::OpenLevel(this, FName(TargetLevel));
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::SetTargetLevel(FString LevelName)
{
	TargetLevel = LevelName;
}

void APortal::SetPortalActive(bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	PrimaryActorTick.bCanEverTick = bActive;
}

