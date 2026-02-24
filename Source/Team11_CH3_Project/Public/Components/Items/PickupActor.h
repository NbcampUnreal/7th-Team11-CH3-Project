// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/ItemTypes.h"
#include "PickupActor.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Thumbnail;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	//#include "Components/SphereComponent.h"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapSphere")
	TObjectPtr<class USphereComponent> OverlapSphere;
};
