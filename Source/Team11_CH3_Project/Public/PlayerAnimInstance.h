#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API UPlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPlayerAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    class ABaseCharacter* OwnerCharacter;

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    class UCharacterMovementComponent* MovementComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Direction;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsAccelerating;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    FVector Velocity;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    FVector Acceleration;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsSprinting;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsDodging;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsDead;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bHasWeapon;

    UPROPERTY(BlueprintReadOnly, Category = "Lean")
    float LeanAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lean")
    float LeanInterpSpeed = 6.0f;

private:
    float PreviousYaw;
    void UpdateLean(float DeltaSeconds);
};