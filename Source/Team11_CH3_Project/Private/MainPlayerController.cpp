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
#include "Perception/AIPerceptionSystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/T11_GameState.h"
#include "Components/StatComponent.h"
#include "UI/Subsystem/UIManageSubsystem.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

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
	//bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	// Input Mode 설정
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	}
	if (LoadingWidgetClass)
	{
		LoadingWidgetInstance = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
	}

	this->PlayerCameraManager->StartCameraFade(1.f, 0.f, 2.0f, FLinearColor::Black, false, false);

	LoadingToHUD();
	SetHUD();
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

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// TODO HardCoded
	TeamID = FGenericTeamId(0);
	SetGenericTeamId(TeamID);

	if (!IsLocalController())
	{
		return;
	}

	if (bHUDShown)
	{
		return;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	UUIManageSubsystem* UI = GI->GetSubsystem<UUIManageSubsystem>();
	if (!UI)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] UIManageSubsystem가 비어있음"));
		return;
	}

	const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TEXT("UI.Request.HUD"));
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] UI.Request.HUD tag가 없음. GameplayTags settings 확인 요망"));
		return;
	}

	UI->ShowWidget(Tag);
	bHUDShown = true;
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

void AMainPlayerController::AimReleased()
{
	if (APlayerCharacter* ControlledPawn =
		Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledPawn->SetAiming(false);
	}
}

UUserWidget* AMainPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMainPlayerController::LoadingToHUD()
{
	MainCanvas = Cast<UCanvasPanel>(LoadingWidgetInstance->GetWidgetFromName(TEXT("MainCanvas")));
	if (MainCanvas)
	{
		CurrentOpacity = 0.0f;
		bFadingIn = true;
		GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &AMainPlayerController::UpdateFade, 0.02f, true);
	}
	LoadingWidgetInstance->AddToViewport();
	if (UTextBlock* StageNameText = Cast<UTextBlock>(LoadingWidgetInstance->GetWidgetFromName(TEXT("StageName"))))
	{
		FString CurrentWorldName = GetWorld()->GetName().RightChop(2);
		StageNameText->SetText(FText::FromString(CurrentWorldName));
	}
}

void AMainPlayerController::UpdateFade()
{
	if (bFadingIn)
	{
		CurrentOpacity += 0.02f;
		if (MainCanvas)
		{
			MainCanvas->SetRenderOpacity(CurrentOpacity);
		}
		if (CurrentOpacity >= 1.5f)
		{
			bFadingIn = false;
		}
	}
	else
	{
		CurrentOpacity -= 0.02f;
		if (MainCanvas)
		{
			MainCanvas->SetRenderOpacity(CurrentOpacity);
		}
		if (CurrentOpacity <= 0.0f)
		{
			LoadingWidgetInstance->RemoveFromParent();
			HUDWidgetInstance->AddToViewport();
			GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
		}
	}
}

UUserWidget* AMainPlayerController::GetLoadingWidget() const
{
	return LoadingWidgetInstance;
}

void AMainPlayerController::UpdateMonsterCount(int32 MonsterCount)
{
	if (UUserWidget* HUDWidget = HUDWidgetInstance)
	{
		if (UTextBlock* MonsterCountText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("RemainingCount"))))
		{
			MonsterCountText->SetText(FText::AsNumber(MonsterCount));
		}
	}
}

void AMainPlayerController::UpdateStageInfo(int32 MaxWave)
{
	if (UUserWidget* HUDWidget = HUDWidgetInstance)
	{
		if (UTextBlock* StageNameText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("StageName"))))
		{
			FString CurrentWorldName = GetWorld()->GetName().RightChop(2);
			StageNameText->SetText(FText::FromString(CurrentWorldName));
		}
		for (int32 WaveIndex = MaxWave + 1; WaveIndex <= 5; WaveIndex++)
		{
			if (UImage* WaveImage = Cast<UImage>(HUDWidget->GetWidgetFromName(FName(*FString::Printf(TEXT("Wave%d"), WaveIndex)))))
			{
				WaveImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	if (UUserWidget* LoadingWidget = LoadingWidgetInstance)
	{
		//LoadingWidget->PlayAnimation(LoadingWidget->FadeOutBGAnim);
	}
}

void AMainPlayerController::UpdateWaveInfo(int32 CurrentWaveIndex, int32 MaxWave)
{
	if (UUserWidget* HUDWidget = HUDWidgetInstance)
	{
		for (int32 WaveIndex = 1; WaveIndex < CurrentWaveIndex; WaveIndex++)
		{
			if (UImage* WaveImage = Cast<UImage>(HUDWidget->GetWidgetFromName(FName(*FString::Printf(TEXT("Wave%d"), WaveIndex)))))
			{
				UTexture2D* Texture = WaveYellowTexture.LoadSynchronous();
				WaveImage->SetBrushFromTexture(Texture);
			}
		}
		if (UImage* WaveImage = Cast<UImage>(HUDWidget->GetWidgetFromName(FName(*FString::Printf(TEXT("Wave%d"), CurrentWaveIndex)))))
		{
			UTexture2D* Texture = WaveRedTexture.LoadSynchronous();
			WaveImage->SetBrushFromTexture(Texture);
		}

		if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
		{
			WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("WAVE %d/%d"), CurrentWaveIndex, MaxWave)));
		}
	}
}

void AMainPlayerController::UpdateHP(float CurrentHP, float MaxHP)
{
	if (UUserWidget* HUDWidget = HUDWidgetInstance)
	{
		if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CurrentHP"))))
		{
			WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), int32(CurrentHP), int32(MaxHP))));
		}

		if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HPBar"))))
		{
			float HealthPercent = (MaxHP > 0.0f) ? (CurrentHP / MaxHP) : 0.0f;
			HPBar->SetPercent(HealthPercent);
		}
	}
}

void AMainPlayerController::UpdateLevelFinished()
{
	HUDWidgetInstance->RemoveFromParent();
}

void AMainPlayerController::SetHUD()
{
	AT11_GameState* GameState = GetWorld()->GetGameState<AT11_GameState>();

	if (GameState)
	{
		GameState->LevelStarted.AddDynamic(this, &AMainPlayerController::UpdateStageInfo);
		GameState->WaveStarted.AddDynamic(this, &AMainPlayerController::UpdateWaveInfo);
		GameState->MonsterSpawned.AddDynamic(this, &AMainPlayerController::UpdateMonsterCount);
		GameState->MonsterKilled.AddDynamic(this, &AMainPlayerController::UpdateMonsterCount);
		GameState->LevelFinished.AddDynamic(this, &AMainPlayerController::UpdateLevelFinished);
	}
	if (APlayerCharacter* PlayerCharacter =
		Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->FindComponentByClass<UStatComponent>()->OnHPChanged.AddDynamic(this, &AMainPlayerController::UpdateHP);
	}
}
