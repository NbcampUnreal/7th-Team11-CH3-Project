#include "MainPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/InventoryComponent.h"

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// 마우스 커서 설정
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	// Input Mode 설정
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	//TODO HardCoded
	TeamID = FGenericTeamId(0);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleLook);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopJumping);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleStartSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopSprint);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleDodge);

	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMainPlayerController::AimPressed);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainPlayerController::AimReleased);


	if (InventoryAction)
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleOpenInventory);

	if (Skill1Action)
		EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Started, this, &AMainPlayerController::HandleUseSkill1);

	if (Consumable1Action)
		EnhancedInputComponent->BindAction(Consumable1Action, ETriggerEvent::Started, this, &AMainPlayerController::HandleUseConsumable1);
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type AMainPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* OtherTeamID = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return (TeamID == OtherTeamID->GetGenericTeamId()) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}


void AMainPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerChar->Move(Value.Get<FVector2D>());
	}
}

void AMainPlayerController::HandleLook(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerChar->Look(Value.Get<FVector2D>());
	}
}

void AMainPlayerController::HandleJump()
{
	if (ACharacter* PlayerChar = Cast<ACharacter>(GetPawn()))
	{
		PlayerChar->Jump();
	}
}

void AMainPlayerController::HandleStopJumping()
{
	if (ACharacter* PlayerChar = Cast<ACharacter>(GetPawn()))
	{
		PlayerChar->StopJumping();
	}
}

void AMainPlayerController::HandleStartSprint()
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerChar->StartSprint();
	}
}

void AMainPlayerController::HandleStopSprint()
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerChar->StopSprint();
	}
}

void AMainPlayerController::HandleDodge()
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerChar->PerformDodge();
	}
}

void AMainPlayerController::HandleOpenInventory()
{
	UE_LOG(LogTemp, Log, TEXT("Inventory Opened"));
	// TODO: UI 위젯 표시
}

void AMainPlayerController::UseSkillSlot(int32 Index)
{ }

void AMainPlayerController::UseConsumableSlot(int32 Index)
{ }

void AMainPlayerController::HandleUseSkill1() { UseSkillSlot(0); }

void AMainPlayerController::HandleUseConsumable1() { UseConsumableSlot(0); }

// TODO - 추후 gamemode에서 캐릭터 사망 처리 후 직접 호출 방식으로 수정

void AMainPlayerController::HandlePlayerDeath()
{
	UnPossess();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
		},
		4.0f,
		false
	);
}

// Skill 호출 관련
/*
void AMainPlayerController::UseSkillSlot(int32 SlotIndex)
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		if (UInventoryComponent* Inventory = PlayerChar->GetInventoryComponent())
		{
			FSkillSlotData SkillData = Inventory->GetSkillSlot(SlotIndex);
			if (SkillData.SkillID != NAME_None)
			{
				UE_LOG(LogTemp, Log, TEXT("Using Skill Slot %d: %s"), SlotIndex, *SkillData.SkillID.ToString());
				// TODO: 외부 스킬 시스템 연동
			}
		}
	}
}
*/

/* void AMainPlayerController::UseConsumableSlot(int32 SlotIndex)
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn()))
	{
		if (UInventoryComponent* Inventory = PlayerChar->GetInventoryComponent())
		{
			if (Inventory->UseConsumable(SlotIndex))
			{
				UE_LOG(LogTemp, Log, TEXT("Used Consumable Slot %d"), SlotIndex);
			}
		}
	}
}*/

void AMainPlayerController::AimPressed()
{
	if (APlayerCharacter* ControlledPawn =
		Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledPawn->SetAiming(true);
	}
}

void AMainPlayerController::AimReleased()
{
	if (APlayerCharacter* ControlledPawn =
		Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledPawn->SetAiming(false);
	}
}

bool AMainPlayerController::GetAimPoint(FVector& OutAimPoint) const
{
	int32 SX, SY;
	GetViewportSize(SX, SY);

	FVector Origin, Dir;
	if (!DeprojectScreenPositionToWorld(SX * 0.5f, SY * 0.5f, Origin, Dir))
		return false;

	const float Dist = 100000.f;
	const FVector Start = Origin;
	const FVector End = Origin + Dir * Dist;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AimTrace), true);
	if (APawn* P = GetPawn()) Params.AddIgnoredActor(P);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	OutAimPoint = bHit ? Hit.ImpactPoint : End;
	return true;
}
