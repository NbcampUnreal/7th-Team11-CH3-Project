// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // 충돌 컴포넌트 생성
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    RootComponent = CollisionComponent;

    // 메시 컴포넌트
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 프로젝타일 무브먼트
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    // 초기 수명
    InitialLifeSpan = 3.0f;
}

void ABaseProjectile::Initialize(int32 InDamage, float InSpeed)
{
    Damage = InDamage;
    if (IsValid(ProjectileMovement))
    {
        ProjectileMovement->InitialSpeed = InSpeed;
        ProjectileMovement->MaxSpeed = InSpeed;
    }
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
    // OnHit 델리게이트 바인딩
    if (IsValid(CollisionComponent))
    {
        CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlap);
    }
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ProcessImpact(OtherActor);
}

void ABaseProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
    bool bFromSweep, const FHitResult& SweepResult)
{
    ProcessImpact(OtherActor);
}

void ABaseProjectile::ProcessImpact(AActor* OtherActor)
{
    // 주인 체크
    if (IsValid(GetOwner()) == false)
    {
        Destroy();
        return;
    }
    // 유효성 체크
    if (IsValid(OtherActor) == false || OtherActor == this || OtherActor == GetOwner())
    {
        return;
    }
    // Projectile 끼리 충돌 나지 않게
    if (OtherActor->IsA(ABaseProjectile::StaticClass()))
    {
        return;
    }


    AController* InstigatorController = nullptr;
    if (IsValid(GetInstigator()))
    {
        InstigatorController = GetInstigator()->GetController();
    }

    // 데미지 처리
    UGameplayStatics::ApplyDamage(
        OtherActor,
        Damage,
        InstigatorController,
        this,
        UDamageType::StaticClass()
    );
    UE_LOG(LogTemp, Warning, TEXT("Hit : %s, Damage : %d"), *OtherActor->GetName(), Damage);

    Destroy();
}
