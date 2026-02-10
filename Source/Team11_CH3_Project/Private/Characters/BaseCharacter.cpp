#include "Characters/BaseCharacter.h"
#include "Characters/InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    bIsDead = false;

    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseCharacter::ReceiveDamage(float DamageAmount, AActor* DamageCauser)
{
}

void ABaseCharacter::Die()
{
    if (bIsDead) return;

    bIsDead = true;

    if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
    {
        MovementComp->DisableMovement();
    }

    if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
    {
        CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    OnDeath();

}

bool ABaseCharacter::IsAlive() const
{
    return !bIsDead;
}

void ABaseCharacter::HandleDeath()
{
    Die();
}