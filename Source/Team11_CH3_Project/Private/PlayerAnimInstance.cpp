#include "PlayerAnimInstance.h"
#include "Characters/BaseCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
    Speed = 0.0f;
    Direction = 0.0f;
    bIsInAir = false;
    bIsAccelerating = false;
    bIsSprinting = false;
    bIsDodging = false;
    bIsDead = false;
    bHasWeapon = false;
    LeanAmount = 0.0f;
    PreviousYaw = 0.0f;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwnerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
    if (OwnerCharacter)
    {
        MovementComponent = OwnerCharacter->GetCharacterMovement();
    }
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerCharacter || !MovementComponent)
    {
        return;
    }

    Velocity = MovementComponent->Velocity;
    Speed = Velocity.Size2D();
    Acceleration = MovementComponent->GetCurrentAcceleration();
    bIsAccelerating = Acceleration.SizeSquared() > 0.0f;
    bIsInAir = MovementComponent->IsFalling();

    if (Speed > 0.0f)
    {
        FRotator VelocityRotation = Velocity.Rotation();
        FRotator ActorRotation = OwnerCharacter->GetActorRotation();
        Direction = UKismetMathLibrary::NormalizedDeltaRotator(VelocityRotation, ActorRotation).Yaw;
    }
    else
    {
        Direction = 0.0f;
    }

    bIsDead = !OwnerCharacter->IsAlive();

    if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OwnerCharacter))
    {
        bIsSprinting = PlayerChar->bIsSprinting;
        bIsDodging = PlayerChar->bIsDodging;
    }

    if (OwnerCharacter->GetInventoryComponent())
    {
        bHasWeapon = OwnerCharacter->GetInventoryComponent()->HasWeaponEquipped();
    }

    UpdateLean(DeltaSeconds);
}

void UPlayerAnimInstance::UpdateLean(float DeltaSeconds)
{
    if (!OwnerCharacter) return;

    float CurrentYaw = OwnerCharacter->GetActorRotation().Yaw;

    float YawDelta = UKismetMathLibrary::NormalizedDeltaRotator(
        FRotator(0, CurrentYaw, 0),
        FRotator(0, PreviousYaw, 0)
    ).Yaw;

    float TargetLean = FMath::Clamp(YawDelta / DeltaSeconds, -15.0f, 15.0f);

    LeanAmount = FMath::FInterpTo(LeanAmount, TargetLean, DeltaSeconds, LeanInterpSpeed);

    PreviousYaw = CurrentYaw;
}