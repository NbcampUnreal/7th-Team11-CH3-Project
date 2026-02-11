// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ClearIsFighting.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_ClearIsFighting::UBTT_ClearIsFighting()
{
	NodeName = "ClearIsFighting";
	IsFightingSelector.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_ClearIsFighting, IsFightingSelector))	;
}

EBTNodeResult::Type UBTT_ClearIsFighting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if ( UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsBool(IsFightingSelector.SelectedKeyName, false);
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
