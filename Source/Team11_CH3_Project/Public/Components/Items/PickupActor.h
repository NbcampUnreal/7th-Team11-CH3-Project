// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/ItemTypes.h"
#include "PickupActor.generated.h"

class UItemDataAsset;

UCLASS()
class TEAM11_CH3_PROJECT_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();
	UFUNCTION(BlueprintCallable)
	void Init(UItemDataAsset* InItemDataAsset, int32 InCount);

//TODO Mesh
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	//#include "Components/SphereComponent.h"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapSphere")
	TObjectPtr<class USphereComponent> OverlapSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemDataAsset> ItemDataAsset;
	int32 ItemCount;
};
