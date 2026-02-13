// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/Fireball.h"
#include "Components/Skills/BaseProjectile.h"
#include "UObject/ConstructorHelpers.h"

UFireball::UFireball()
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

void UFireball::Activate()
{
    Super::Activate();
    UE_LOG(LogTemp, Warning, TEXT("Fire"));
}

void UFireball::InitFromData()
{
    // 데이터 테이블 기준으로 하면 수정 예정
    // 현재는 수동 입력
    CooldownTime = 5;
    ManaCost = 5.f;
    Damage = 30;
    ProjectileSpeed = 3000.f;
}
