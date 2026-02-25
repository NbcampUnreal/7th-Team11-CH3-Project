// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActiveSkillSlot.generated.h"

class USkillManager;
class USkillDataAsset;
class USkillSlot;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UActiveSkillSlot : public UObject
{
	GENERATED_BODY()
public:	
	void Init(USkillManager* SkillManager);
	void OnStartSkill(USkillSlot* Skill);
	void OnExecute();
	void OnTick(float DeltaSeconds, AActor* Owner);
	void OnExit();
	float GetElapsedTime() const;
	UFUNCTION(BlueprintCallable)
	USkillSlot* GetSkill() const { return  CurrentActiveSkillSlot.Get(); }

	bool GetIsEnd();

private:
	//UPROPERTY()
	//TObjectPtr<USkillDataAsset> CurrentSKill;
	float ElapsedTime;	
	UPROPERTY()
	TWeakObjectPtr<USkillManager> SkillComponent;
	bool bIsEnd;
	// 현재 사용중인 스킬 슬롯데이터
	TWeakObjectPtr<USkillSlot> CurrentActiveSkillSlot;
};
