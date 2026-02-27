#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "Types/StatTypes.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UCanvasPanel;

UCLASS()
class TEAM11_CH3_PROJECT_API AMainPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> Skill1Action;
	// 스킬 슬롯. 필요시 2, 3 식으로 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> Consumable1Action;
	// 소모품 슬롯. 필요시 2, 3식으로 추가
	
	// TODO - 추후 gamemode에서 캐릭터 사망 처리 후 직접 호출 방식으로 수정
	void HandlePlayerDeath();

	// 조준점 월드 타겟
	UFUNCTION(BlueprintCallable)
	bool GetAimPoint(FVector& OutAimPoint) const;

	// 조준시 조준점 토글
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetCrosshairVisible(bool bVisible);

protected:

	// move
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleStopJumping();
	void HandleDodge();

	// camera
	void Turn(float Value);
	void LookUp(float Value);

	// inventory/skill
	void HandleOpenInventory();

	void HandleUseSkill1();
	void HandleUseConsumable1();

	void UseSkillSlot(int32 SlotIndex);
	void UseConsumableSlot(int32 SlotIndex);

	// aim move
	void AimPressed();
	void AimReleased();

	bool bHUDShown = false;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag HUDRequestTag;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loading")
	UUserWidget* LoadingWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	FTimerHandle FadeTimerHandle;
	float CurrentOpacity = 1.0f;
	bool bFadingIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading")
	UCanvasPanel* MainCanvas;
	void LoadingToHUD();
	UFUNCTION(Category = "Loading")
	void UpdateFade();
	UFUNCTION(BlueprintPure, Category = "Loading")
	UUserWidget* GetLoadingWidget() const;

	UFUNCTION()
	void UpdateMonsterCount(int32 MonsterCount);
	UFUNCTION()
	void UpdateStageInfo(int32 MaxWave);
	UFUNCTION()
	void UpdateWaveInfo(int32 WaveIndex, int32 MaxWave);
	UFUNCTION()
	void UpdateHP(float CurrentHP, float MaxHP);
	UFUNCTION()
	void UpdateLevelFinished();

	void SetHUD();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> WaveYellowTexture;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> WaveRedTexture;
private:

	FGenericTeamId TeamID;

};
