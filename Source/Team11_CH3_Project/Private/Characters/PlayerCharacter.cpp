#include "Characters/PlayerCharacter.h"
#include "Characters/InventoryComponent.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "WeaponActor.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatComponent.h"
#include "Components/BuffManager.h"
#include "Components/SkillManager.h"
#include "Components/ItemManager.h"
#include "Components/Skills/SkillSlot.h"
#include "Core/T11_GameInstance.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = CameraBoomLength;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = !bIsAiming;
    CameraBoom->CameraLagSpeed = 20.0f;
    CameraBoom->CameraLagMaxDistance = 0.f;
    CameraBoom->bEnableCameraRotationLag = false;
    CameraBoom->CameraRotationLagSpeed = 20.f;
    CameraBoom->bUseCameraLagSubstepping = true;
    CameraBoom->CameraLagMaxTimeStep = 1.f / 60.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 캡슐과 카메라 사이 충돌 비활성화
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
	                                                     ECollisionResponse::ECR_Ignore);
	// 매쉬와 카메라 사이 충돌 비활성화
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
    BuffManager = CreateDefaultSubobject<UBuffManager>(TEXT("BuffManager"));
	SkillComponent = CreateDefaultSubobject<USkillManager>("SkillComponent");
	ItemManager = CreateDefaultSubobject<UItemManager>(TEXT("ItemManager"));

    bIsDodging = false;
    bCanDodge = true;

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(
			AttackAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerCharacter::Attack);

		EnhancedInput->BindAction(
			SkillQAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerCharacter::SkillQ);

		EnhancedInput->BindAction(
			SkillEAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerCharacter::SkillE);
	}
}

void APlayerCharacter::GetSkillTargetLocation(FVector& TargetLocation)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		int32 SizeX, SizeY;
		PlayerController->GetViewportSize(SizeX, SizeY);
		FVector2D ScreenCenter(SizeX / 2, SizeY / 2);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = false;
		Params.AddIgnoredActor(WeaponActor);
		FHitResult HitResult;
		if (PlayerController->GetHitResultAtScreenPosition(ScreenCenter, ECC_Camera, Params, HitResult))
		{
			TargetLocation = HitResult.ImpactPoint;
		}
		else
		{
			FVector LookDir;
			FVector StartPos;
			PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, StartPos, LookDir);
			TargetLocation = StartPos + (LookDir * 10000.0f);
		}
	}
	else
	{
		TargetLocation = GetActorForwardVector() * 1000.0f + GetActorLocation();
	}
}

void APlayerCharacter::DealDamage()
{
	if (IsValid(this) && WeaponActor)
	{
		WeaponActor->PerformDamage();
	}
}

// 스킬 정의
void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (IsValid(SkillComponent->GetSkillSlot(0)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Empty!"));
		return;
	}

	if (SkillComponent->IsSkillOnCooldown(0))
	{
		return;
	}
	if (!IsValid(WeaponActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Empty!"));
		return;
	}

	// if (SkillComponent->CurrentActiveSkill) 체크로 지금 활성화된 스킬이 있으면 그거 사용 아니면 넘겨서 아래 함수로 넘어감 
	// 처음 등록의 skillmanager의 enter함수 Tick 다시 누르면 Excute함수 쓰면서 스킬 발동 발동 끝나는 시점에 Exit
	FVector TargetLocation;
	GetSkillTargetLocation(TargetLocation);
	PerformAttack(SkillComponent->GetSkillSlot(0), TargetLocation);
}

void APlayerCharacter::SkillQ(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Q Used"));

	if (IsValid(SkillComponent->GetSkillSlot(1)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Empty!"));
		return;
	}

	if (SkillComponent->IsSkillOnCooldown(1))
	{
		UE_LOG(LogTemp, Warning, TEXT("CoolTime Remaining : %0.1f"), SkillComponent->GetCooldownRemaining(1));
		return;
	}
	if (!IsValid(WeaponActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Empty!"));
		return;
	}


	// if (SkillComponent->CurrentActiveSkill) 체크로 지금 활성화된 스킬이 있으면 return 


	FVector TargetLocation;
	GetSkillTargetLocation(TargetLocation);
	PerformAttack(SkillComponent->GetSkillSlot(1), TargetLocation);
}

void APlayerCharacter::SkillE(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill E Used"));

	if (IsValid(SkillComponent->GetSkillSlot(2)) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Empty!"));
		return;
	}

	if (SkillComponent->IsSkillOnCooldown(2))
	{
		UE_LOG(LogTemp, Warning, TEXT("CoolTime Remaining : %0.1f"), SkillComponent->GetCooldownRemaining(2));
		return;
	}
	if (!IsValid(WeaponActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Empty!"));
		return;
	}

	FVector TargetLocation;
	GetSkillTargetLocation(TargetLocation);
	PerformAttack(SkillComponent->GetSkillSlot(2), TargetLocation);
}

void APlayerCharacter::SetAiming(bool bNewAiming)
{
	UE_LOG(LogTemp, Warning, TEXT("SetAiming: %d"), bNewAiming);
	bIsAiming = bNewAiming;

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : DefaultWalkSpeed;
	UpdateMovementSpeed();

    if (AMainPlayerController* PC = Cast<AMainPlayerController>(GetController()))
    {
        PC->BP_SetCrosshairVisible(bIsAiming);
    }

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 스탯 변경 시 이동속도 업데이트
	StatComponent->OnStatChanged.AddDynamic(this, &APlayerCharacter::UpdateMovementSpeed);

	UT11_GameInstance* GI = Cast<UT11_GameInstance>(GetGameInstance());
	if (IsValid(GI) == false)
		return;

	if (GI->HasSavedData())
	{
		GI->RestorePlayerData(StatComponent, ItemManager, SkillComponent);
	}
	else
	{
		// 초기 스탯 설정
		FStatData InitialStat;
		InitialStat.MaxHP = 1000.f;
		InitialStat.DEF = 10.f;
		InitialStat.MoveSpeed = 0.f;
		InitialStat.AttackDamage = 20.f;
		InitialStat.CastSpeed = 1.f;
		InitialStat.ProjectileSpeed = 500.f;
		InitialStat.CriticalChance = 10.f;
		InitialStat.CriticalDamage = 1.5f;
		StatComponent->InitStat(InitialStat);

        // 기본 장비 장착(시작은 기본 무기만)
        ItemManager->UseItem(TEXT("StaffWeapon"), EItemType::Equipment, 0);
    }

    UpdateMovementSpeed();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    const FVector TargetOffset = bIsAiming ? AimSocketOffset : DefaultSocketOffset;
    CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, CameraInterpSpeed);
    /*
    float TargetArm = bIsAiming ? AimArmLength : DefaultArmLength;
    FVector TargetOffset = bIsAiming ? AimSocketOffset : DefaultSocketOffset;

    CameraBoom->TargetArmLength =
        FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArm, DeltaTime, CameraInterpSpeed);

    CameraBoom->SocketOffset =
        FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, CameraInterpSpeed);
        */
}


void APlayerCharacter::Move(const FVector2D& MovementVector)
{
	if (bIsDodging) return;

	if (!Controller) return;

    SmoothedMoveInput = FMath::Vector2DInterpTo(
        SmoothedMoveInput,
        MovementVector,
        GetWorld()->GetDeltaSeconds(),
        MoveInputSmoothSpeed
    );

    const float X = SmoothedMoveInput.X;
    const float Y = SmoothedMoveInput.Y;

    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDir, Y);
    AddMovementInput(RightDir, X);

    /*
    FRotator ControlRotation = Controller->GetControlRotation();
    FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

    FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(Forward, MovementVector.Y);
    AddMovementInput(Right, MovementVector.X);
    */
}

void APlayerCharacter::Look(const FVector2D& LookAxisVector)
{
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * CameraSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * CameraSensitivity);
	}
}

void APlayerCharacter::PerformDodge()
{
	if (!bCanDodge || bIsDodging) return;

	ExecuteDodge();
}

void APlayerCharacter::UpdateMovementSpeed()
{
    if (!GetCharacterMovement() || !StatComponent) return;
    // 조준상태일때도 고려해서 최고속도 업데이트
    GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : WalkSpeed + StatComponent->GetCurrentStat(EStat::MoveSpeed);
}

EDodgeDir APlayerCharacter::GetDodgeDirectionFromInput() const
{
	FVector InputDir = GetLastMovementInputVector();

	if (InputDir.IsNearlyZero())
	{
		return EDodgeDir::Forward;
	}

	InputDir.Z = 0.f;
	InputDir = InputDir.GetSafeNormal();

	const float ForwardDot = FVector::DotProduct(InputDir, GetActorForwardVector());
	const float RightDot = FVector::DotProduct(InputDir, GetActorRightVector());

	if (FMath::Abs(ForwardDot) >= FMath::Abs(RightDot))
	{
		return (ForwardDot >= 0.f) ? EDodgeDir::Forward : EDodgeDir::Backward;
	}
	else
	{
		return (RightDot >= 0.f) ? EDodgeDir::Right : EDodgeDir::Left;
	}
}

void APlayerCharacter::PlayDodgeMontage(EDodgeDir Dir)
{
	UAnimMontage* MontageToPlay = nullptr;

	switch (Dir)
	{
	case EDodgeDir::Forward: MontageToPlay = DodgeMontage_F;
		break;
	case EDodgeDir::Backward: MontageToPlay = DodgeMontage_B;
		break;
	case EDodgeDir::Left: MontageToPlay = DodgeMontage_L;
		break;
	case EDodgeDir::Right: MontageToPlay = DodgeMontage_R;
		break;
	default: break;
	}

	if (!MontageToPlay) return;

	if (UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		Anim->Montage_Play(MontageToPlay, 1.0f);
	}
}

void APlayerCharacter::ExecuteDodge()
{
	if (!bCanDodge) return;

	bIsDodging = true;
	bCanDodge = false;

	const EDodgeDir Dir = GetDodgeDirectionFromInput();
	PlayDodgeMontage(Dir);

	/*
	UAnimMontage* M = DodgeMontage_F; // 테스트
	UE_LOG(LogTemp, Warning, TEXT("[Dodge] Montage=%s"), *GetNameSafe(M));

	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
	    const float Len = Anim->Montage_Play(M, 1.0f);
	    UE_LOG(LogTemp, Warning, TEXT("[Dodge] Len=%.3f AnimClass=%s"),
	        Len, *GetNameSafe(GetMesh()->GetAnimClass()));
	}
	else
	{
	    UE_LOG(LogTemp, Warning, TEXT("[Dodge] AnimInstance NULL"));
	}
	*/


	DodgeDir = GetLastMovementInputVector();
	if (DodgeDir.IsNearlyZero()) DodgeDir = GetVelocity();
	if (DodgeDir.IsNearlyZero()) DodgeDir = GetActorForwardVector();

	DodgeDir.Z = 0.f;
	DodgeDir = DodgeDir.GetSafeNormal();

	DodgeRemainingDist = DodgeDistance * DodgeDistanceScale;

	auto* Move = GetCharacterMovement();

	if (CameraBoom)
	{
		bSavedEnableCameraLag = CameraBoom->bEnableCameraLag;
		SavedCameraLagSpeed = CameraBoom->CameraLagSpeed;

		CameraBoom->bEnableCameraLag = true;
		CameraBoom->CameraLagSpeed = DodgeCameraLagSpeed;
	}

	// 마찰 저장
	SavedGroundFriction = Move->GroundFriction;
	SavedBrakingFrictionFactor = Move->BrakingFrictionFactor;
	SavedBrakingDecel = Move->BrakingDecelerationWalking;

	// 드르륵 방지
	Move->GroundFriction = 0.f;
	Move->BrakingFrictionFactor = 0.f;
	Move->BrakingDecelerationWalking = 0.f;

	Move->StopMovementImmediately();

	DodgeLastTimeSec = GetWorld()->GetTimeSeconds();

	GetWorldTimerManager().SetTimer(
		DodgeTickHandle,
		this,
		&APlayerCharacter::DodgeStep,
		0.005f,
		true);

	// 닷지 종료
	GetWorldTimerManager().SetTimer(
		DodgeTimerHandle,
		this,
		&APlayerCharacter::EndDodge,
		DodgeDuration,
		false);

	// 쿨타임
	GetWorldTimerManager().SetTimer(
		DodgeCooldownTimerHandle,
		this,
		&APlayerCharacter::ResetDodgeCooldown,
		DodgeCooldown,
		false);
}

void APlayerCharacter::DodgeStep()
{
	if (!bIsDodging)
		return;

	const double Now = GetWorld()->GetTimeSeconds();
	const float DeltaSeconds = float(Now - DodgeLastTimeSec);
	DodgeLastTimeSec = Now;

	if (DeltaSeconds <= 0.f)
		return;

	const float TotalDist = DodgeDistance * DodgeDistanceScale;
	const float Speed = TotalDist / DodgeDuration;

	float StepDist = Speed * DeltaSeconds;
	StepDist = FMath::Min(StepDist, DodgeRemainingDist);

	FVector Delta = DodgeDir * StepDist;

	FHitResult Hit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(
		Delta,
		GetActorRotation(),
		true,
		Hit
	);

	DodgeRemainingDist -= StepDist;

	// 벽에 막히거나 거리 끝
	if (DodgeRemainingDist <= KINDA_SMALL_NUMBER || Hit.bBlockingHit)
	{
		EndDodge();
	}
}

void APlayerCharacter::EndDodge()
{
	if (!bIsDodging)
		return;

	bIsDodging = false;

	GetWorldTimerManager().ClearTimer(DodgeTickHandle);

	auto* Move = GetCharacterMovement();

	// 카메라 복구
	if (CameraBoom)
	{
		CameraBoom->bEnableCameraLag = bSavedEnableCameraLag;
		CameraBoom->CameraLagSpeed = SavedCameraLagSpeed;
	}

	// 마찰 복구
	Move->GroundFriction = SavedGroundFriction;
	Move->BrakingFrictionFactor = SavedBrakingFrictionFactor;
	Move->BrakingDecelerationWalking = SavedBrakingDecel;
}


void APlayerCharacter::ResetDodgeCooldown()
{
	bCanDodge = true;
}

// TakeDamage로 변경
float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
    class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!StatComponent || bIsDead)
		return 0.0f;

	const bool bDied = StatComponent->TakeDamage(ActualDamage);

	if (bDied)
	{
		//    Die();
	}
	else
	{
		Hit();
	}
	return ActualDamage;
}

void APlayerCharacter::Hit()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Hit"));

	if (HitMontage)
	{
		PlayAnimMontage(HitMontage);
	}
}

void APlayerCharacter::Die()
{
	Super::Die();

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}

	if (AMainPlayerController* PC =
		Cast<AMainPlayerController>(GetController()))
	{
		PC->HandlePlayerDeath();
	}
}

void APlayerCharacter::SetWeaponActor(AWeaponActor* NewWeapon)
{
    if (WeaponActor)
    {
        WeaponActor->Destroy();
    } 

    WeaponActor = NewWeapon;
    SkillComponent->EquipSkillGem(0,NewWeapon->GetDefaultSkillData());
		
}

// dodge, death anim
/*
void APlayerCharacter::PlayDodgeAnimation()
{
    if (DodgeMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(DodgeMontage);
    }
}

void APlayerCharacter::PlayDeathAnimation()
{
    if (DeathMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
    }
}
*/
void APlayerCharacter::PerformAttack(USkillSlot* SkillSlot, const FVector& TargetLocation)
{
	//	SkillComponent
	if (WeaponActor->IsAttacking())
	{
		return;
	}
	// Q,E 스킬 없으면 터지는거 방지
	if (IsValid(SkillSlot->GetEquippedSkill()) == false)
		return;
	if (IsValid(SkillSlot->GetEquippedSkill()->GetSkillMontage()) == false)
		return;

	UAnimMontage* SkillMontage = SkillSlot->GetEquippedSkill()->GetSkillMontage();

	PlayAnimMontage(SkillMontage, 1,TEXT("UpperBody"));

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APlayerCharacter::OnAttackMontageEnded);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, SkillMontage);
	WeaponActor->StartAttack(TargetLocation,
	                         SkillSlot);
}


void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (WeaponActor)
	{
		WeaponActor->EndAttack();
	}
}
