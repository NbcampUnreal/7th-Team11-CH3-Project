// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckRange.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UBTS_CheckRange : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_CheckRange();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BlackBoard")
	FBlackboardKeySelector IsInRangeKey;
};
