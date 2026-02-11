// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_UpdateMaxMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UBTT_UpdateMaxMoveSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_UpdateMaxMoveSpeed();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FBlackboardKeySelector MoveSpeedKeySelector;
};
