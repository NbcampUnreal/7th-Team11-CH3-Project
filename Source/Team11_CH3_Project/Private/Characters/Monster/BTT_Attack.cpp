// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/BTT_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/MonsterControllerBase.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = "Attack";
	bNotifyTick = true;
	TargetActorSelector.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Attack, TargetActorSelector),
	                                    AActor::StaticClass());
}

uint16 UBTT_Attack::GetInstanceMemorySize() const
{
	return sizeof(FBTT_AttackMemory);
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTT_AttackMemory* AttackMemory = CastInstanceNodeMemory<FBTT_AttackMemory>(NodeMemory);
	AMonsterControllerBase* MonsterControllerBase = Cast<AMonsterControllerBase>(OwnerComp.GetAIOwner());
	if (!AttackMemory || !MonsterControllerBase)
	{
		return EBTNodeResult::Failed;
	}
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor  = Cast<AActor>(BB->GetValueAsObject(TargetActorSelector.SelectedKeyName));
		if (MonsterControllerBase->TryAttack(TargetActor))
		{
			MonsterControllerBase->OnAttackFinished.Remove(AttackMemory->OnAttackFinishedHandle);
			AttackMemory->OnAttackFinishedHandle = MonsterControllerBase->OnAttackFinished.AddLambda(
			[this, &OwnerComp, AttackMemory, MonsterControllerBase]()
			{
				MonsterControllerBase->OnAttackFinished.Remove(AttackMemory->OnAttackFinishedHandle);
				AttackMemory->OnAttackFinishedHandle.Reset();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}	
			);
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
	
}

EBTNodeResult::Type UBTT_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTT_AttackMemory* AttackMemory = CastInstanceNodeMemory<FBTT_AttackMemory>(NodeMemory);
	AMonsterControllerBase* MonsterControllerBase = Cast<AMonsterControllerBase>(OwnerComp.GetAIOwner());
	if (MonsterControllerBase && AttackMemory->OnAttackFinishedHandle.IsValid())
	{
		MonsterControllerBase->OnAttackFinished.Remove(AttackMemory->OnAttackFinishedHandle);
		AttackMemory->OnAttackFinishedHandle.Reset();
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

