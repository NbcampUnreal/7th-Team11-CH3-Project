// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterControllerBase.generated.h"

/**
 * 
 */
class UBehaviorTree;

UCLASS()
class TEAM11_CH3_PROJECT_API AMonsterControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterControllerBase();
	
protected:
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="BehaviorTree")
	TObjectPtr<UBehaviorTree> BehaviorTree;

};
