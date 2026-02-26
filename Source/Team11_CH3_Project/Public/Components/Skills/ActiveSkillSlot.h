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
	void OnStartSkill(AActor* InOwner, const FVector& InTargetLocation, USkillSlot* SkillSlot);
	void OnExecute();
	void OnTick(float DeltaSeconds);
	void OnExit();
	void Notify(FName NotifyName);
	float GetElapsedTime() const;
	UFUNCTION(BlueprintCallable)
	USkillSlot* GetSkillSlot() const { return  CurrentActiveSkillSlot.Get(); }

	bool GetIsEnd();
	void SetIsEnd(bool isEnd);

private:
	TWeakObjectPtr<AActor> Owner;
	FVector TargetLocation;
	float ElapsedTime;	
	UPROPERTY()
	TWeakObjectPtr<USkillManager> SkillComponent;
	bool bIsEnd;
	// 현재 사용중인 스킬 슬롯데이터
	TWeakObjectPtr<USkillSlot> CurrentActiveSkillSlot;
};
