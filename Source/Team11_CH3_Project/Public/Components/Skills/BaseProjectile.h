// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	// 초기화 (데미지, 속도 설정)
	void Initialize(int32 IDamage, float InSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Hit 함수
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);
	// 맞췄을때 처리
	void ProcessImpact(AActor* OtherActor);

	// 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> CollisionComponent;
	// 프로젝타일 무브먼트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
	// 매시 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
	// 데미지
	UPROPERTY()
	int32 Damage;
};
