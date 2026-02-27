// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CanUseSkill.generated.h"

/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UBTS_CanUseSkill : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_CanUseSkill();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BlackBoard")
	FBlackboardKeySelector CanUseSkill;
};
