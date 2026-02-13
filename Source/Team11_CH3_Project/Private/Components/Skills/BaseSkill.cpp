// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Skills/BaseSkill.h"
#include "Camera/CameraComponent.h"
#include "Components/Skills/BaseProjectile.h"

void UBaseSkill::Activate()
{

    // 캐릭터 불러오기 수정 전 주석의 코드는 BasicAttack(깊이 2)과 다른 Skill들(깊이 3)과의 계층 깊이가 달라
    // 동일하게 적용했더니 Fireball의 경우엔 nullptr을 가르켰음
    //AActor* Owner = Cast<AActor>(GetOuter()->GetOuter());
    AActor* Owner = GetTypedOuter<AActor>();
    if (IsValid(Owner) == false)
        return;

    // 캐릭터의 메시 컴포넌트
    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    if (IsValid(Mesh) == false)
        return;

    // 카메라 위치/방향 가져오기(캐릭터의 카메라 컴포넌트)
    UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();
    if (IsValid(Camera) == false)
        return;

    FVector CameraLocation = Camera->GetComponentLocation();
    FVector CameraForWard = Camera->GetForwardVector();

    // 라인트레이스로 타겟 지점 찾기
    FVector TraceEnd = CameraLocation + (CameraForWard * 10000.f); // 10m
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    // LineTrace와 충돌한 위치값 저장 변수
    FVector TargetLocation;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd,
        ECC_Visibility, QueryParams))
    {
        // 카메라 위치에서 카메라 정면으로 LinTrace해서 충돌한 결과 데이터 HitResult에 저장
        TargetLocation = HitResult.Location;
    }
    else
    {
        // 충돌이 없으면 끝지점
        TargetLocation = TraceEnd;
    }

    // 캐릭터의 hand_r 소켓에서 발사를 위해서 가져오기
    FVector SpawnLocation = Mesh->GetSocketLocation(TEXT("hand_r"));
    // 손 -> 타겟 방향 계산
    FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

    // 투사체 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;
    
    SpawnParams.Instigator = Cast<APawn>(Owner);

    ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(
        ProjectileClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (IsValid(Projectile))
    {
        Projectile->Initialize(Damage, ProjectileSpeed);
    }

    UE_LOG(LogTemp, Warning, TEXT("MagicMissile : %d"), Damage);
}