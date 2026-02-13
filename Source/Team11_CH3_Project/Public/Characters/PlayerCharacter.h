#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/Skills/BasicAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/StatComponent.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UBuffManager;
class UStatComponent;
// class AWeapon; // 일단 전방 선언

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeEnded);

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

#pragma region Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraBoomLength = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraSensitivity = 1.0f;
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

#pragma region State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDodging;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bCanDodge;

    UPROPERTY(BlueprintAssignable, Category = "State")
    FOnSprintStarted OnSprintStartedEvent;

    UPROPERTY(BlueprintAssignable, Category = "State")
    FOnSprintEnded OnSprintEndedEvent;
#pragma endregion

#pragma region Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<UStatComponent> StatComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<UBuffManager> BuffManager;
#pragma endregion

#pragma region Input
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> AttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> SkillQAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> SkillEAction;
#pragma endregion

    /*
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> WeaponClass;
    */

    UPROPERTY()
    TObjectPtr<UBasicAttack> BasicAttack;

    FTimerHandle DodgeTimerHandle;
    FTimerHandle DodgeCooldownTimerHandle;

#pragma region Movement
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Move(const FVector2D& MovementVector);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Look(const FVector2D& LookAxisVector);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void PerformDodge();
#pragma endregion

#pragma region Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TObjectPtr<UAnimMontage> HitMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TObjectPtr<UAnimMontage> DeathMontage;

    // montage로 할지 animation으로 할지 고려
    /*
    UFUNCTION(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    void PlayDodgeAnimation();

    UFUNCTION(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    void PlayDeathAnimation();
    */
#pragma endregion

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void ReceiveDamage(float Damage, AActor* DamageCauser) override;
    virtual void Die() override;

    // void AttachWeapon(TSubclassOf<AActor> WeaponClass);

protected:

    void UpdateMovementSpeed();
    void ExecuteDodge();
    void EndDodge();
    void ResetDodgeCooldown();

    void Hit();
    
    void Attack(const FInputActionValue& Value);
    void SkillQ(const FInputActionValue& Value);
    void SkillE(const FInputActionValue& Value);

private:
    bool bIsDead = false;
    
};