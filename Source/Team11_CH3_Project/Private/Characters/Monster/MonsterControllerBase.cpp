// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AMonsterControllerBase::AMonsterControllerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Game/Characters/Monster/BT_Monster.BT_Monster"));
	if (BehaviorTreeFinder.Succeeded())
	{
		BehaviorTree = BehaviorTreeFinder.Object;
	}
}

void AMonsterControllerBase::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ACharacter* PossessedCharacter = Cast<ACharacter>(InPawn))
	{
		PossessedCharacter->bUseControllerRotationYaw = false;
		if (UCharacterMovementComponent* MovComp = PossessedCharacter->GetCharacterMovement())
		{
			MovComp->bOrientRotationToMovement = true;
		}
	}
}

void AMonsterControllerBase::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	AActor* Target =  GetWorld()->GetFirstPlayerController()->GetPawn();
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Target);
}
