// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Skills/BaseSkill.h"

void UBaseSkill::Activate()
{
	if (IsValid(SkillData) == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Activate : %s"), *SkillData->SkillName.ToString());

}