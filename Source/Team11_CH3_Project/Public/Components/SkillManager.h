// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManager();
	// Called every frame FTimerHandle 사용 예정(프로젝트에서 Tick으로 진행한다면 다시 주석 해제) 
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter
	UFUNCTION(BlueprintCallable)
	UBasicAttack* GetBasicAttack() const { return BasicAttack; }
	UFUNCTION(BlueprintCallable)
	USkillSlot* GetSkillSlot(int32 Index) const { return SkillSlots.IsValidIndex(Index) ? SkillSlots[Index] : nullptr; }

	// 스킬 실행
	UFUNCTION(BlueprintCallable)
	void UseBasicAttack();
	UFUNCTION(BlueprintCallable)
	void UseSkillSlot(int32 Index);
	
	// 스킬 보석 교체
	UFUNCTION(BlueprintCallable)
	void EquipSkillGem(int32 SlotIndex, TSubclassOf<class UBaseSkill> NewSkillClass);

	// 쿨타임 조회
	UFUNCTION(BlueprintCallable)
	bool IsSkillOnCooldown(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable)
	float GetCooldownRemaining(int32 SlotIndex) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// 기본공격 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasicAttack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBasicAttack> BasicAttack;
	// 스킬 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|SkillSlot", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class USkillSlot>> SkillSlots;
	// 스킬 데이터 테이블
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDataTable> SkillDataTable;
	// 쿨다운 관리
	UPROPERTY()
	TMap<int32, FTimerHandle> CooldownTimers;
};
