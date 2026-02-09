// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/BasicAttack.h"
#include "Engine/Engine.h"

void UBasicAttack::InitFromData()
{
    // 데이터 테이블 기준으로 하면 수정 예정
    // 현재는 수동 입력
    CooldownTime = 0;
    ManaCost = 0;
    Damage = 10;
}

void UBasicAttack::Activate()
{
    // 매직 미사일 구현 예정
    if (IsValid(GEngine))
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("MagicMissile : %d"), Damage));
    }
}