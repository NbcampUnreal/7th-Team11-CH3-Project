#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

struct FSpawnMonsterData;
class UMonsterSubSystem;
class UBoxComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnRandomMonster();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UDataTable> SpawnMonsterDataTable;

	FVector GetRandomPointInVolume() const;
	FSpawnMonsterData* GetRandomMonster() const;
};
