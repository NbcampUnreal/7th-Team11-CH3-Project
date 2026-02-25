// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManager.generated.h"


class UActiveSkillSlot;
class UStatComponent;
class USkillSlot;
class USkillDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManager();
	// Called every frame FTimerHandle 사용 예정(프로젝트에서 Tick으로 진행한다면 다시 주석 해제) 
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Getter

	UFUNCTION(BlueprintCallable)
	USkillSlot* GetSkillSlot(int32 Index) const { return SkillSlots.IsValidIndex(Index) ? SkillSlots[Index] : nullptr; }
	UFUNCTION(BlueprintCallable)
	TArray<int32> FindReadySlotIndexes()const;	
	UFUNCTION(BlueprintCallable)
	int32 GetBestSkill(AActor* Actor, AActor* Target)const;

	// 스킬 실행
	UFUNCTION(BlueprintCallable)
	void StartSkillCooldown(int32 Index);
	
	// 스킬 보석 교체
	UFUNCTION(BlueprintCallable)
	void EquipSkillGem(int32 SlotIndex, USkillDataAsset* NewSkillData);
	UFUNCTION(BlueprintCallable)
	void AddSKillGems(TArray<TSoftObjectPtr<USkillDataAsset>> Skills);	
	// 쿨타임 조회
	UFUNCTION(BlueprintCallable)
	bool IsSkillOnCooldown(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable)
	float GetCooldownRemaining(int32 SlotIndex) const;

	void Clear();
	
	UActiveSkillSlot* GetActiveSkillSlot() const;
	void ActiveSkill(USkillSlot* CurrentSlot);
	void TickActiveSkill(float DeltaSeconds, AActor* Owner);
	void ExecuteActiveSkill();
	void ExitActiveSkill();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// 스킬 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill|SkillSlot", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkillSlot>> SkillSlots;
	// 초기 스킬 클래스(블루프린트 설정용)
	// 스킬 데이터 테이블(도전 기능때 구현)
	//UPROPERTY(EditDefaultsOnly, Category = "Skill|Data", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UDataTable> SkillDataTable;
	// 스탯 연동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UStatComponent> StatComp;
	// 쿨다운 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UActiveSkillSlot> ActiveSkillSlot;
};
