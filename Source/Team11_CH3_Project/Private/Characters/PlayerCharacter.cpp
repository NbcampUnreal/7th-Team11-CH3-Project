#include "Characters/PlayerCharacter.h"
#include "Characters/InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/StatComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = CameraBoomLength;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 3.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    bIsSprinting = false;
    bIsDodging = false;
    bCanDodge = true;

}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    UpdateMovementSpeed();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter::Move(const FVector2D& MovementVector)
{
    if (bIsDodging) return;

    if (Controller != nullptr && MovementVector.SizeSquared() > 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Look(const FVector2D& LookAxisVector)
{
    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X * CameraSensitivity);
        AddControllerPitchInput(LookAxisVector.Y * CameraSensitivity);
    }
}

void APlayerCharacter::StartSprint()
{
    if (bIsDodging) return;

    bIsSprinting = true;
    UpdateMovementSpeed();
    BP_OnSprintStarted();
}

void APlayerCharacter::StopSprint()
{
    bIsSprinting = false;
    UpdateMovementSpeed();
    BP_OnSprintEnded();
}

void APlayerCharacter::PerformDodge()
{
    if (!bCanDodge || bIsDodging) return;

    ExecuteDodge();
}

void APlayerCharacter::UpdateMovementSpeed()
{
    if (!GetCharacterMovement()) return;

    if (bIsSprinting)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}


void APlayerCharacter::ExecuteDodge()
{
    bIsDodging = true;
    bCanDodge = false;

    if (bIsSprinting)
    {
        StopSprint();
    }

    PlayDodgeAnimation();

    FVector DodgeDirection = GetVelocity().GetSafeNormal();

    if (DodgeDirection.IsNearlyZero())
    {
        DodgeDirection = GetActorForwardVector();
    }

    FVector LaunchVelocity = DodgeDirection * (DodgeDistance / DodgeDuration);
    LaunchCharacter(LaunchVelocity, true, true);

    BP_OnDodgeStarted();

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
    BP_OnDodgeEnded();
}

void APlayerCharacter::ResetDodgeCooldown()
{
    bCanDodge = true;
}

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

