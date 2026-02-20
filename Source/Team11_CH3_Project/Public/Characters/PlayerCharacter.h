#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/StatComponent.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AWeaponActor;
class USkillManager;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UBuffManager;
class UStatComponent;
class UItemManager;
// class AWeapon; // 일단 전방 선언

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeEnded);

UCLASS()
class TEAM11_CH3_PROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
#pragma region TESTCODE
	FWeaponItemData WeaponItemData;
#pragma region
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

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    FVector DefaultSocketOffset = FVector(0.f, 0.f, 0.f);

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    FVector AimSocketOffset = FVector(0.f, 50.f, 15.f);

    UPROPERTY(EditAnywhere, Category = "Camera|Dodge")
    float DodgeCameraLagSpeed = 15.f;   // 닷지 중 랙 스피드

    UPROPERTY(EditAnywhere, Category = "Camera|Dodge")
    bool bDisableCameraLagWhileDodging = false;


    bool bSavedEnableCameraLag = false;

    float SavedCameraLagSpeed = 0.f;
    float CameraInterpSpeed = 10.f;
#pragma endregion

#pragma region Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 400.0f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DodgeDistance = 1800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DodgeDuration = 0.5f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DodgeCooldown = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float DodgeDistanceScale = 0.4f;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UItemManager> ItemManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillManager> SkillComponent;
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

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    bool bIsAiming;

    // 조준 상태 변경
    void SetAiming(bool bNewAiming);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() override;

	void SetWeaponActor(AWeaponActor* NewWeapon) { WeaponActor = NewWeapon; }

	// void AttachWeapon(TSubclassOf<AActor> WeaponClass);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeaponActor> WeaponActor;

protected:
	void UpdateMovementSpeed();
	void ExecuteDodge();
	void EndDodge();
	void ResetDodgeCooldown();
    // void AttachWeapon(TSubclassOf<AActor> WeaponClass);

    // 쏘아져나가는 것 방지용 닷지 시작위치 저장, alpha 보간, deltasecond 사용
    FTimerHandle DodgeTickHandle;
    FVector DodgeStartLoc;
    FVector DodgeDir = FVector::ZeroVector;
    float DodgeElapsed = 0.f;
    double DodgeLastTimeSec = 0.0;
    float DodgeRemainingDist = 0.f;

	void Hit();


protected:

    void Attack(const FInputActionValue& Value);
    void SkillQ(const FInputActionValue& Value);
    void SkillE(const FInputActionValue& Value);

    void DodgeStep();

private:
    bool bIsDead = false;

    float DefaultWalkSpeed = 600.f;
    float AimWalkSpeed = 300.f;

    float DefaultArmLength = 300.f;
    float AimArmLength = 200.f;

    float SavedGroundFriction = 0.f;
    float SavedBrakingFrictionFactor = 0.f;
    float SavedBrakingDecel = 0.f;
};
