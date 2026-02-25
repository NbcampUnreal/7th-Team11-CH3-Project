// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Skills/SkillIndicatorActor.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
ASkillIndicatorActor::ASkillIndicatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	RootComponent = NiagaraComponent;
}

// Called when the game starts or when spawned
void ASkillIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
	
	//if (IsValid(IndicatorEffect) == false)
	//	return;
	//if (IsValid(NiagaraComponent) == false)
	//	return;

	//NiagaraComponent->SetAsset(IndicatorEffect);
}

// Called every frame
void ASkillIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillIndicatorActor::Initialize(APawn* InInstigator, float InMaxRange)
{
    OwnerInstigator = InInstigator;
	MaxRange = InMaxRange;
}

void ASkillIndicatorActor::UpdateLocation()
{
    if (OwnerInstigator.IsValid() == false) 
        return;

    UCameraComponent* Camera = OwnerInstigator->FindComponentByClass<UCameraComponent>();
    if (IsValid(Camera) == false) 
        return;

    FVector CameraLocation = Camera->GetComponentLocation();
    FVector CameraForward = Camera->GetForwardVector();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerInstigator.Get());
    QueryParams.AddIgnoredActor(this);

    FHitResult HitResult;
    FVector TraceEnd = CameraLocation + (CameraForward * MaxRange);

    if (OwnerInstigator->GetWorld()->LineTraceSingleByChannel(
        HitResult, 
        CameraLocation, 
        TraceEnd, 
        ECC_Visibility, 
        QueryParams))
    {
        // 바닥 충돌 위치
        IndicatorLocation = HitResult.Location;
    }
    else
    {
        // MaxRange 끝에서 뒤로 당기면서 바닥 있는 위치 탐색
        FHitResult GroundHit;
        bool bFoundGround = false;

        for (float Dist = MaxRange; Dist > 0.f; Dist -= 100.f)
        {
            FVector CheckPoint = CameraLocation + (CameraForward * Dist);
            FVector GroundEnd = CheckPoint - FVector(0.f, 0.f, 5000.f);

            if (OwnerInstigator->GetWorld()->LineTraceSingleByChannel(
                GroundHit, 
                CheckPoint, 
                GroundEnd, 
                ECC_Visibility, 
                QueryParams))
            {
                IndicatorLocation = GroundHit.Location;
                bFoundGround = true;
                break;
            }
        }

        if (bFoundGround == false) 
            return;
    }

    SetActorLocation(IndicatorLocation);
}





