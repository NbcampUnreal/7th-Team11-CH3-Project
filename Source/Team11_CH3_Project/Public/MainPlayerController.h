#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TEAM11_CH3_PROJECT_API AMainPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* Skill1Action;
	// 스킬 슬롯. 필요시 2, 3 식으로 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* Consumable1Action;
	// 소모품 슬롯. 필요시 2, 3식으로 추가
	
	// TODO - 추후 gamemode에서 캐릭터 사망 처리 후 직접 호출 방식으로 수정
	void HandlePlayerDeath();

protected:

	// move
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleStopJumping();
	void HandleStartSprint();
	void HandleStopSprint();
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
private:
	FGenericTeamId TeamID;

};
