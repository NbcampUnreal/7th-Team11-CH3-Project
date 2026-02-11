// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/BTT_GetPatrolLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/MonsterBase.h"

UBTT_GetPatrolLocation::UBTT_GetPatrolLocation()
{
	NodeName = "GetPatrolLocation";
}

EBTNodeResult::Type UBTT_GetPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AMonsterBase* Monster =  Cast<AMonsterBase>(AIController->GetCharacter()))
		{
			FVector2D RandPoint =  FMath::RandPointInCircle(500.0f);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"),Monster->GetOriginLocation()+FVector{RandPoint.X, RandPoint.Y,0.0f});
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
