#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Characters/PlayerCharacter.h"
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

	if (InventoryAction)
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleOpenInventory);

	if (Skill1Action)
		EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Started, this, &AMainPlayerController::HandleUseSkill1);

	if (Consumable1Action)
		EnhancedInputComponent->BindAction(Consumable1Action, ETriggerEvent::Started, this, &AMainPlayerController::HandleUseConsumable1);
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