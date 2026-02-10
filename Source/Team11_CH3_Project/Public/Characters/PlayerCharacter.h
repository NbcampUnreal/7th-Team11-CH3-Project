#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TEAM11_CH3_PROJECT_API APlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

#pragma region Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;
#pragma endregion

#pragma region Input
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* DodgeAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* SprintAction;
#pragma endregion

#pragma region Movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DodgeDistance = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DodgeDuration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DodgeCooldown = 1.0f;
#pragma endregion

    // Camera Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraBoomLength = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraSensitivity = 1.0f;

#pragma region State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDodging;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bCanDodge;
#pragma endregion

    // Dodge / Sprint Animation Setting
    UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
    void OnDodgeStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
    void OnDodgeEnded();

    UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
    void OnSprintStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
    void OnSprintEnded();


    FTimerHandle DodgeTimerHandle;
    FTimerHandle DodgeCooldownTimerHandle;


    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartSprint();
    void StopSprint();
    void PerformDodge();

    // void UpdateMovementSpeed();
    void ExecuteDodge();
    void EndDodge();
    void ResetDodgeCooldown();

    // 추후 퀵슬롯 추가
    void UseQuickSlot();
};