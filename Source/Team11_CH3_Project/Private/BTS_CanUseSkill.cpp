// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CanUseSkill.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/MonsterBase.h"

UBTS_CanUseSkill::UBTS_CanUseSkill()
{
	NodeName = "BTS_CanUseSkill";
	CanUseSkill.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_CanUseSkill, CanUseSkill));
}

void UBTS_CanUseSkill::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Pawn || !BB)
	{
		return;
	}
	if (AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor")))
	{
		if (AMonsterBase* MonsterBase = Cast<AMonsterBase>(Pawn))
		{
			if (MonsterBase->CanUseSkill(Target))
			{
				BB->SetValueAsBool(CanUseSkill.SelectedKeyName, true);
				return;
			}
			else
			{
				BB->SetValueAsBool(CanUseSkill.SelectedKeyName, false);
				return;
			}
		}

	}
	BB->SetValueAsBool(CanUseSkill.SelectedKeyName, false);
	
}
