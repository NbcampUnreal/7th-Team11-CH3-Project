// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/BasicAttack.h"
#include "Engine/Engine.h"
#include "Components/Skills/BaseProjectile.h"
#include "UObject/ConstructorHelpers.h"

UBasicAttack::UBasicAttack()
{
    // 테스트용으로 임시 프로젝타일 할당
    // 추후에는 장착한 Skill Gem에 지정된 프로젝타일을 가져와서 사용
    static ConstructorHelpers::FClassFinder<ABaseProjectile> ProjectileBP(
        TEXT("/Game/Character/Blueprints/BP_Projectile")
    );
    if (ProjectileBP.Succeeded())
    {
        ProjectileClass = ProjectileBP.Class;
    }
    InitFromData();
}

void UBasicAttack::InitFromData()
{
    // 데이터 테이블 기준으로 하면 수정 예정
    // 현재는 수동 입력
    CooldownTime = 0;
    ManaCost = 0;
    Damage = 10;
}