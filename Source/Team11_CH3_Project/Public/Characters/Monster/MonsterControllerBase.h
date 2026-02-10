// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterControllerBase.generated.h"

class UAISenseConfig_Touch;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
/**
 * 
 */
class UBehaviorTree;
class UAIPerceptionComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API AMonsterControllerBase : public AAIController
{
	GENERATED_BODY()

public:

	AMonsterControllerBase();

protected:
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void BeginPlay() override;
	
	
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAISenseConfig_Touch> TouchConfig;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;
};
