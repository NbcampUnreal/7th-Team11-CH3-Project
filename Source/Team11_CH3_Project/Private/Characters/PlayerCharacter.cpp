#include "Characters/PlayerCharacter.h"
#include "Characters/InventoryComponent.h"
#include "Components/SkillManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // Camera arm
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = CameraBoomLength;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 3.0f;

    // Follow Camera 설정
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // 캐릭터 회전 설정
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // CharacterMovement 설정
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // SkillManager 생성
    SkillManagerComponent = CreateDefaultSubobject<USkillManager>(TEXT("SkillManager"));

    // State 초기화
    bIsSprinting = false;
    bIsDodging = false;
    bCanDodge = true;
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input 설정
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // 초기 이동 속도 설정
    // UpdateMovementSpeed();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

        EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::PerformDodge);

        // 스킬 입력 바인딩
        if (BasicAttackAction)
        {
            EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::UseBasicAttack);
        }
        if (SkillSlot1Action)
        {
            EnhancedInputComponent->BindAction(SkillSlot1Action, ETriggerEvent::Started, this, &APlayerCharacter::UseSkillSlot1);
        }
        if (SkillSlot2Action)
        {
            EnhancedInputComponent->BindAction(SkillSlot2Action, ETriggerEvent::Started, this, &APlayerCharacter::UseSkillSlot2);
        }
        if (SkillSlot3Action)
        {
            EnhancedInputComponent->BindAction(SkillSlot3Action, ETriggerEvent::Started, this, &APlayerCharacter::UseSkillSlot3);
        }
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    if (bIsDodging) return;

    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X * CameraSensitivity);
        AddControllerPitchInput(LookAxisVector.Y * CameraSensitivity);
    }
}

void APlayerCharacter::StartSprint()
{
    bIsSprinting = true;
    // UpdateMovementSpeed();
}

void APlayerCharacter::StopSprint()
{
    bIsSprinting = false;
    // UpdateMovementSpeed();
}

void APlayerCharacter::PerformDodge()
{
    if (!bCanDodge || bIsDodging) return;

    ExecuteDodge();
}

// stat 업데이트 시 활성화
/*void APlayerCharacter::UpdateMovementSpeed()
{
    if (!StatComponent) return;

    float BaseSpeed = StatComponent->GetMoveSpeed();
    float TargetSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;

    if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
    {
        MovementComp->MaxWalkSpeed = TargetSpeed;
    }
}*/

void APlayerCharacter::ExecuteDodge()
{
    bIsDodging = true;
    bCanDodge = false;

    FVector DodgeDirection = GetVelocity().GetSafeNormal();

    if (DodgeDirection.IsNearlyZero())
    {
        DodgeDirection = GetActorForwardVector();
    }

    FVector LaunchVelocity = DodgeDirection * (DodgeDistance / DodgeDuration);
    LaunchCharacter(LaunchVelocity, true, true);


    GetWorldTimerManager().SetTimer(
        DodgeTimerHandle,
        this,
        &APlayerCharacter::EndDodge,
        DodgeDuration,
        false
    );

    GetWorldTimerManager().SetTimer(
        DodgeCooldownTimerHandle,
        this,
        &APlayerCharacter::ResetDodgeCooldown,
        DodgeCooldown,
        false
    );
}

void APlayerCharacter::EndDodge()
{
    bIsDodging = false;
}

void APlayerCharacter::ResetDodgeCooldown()
{
    bCanDodge = true;
}

void APlayerCharacter::UseBasicAttack()
{
    if (IsValid(SkillManagerComponent))
    {
        SkillManagerComponent->UseBasicAttack();
    }
}

void APlayerCharacter::UseSkillSlot1()
{
    if (IsValid(SkillManagerComponent))
    {
        SkillManagerComponent->UseSkillSlot(0);
    }
}

void APlayerCharacter::UseSkillSlot2()
{
    if (IsValid(SkillManagerComponent))
    {
        SkillManagerComponent->UseSkillSlot(1);
    }
}

void APlayerCharacter::UseSkillSlot3()
{
    if (IsValid(SkillManagerComponent))
    {
        SkillManagerComponent->UseSkillSlot(2);
    }
}
