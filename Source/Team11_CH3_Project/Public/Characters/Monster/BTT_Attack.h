// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
struct FBTT_AttackMemory
{
	FDelegateHandle OnAttackFinishedHandle;
};

UCLASS()
class TEAM11_CH3_PROJECT_API UBTT_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTT_Attack();
protected:
	virtual uint16 GetInstanceMemorySize() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FBlackboardKeySelector TargetActorSelector;
};
