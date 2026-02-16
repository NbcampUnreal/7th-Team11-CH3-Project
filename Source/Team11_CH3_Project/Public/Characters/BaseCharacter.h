#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UStatComponent;

UCLASS()
class TEAM11_CH3_PROJECT_API ABaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    virtual void ReceiveDamage(float DamageAmount, AActor* DamageCauser);
    virtual void Die();
    virtual bool IsAlive() const;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UInventoryComponent* InventoryComponent;

    UFUNCTION()
    void HandleDeath();

public:

    // 당장은 정의만 추가
    UFUNCTION(BlueprintPure, Category = "Character")
    UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};