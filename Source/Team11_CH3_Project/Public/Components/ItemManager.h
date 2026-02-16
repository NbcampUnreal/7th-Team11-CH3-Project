// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Items/ItemDataBase.h"
#include "Components/Items/PotionItemData.h"
#include "Components/Items/SkillGemItemData.h"
#include "Components/Items/Equipments/WeaponItemData.h"
#include "Components/Items/Equipments/ArmorItemData.h"
#include "ItemManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM11_CH3_PROJECT_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemManager();

	// 아이템 사용(타입별 분기)
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItem(FName RowName);

	// 장비 해제
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UnequipWeapon(FName RowName);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void UnequipArmor(FName RowName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// ItemTpye에 따른 사용 로직
	void UsePotion(FPotionItemData* Data);
	void UseSkillGem(FSkillGemItemData* Data);
	void EquipWeapon(FWeaponItemData* Data);
	void EquipArmor(FArmorItemData* Data);
	//데이터 테이블 참조
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> PotionTable;
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> SkillGemTable;
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponTable;
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ArmorTable;
};
