// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearIsFighting.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UBTT_ClearIsFighting : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ClearIsFighting();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	FBlackboardKeySelector IsFightingSelector;
};
