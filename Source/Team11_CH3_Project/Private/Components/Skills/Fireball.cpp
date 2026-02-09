// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/Fireball.h"

void UFireball::InitFromData()
{
    // 데이터 테이블 기준으로 하면 수정 예정
    // 현재는 수동 입력
    CooldownTime = 0;
    ManaCost = 0;
    Damage = 30;
}

void UFireball::Activate()
{
    // 파이어볼 구현 예정
    if (IsValid(GEngine))
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Fireball : %d"), Damage));
    }
}
