// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActiveSkillSlot.generated.h"

class USkillManager;
class USkillDataAsset;
/**
 * 
 */
UCLASS()
class TEAM11_CH3_PROJECT_API UActiveSkillSlot : public UObject
{
	GENERATED_BODY()
public:	
	void Init(USkillManager* SkillManager);
	void OnStartSkill(USkillDataAsset* Skill);
	void OnExecute();
	void OnTick(float DeltaSeconds, AActor* Owner);
	void OnExit();
	float GetElapsedTime() const;
	UFUNCTION(BlueprintCallable)
	USkillDataAsset* GetSkill() const { return  CurrentSKill; }

	bool GetIsEnd();

private:
	UPROPERTY()
	TObjectPtr<USkillDataAsset> CurrentSKill;
	float ElapsedTime;	
	UPROPERTY()
	TWeakObjectPtr<USkillManager> SkillComponent;
	bool bIsEnd;
};
