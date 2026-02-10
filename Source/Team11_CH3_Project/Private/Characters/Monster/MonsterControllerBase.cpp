// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/MonsterControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Game/Characters/Monster/BT_Monster.BT_Monster"));
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
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	HearingConfig->HearingRange = 400.0f;
	HearingConfig->SetMaxAge(5.0f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DamageConfig->SetMaxAge(5.0f);

	TouchConfig->DetectionByAffiliation.bDetectEnemies = true;
	TouchConfig->DetectionByAffiliation.bDetectFriendlies = true;
	TouchConfig->DetectionByAffiliation.bDetectNeutrals = true;
	TouchConfig->SetMaxAge(5.0f);
	
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->ConfigureSense(*HearingConfig);
	AIPerceptionComp->ConfigureSense(*DamageConfig);
	AIPerceptionComp->ConfigureSense(*TouchConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterControllerBase::TargetPerceptionUpdated );
	AIPerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &AMonsterControllerBase::TargetPerceptionForgotten);
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
		GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red, Actor->GetName());

		TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
		if (SenseClass == nullptr)
		{
			return;
		}
		if (SenseClass == UAISense_Sight::StaticClass() || SenseClass == UAISense_Damage::StaticClass() || SenseClass == UAISense_Touch::StaticClass())
		{
		
		}
		if (SenseClass == UAISense_Hearing::StaticClass())
		{
		
		}
			
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1,10,FColor::Green, Actor->GetName());
	}
	
}

//Expired
void AMonsterControllerBase::TargetPerceptionForgotten(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1,10,FColor::Cyan, Actor->GetName());

	
	
}
