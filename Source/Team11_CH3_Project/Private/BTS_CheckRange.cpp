// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_CheckRange::UBTS_CheckRange()
{
	NodeName = "BTS_CheckRange";
	IsInRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_CheckRange, IsInRangeKey));
}

void UBTS_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Pawn || !BB)
	{
		return;
	}
	if (AActor* Target = Cast<AActor> (BB->GetValueAsObject("TargetActor")))
	{
		float Range = BB->GetValueAsFloat("AttackRange");
		bool bIsInRange = FVector::DistSquared(Target->GetActorLocation(), Pawn->GetActorLocation()) < Range*Range;
		BB->SetValueAsBool(IsInRangeKey.SelectedKeyName,bIsInRange);
		
	}
}
