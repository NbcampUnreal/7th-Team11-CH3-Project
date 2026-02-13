// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/MonsterBase.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Touch.h"
#include "Perception/AISense_Touch.h"


AMonsterControllerBase::AMonsterControllerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(
		TEXT("/Game/Characters/Monster/BT_Monster.BT_Monster"));
	if (BehaviorTreeFinder.Succeeded())
	{
		BehaviorTree = BehaviorTreeFinder.Object;
	}
	bAttachToPawn = true;

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComp");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("HearingConfig");
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageConfig");
	TouchConfig = CreateDefaultSubobject<UAISenseConfig_Touch>("TouchConfig");

	SightConfig->LoseSightRadius = 1500.0f;
	SightConfig->SightRadius = 1000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 45.0f;
	SightConfig->SetMaxAge(1000.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	HearingConfig->HearingRange = 400.0f;
	HearingConfig->SetMaxAge(10.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;

	DamageConfig->SetMaxAge(1000.0f);

	TouchConfig->DetectionByAffiliation.bDetectEnemies = true;
	TouchConfig->DetectionByAffiliation.bDetectFriendlies = false;
	TouchConfig->DetectionByAffiliation.bDetectNeutrals = false;
	TouchConfig->SetMaxAge(1000.0f);

	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->ConfigureSense(*HearingConfig);
	AIPerceptionComp->ConfigureSense(*DamageConfig);
	AIPerceptionComp->ConfigureSense(*TouchConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterControllerBase::TargetPerceptionUpdated);
	AIPerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &AMonsterControllerBase::TargetPerceptionForgotten);
	
	//TODO HardCoded
	TeamID = FGenericTeamId(1);
}

void AMonsterControllerBase::BlackboardUpdate()
{
	if (AMonsterBase* PossessedCharacter = Cast<AMonsterBase>(GetPawn()))
	{
		PossessedCharacter->bUseControllerRotationYaw = false;
		if (UCharacterMovementComponent* MovComp = PossessedCharacter->GetCharacterMovement())
		{
			MovComp->bOrientRotationToMovement = true;
		}
		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsFloat(TEXT("FightMaxMoveSpeed"), PossessedCharacter->GetCharacterMovement()->GetMaxSpeed());
			BB->SetValueAsFloat(TEXT("PatrolMaxMoveSpeed"), PossessedCharacter->GetCharacterMovement()->GetMaxSpeed() / 3);
			BB->SetValueAsFloat(TEXT("AttackRange"), PossessedCharacter->GetAttackRange());
		}
	}
}


void AMonsterControllerBase::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);
	BlackboardUpdate();
}

void AMonsterControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterControllerBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (APawn* CurrentPwan = GetPawn())
	{
		OutLocation = CurrentPwan->GetPawnViewLocation();
		OutRotation = CurrentPwan->GetActorRotation();
	}
	else
	{
		Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	}
}


void AMonsterControllerBase::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}
	if (Stimulus.WasSuccessfullySensed())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, Actor->GetName());

		TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
		if (SenseClass == nullptr)
		{
			return;
		}
		if (SenseClass == UAISense_Sight::StaticClass() || SenseClass == UAISense_Damage::StaticClass() || SenseClass ==
			UAISense_Touch::StaticClass())
		{
			if (UBlackboardComponent* BB = GetBlackboardComponent())
			{
				BB->SetValueAsObject(TEXT("TargetActor"), Actor);
				BB->SetValueAsBool(TEXT("bIsFighting"), true);
			}
		}
		if (SenseClass == UAISense_Hearing::StaticClass())
		{
			if (UBlackboardComponent* BB = GetBlackboardComponent())
			{
				BB->SetValueAsVector(TEXT("LastHearingLocation"), Actor->GetActorLocation());
				BB->SetValueAsBool(TEXT("bIsTracking"), true);
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, Actor->GetName());
	}
}

//Expired
//Nothing To do Now (Eternal Sense)
void AMonsterControllerBase::TargetPerceptionForgotten(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, Actor->GetName());

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsVector(TEXT("LastHearingLocation"), FVector::ZeroVector);
		BB->SetValueAsBool(TEXT("bIsTracking"), false);
	}
}


FGenericTeamId AMonsterControllerBase::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type AMonsterControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* OtherTeamID = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamID == OtherTeamID->GetGenericTeamId())
			{
				return ETeamAttitude::Friendly;
			}else
			{
				return ETeamAttitude::Hostile;	
			}
		}
	}
	return ETeamAttitude::Neutral;
}
