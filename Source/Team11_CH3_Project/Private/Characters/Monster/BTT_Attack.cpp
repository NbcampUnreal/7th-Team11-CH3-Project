// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/BTT_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/MonsterBase.h"
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
	AMonsterBase* MonsterBase = Cast<AMonsterBase>(MonsterControllerBase->GetPawn());
	if (!MonsterBase)
	{
		return EBTNodeResult::Failed;
	}
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor  = Cast<AActor>(BB->GetValueAsObject(TargetActorSelector.SelectedKeyName));
		if (MonsterBase->TryAttack(TargetActor))
		{
			MonsterControllerBase->SetFocus(TargetActor);
			MonsterBase->OnAttackFinished.Remove(AttackMemory->OnAttackFinishedHandle);
			
			TWeakObjectPtr<UBehaviorTreeComponent> WeakOwnerComp = &OwnerComp;
			TWeakObjectPtr<UBTT_Attack> WeakThis = this;
			
			AttackMemory->OnAttackFinishedHandle = MonsterBase->OnAttackFinished.AddLambda(
			[WeakThis, WeakOwnerComp]()
			{
				if (WeakOwnerComp.IsValid() && WeakThis.IsValid())
				{
					WeakThis->FinishLatentTask(*WeakOwnerComp, EBTNodeResult::Succeeded);
				}
			}	
			);
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
	
}

void UBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AMonsterControllerBase* MonsterControllerBase = Cast<AMonsterControllerBase>(OwnerComp.GetAIOwner());
	if (!MonsterControllerBase)
	{
		return;
	}
	AMonsterBase* MonsterBase = Cast<AMonsterBase>(MonsterControllerBase->GetPawn());
	if (!MonsterBase)
	{
		return ;
	}
	
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		AActor* TargetActor  = Cast<AActor>(BB->GetValueAsObject(TargetActorSelector.SelectedKeyName));
		if (TargetActor)
		{
			MonsterBase->UpdateTargetLocation(TargetActor->GetActorLocation());
		}
	}
	
	
	
}

void UBTT_Attack::ClearDelegate(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTT_AttackMemory* AttackMemory = CastInstanceNodeMemory<FBTT_AttackMemory>(NodeMemory);
	AMonsterControllerBase* MonsterControllerBase = Cast<AMonsterControllerBase>(OwnerComp.GetAIOwner());
	if (MonsterControllerBase)
	{
		MonsterControllerBase->ClearFocus(EAIFocusPriority::Gameplay);
		if (AttackMemory->OnAttackFinishedHandle.IsValid())
		{
			if (AMonsterBase* MonsterBase = Cast<AMonsterBase>(MonsterControllerBase->GetPawn()))
			{
				MonsterBase->OnAttackFinished.Remove(AttackMemory->OnAttackFinishedHandle);
				AttackMemory->OnAttackFinishedHandle.Reset();
			}
		}
	}
}

EBTNodeResult::Type UBTT_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ClearDelegate(OwnerComp, NodeMemory);
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	ClearDelegate(OwnerComp, NodeMemory);
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

