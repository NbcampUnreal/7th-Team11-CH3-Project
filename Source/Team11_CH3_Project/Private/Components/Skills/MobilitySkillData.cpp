// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/MobilitySkillData.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"


void UMobilitySkillData::Activate(UActiveSkillSlot* InActiveSkillSlot)
{
	Super::Activate(InActiveSkillSlot);
	APawn* Instigator = Cast<APawn>(ActiveSkillSlot->GetOwner());;
	
	// 마법진 소환
	if (IsValid(Instigator) == false)
		return;

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (IsValid(Character) == false)
		return;

	UNiagaraSystem* MagicCircle = GetMagicCircleEffect();
	if (IsValid(MagicCircle) == false)
		return;

	FVector SpawnLocation = Instigator->GetActorLocation();
	SpawnLocation.Z -= 85.f;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		Instigator->GetWorld(),
		MagicCircle,
		SpawnLocation,
		Instigator->GetActorRotation()
	);

	// 플레이어가 바라보는 방향 Yaw가 Z축기준
	FRotator ControlYaw = FRotator(0.f, Instigator->GetControlRotation().Yaw, 0.f);
	FVector ForwardDirection = ControlYaw.Vector();
	// LineTrace를 캐릭터 중심보다 약간 위로 설정해서 바닥이랑 즉시 충돌 방지
	FVector TraceStart = Instigator->GetActorLocation() + FVector(0.f, 0.f, 50.f);
	FVector TraceEnd = TraceStart + (ForwardDirection * Range);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);

	// 캐릭터 정면에서 바라보는 방향으로 장애물 또는 사거리 탐색
	FHitResult HitResult;
	FVector Destination;
	if (Instigator->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	))
	{
		// 장애물 충돌 시 충돌 직전 위치 보정(50cm정도 여유 남기고)
		Destination = HitResult.Location - (ForwardDirection * 50.f);
	}
	else
	{
		// 충돌 없으면 최대 사거리
		Destination = TraceEnd;
	}


	// 바닥 체크해서 위치 보정
	FHitResult GroundHit;
	FVector GroundTraceStart = Destination + FVector(0.f, 0.f, 200.f);
	FVector GroundTraceEnd = Destination - FVector(0.f, 0.f, 500.f);

	if (Instigator->GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		GroundTraceStart,
		GroundTraceEnd,
		ECC_Visibility,
		QueryParams
	))
	{
		// 캐릭터 발이 바닥에 닿도록
		float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		Destination.Z = GroundHit.Location.Z + HalfHeight;
	}
	else
	{
		// 바닥을 못 찾으면 현재 캐릭터 Z 유지
		Destination.Z = Instigator->GetActorLocation().Z;
	}

	Instigator->SetActorLocation(Destination, true);
}
