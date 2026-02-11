// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/BTT_UpdateMaxMoveSpeed.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_UpdateMaxMoveSpeed::UBTT_UpdateMaxMoveSpeed()
{
	NodeName = "UpdateMaxMoveSpeed";
	MoveSpeedKeySelector.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_UpdateMaxMoveSpeed, MoveSpeedKeySelector));
}

EBTNodeResult::Type UBTT_UpdateMaxMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (AAIController* AIController =  OwnerComp.GetAIOwner())
	{
		if (ACharacter* Character = AIController->GetCharacter())
		{
			if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
			{
				if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent()){
					MovementComponent->MaxWalkSpeed = BB->GetValueAsFloat(MoveSpeedKeySelector.SelectedKeyName);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
