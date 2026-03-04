// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipmentComponent.h"
#include "Components/SkillComponent.h"
#include "Components/BuffComponent.h"
#include "Components/StatComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Subsystems/ItemWorldSubsystem.h"
#include "WeaponActor.h"
#include "Characters/Monster/MonsterBase.h"
#include "Components/Items/EquipmentItemDataAsset.h"
#include "Components/Items/GemItemDataAsset.h"
#include "Components/Items/ItemSlot.h"
#include "Components/Items/WeaponItemDataAsset.h"
#include "Components/Items/Equipments/EquipmentInstance.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}

void UEquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (UEnum* EnumPtr = StaticEnum<EEquipmentType>())
	{
		for (int32 i = 0; i < EnumPtr->NumEnums(); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(EnumPtr->GetValueByIndex(i));
			UEquipmentSlot* EquipmentSlot = NewObject<UEquipmentSlot>(this);
			EquipmentSlot->Init(this, i, Type);
			EquipmentSlots.Add(EquipmentSlot);
		}
	}
	GemSlots.SetNum(MaxSKillGemCount);
	for (int32 i = 0; i < MaxSKillGemCount; ++i)
	{
		UEquipmentSlot* EquipmentSlot = NewObject<UEquipmentSlot>(this);
		GemSlots[i] = EquipmentSlot;
		GemSlots[i]->Init(this, EquipmentSlots.Num() + i, EEquipmentType::SkillGem);
	}
}

TArray<TObjectPtr<UEquipmentInstance>> UEquipmentComponent::GetEquipments()
{
	TArray<TObjectPtr<UEquipmentInstance>> Ret;
	for (TObjectPtr<UEquipmentSlot>& EquipmentSlot : EquipmentSlots)
	{
		Ret.Add(Cast<UEquipmentInstance>(EquipmentSlot->GetItemInstance()));
	}
	return Ret;
}

//TArray<TObjectPtr<UEquipmentInstance>> UItemManager::GetGemEquipments()
//{
//	TArray<TObjectPtr<UEquipmentInstance>> Ret;
//	for (TObjectPtr<UEquipmentSlot>& GemSlot : GemSlots)
//	{
//		Ret.Add(Cast<UEquipmentInstance>(GemSlot->GetItemInstance()));
//	}
//	return Ret;
//}
//
//void UItemManager::RestoreGemEquipments(const TArray<TObjectPtr<UEquipmentInstance>>& GemData)
//{
//	for (int32 i = 0; i < GemData.Num(); i++)
//	{
//		if (IsValid(GemData[i]) == false)
//			continue;
//		
//		EquipGemTo(i, GemData[i]);
//	}
//}

void UEquipmentComponent::Clear()
{
	UnequipWeapon();
	for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
	{
		EquipmentSlots[i]->SetItemInstance(nullptr);
	}
	for (int32 i = 0; i < GemSlots.Num(); ++i)
	{
		GemSlots[i]->SetItemInstance(nullptr);
	}
}

void UEquipmentComponent::EquipWeapon(UEquipmentInstance* WeaponItemInstance)
{
	UnequipWeapon();
	if (!WeaponItemInstance)
	{
		return;
	}
	UWeaponItemDataAsset* WeaponItemDataAsset = Cast<UWeaponItemDataAsset>(WeaponItemInstance->GetItemDataAsset());
	if (!WeaponItemDataAsset)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(
		WeaponItemDataAsset->GetWeaponActorClass().LoadSynchronous(), SpawnParams);
	if (IsValid(Weapon))
	{
		auto* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		Weapon->Init(WeaponItemDataAsset, Mesh);

		CurrentWeapon = Weapon;

		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
		{
			if (USkillComponent* SkillComponent = Player->FindComponentByClass<USkillComponent>())
			{
				SkillComponent->EquipSkillGem(0, Weapon->GetDefaultSkillData());
			}
		}
		if (AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner()))
		{
			if (USkillComponent* SkillComponent = Monster->FindComponentByClass<USkillComponent>())
			{
				SkillComponent->EquipSkillGem(0, Weapon->GetDefaultSkillData());
			}
		}
	}
}

EItemContainerType UEquipmentComponent::GetItemContainerType() const
{
	return EItemContainerType::Equipment;
}


UItemInstance* UEquipmentComponent::GetItem(int32 Index)
{
	if (Index < 0)
	{
		return nullptr;
	}

	if (Index < EquipmentSlots.Num())
	{
		if (EquipmentSlots[Index])
		{
			return EquipmentSlots[Index]->GetItemInstance();
		}
		return nullptr;
	}
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		Index -= EquipmentSlots.Num();
		if (GemSlots[Index])
		{
			return GemSlots[Index]->GetItemInstance();
		}
		return nullptr;
	}
	return nullptr;
}

void UEquipmentComponent::UnequipAt(int32 Index)
{
	if (Index < EquipmentSlots.Num())
	{
		if (UEquipmentInstance* CurrentEquipmentInstance = EquipmentSlots[Index]->GetEquipmentInstance())
		{
			CurrentEquipmentInstance->SetIsEquipped(false);

			UBuffComponent* BuffManager = GetOwner()->FindComponentByClass<UBuffComponent>();
			if (IsValid(BuffManager))
			{
				if (TArray<int32>* IDs = EquipmentBuffIDs.Find(EquipmentSlots[Index]->GetEquipmentType()))
				{
					for (int32& ID : *IDs)
					{
						BuffManager->RemoveBuff(ID);
					}
					IDs->Empty();
				}
			}
			CurrentEquipmentInstance->OnStatsRecalculated.RemoveDynamic(this, &UEquipmentComponent::OnEquipmentStatChanged);
			EquipmentSlots[Index]->SetItemInstance(nullptr);
		}
	}
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		int32 GemIndex = Index - EquipmentSlots.Num();
		if (UEquipmentInstance* CurrentEquipmentInstance = GemSlots[GemIndex]->GetEquipmentInstance())
		{
			CurrentEquipmentInstance->SetIsEquipped(false);
		}
		GemSlots[GemIndex]->SetItemInstance(nullptr);

		if (AActor* Owner = GetOwner())
		{
			if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
			{
				SkillComponent->UnEquipSkillGem(GemIndex + 1);
			}
		}
	}
}

void UEquipmentComponent::EquipTo(int32 Index, UEquipmentInstance* EquipmentInstance)
{
	if (EquipmentSlots[Index] &&
		(
			EquipmentInstance == nullptr ||
			EquipmentSlots[Index]->GetEquipmentType() == EquipmentInstance->GetEquipmentType()
		)
	)
	{
		//TODO STAT And BroadCast UI 브로드 캐스트
		EquipmentSlots[Index]->SetItemInstance(EquipmentInstance);
		if (EquipmentInstance)
		{
			// OnEquipmentStatChanged 바인딩 및 장비 장착 여부 수정 set 스탯 계산
			EquipmentInstance->OnStatsRecalculated.AddDynamic(this, &UEquipmentComponent::OnEquipmentStatChanged);
			EquipmentInstance->SetIsEquipped(true);
			// 장착하고 스탯 계산 -> OnEquipmentStatChanged 호출
			EquipmentInstance->CalculateStats();
		}
	}
	// 무기
	if (EquipmentSlots[Index]->GetEquipmentType() == EEquipmentType::Weapon)
	{
		EquipWeapon(EquipmentInstance);
	}
}

void UEquipmentComponent::EquipGemTo(int32 Index, UEquipmentInstance* EquipmentInstance)
{
	GemSlots[Index]->SetItemInstance(EquipmentInstance);
	OnEquipmentSlotChanged.Broadcast(GemSlots[Index]);

	if (AActor* Owner = GetOwner())
	{
		if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
		{
			if (EquipmentInstance)
			{
				if (UGemItemDataAsset* GemItemDataAsset = Cast<
					UGemItemDataAsset>(EquipmentInstance->GetItemDataAsset()))
				{
					//TODO UI 브로드 캐스트
					SkillComponent->EquipSkillGem(Index + 1, GemItemDataAsset->GetSkillData());
				}
			}
		}
	}
}

// 비우는것도 여기서 있는거랑 있는거 바꿀때 구현X 
bool UEquipmentComponent::SetItemAt(UItemInstance* ItemInstance, int32 Index)
{
	UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance);

	if (Index < 0 || Index >= EquipmentSlots.Num() + GemSlots.Num())
	{
		return false;
	}
	//장착중인 장비 해제
	UnequipAt(Index);


	// 장비 장착
	if (Index < EquipmentSlots.Num())
	{
		EquipTo(Index, EquipmentInstance);
		OnEquipmentSlotChanged.Broadcast(EquipmentSlots[Index]);
	}
	// 스킬 젬
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		Index -= EquipmentSlots.Num();
		EquipGemTo(Index, EquipmentInstance);
		OnEquipmentSlotChanged.Broadcast(GemSlots[Index]);
	}
	return true;
}


bool UEquipmentComponent::CanReceiveItem(UItemInstance* ItemInstance, int32 Index)
{
	if (Index < 0)
	{
		return false;
	}
	if (ItemInstance == nullptr)
	{
		return true;
	}
	UEquipmentInstance* EquipmentInstance = Cast<UEquipmentInstance>(ItemInstance);
	if (!EquipmentInstance)
	{
		return false;
	}
	if (Index < EquipmentSlots.Num())
	{
		if (EquipmentSlots[Index]->GetEquipmentType() != EquipmentInstance->GetEquipmentType())
		{
			return false;
		}
		return true;
	}
	else if (Index < EquipmentSlots.Num() + GemSlots.Num())
	{
		Index -= EquipmentSlots.Num();
		if (EquipmentInstance->GetEquipmentType() != EEquipmentType::SkillGem)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool UEquipmentComponent::SwapItems(int32 MyIndex, IItemContainer* OtherContainer, int32 OtherIndex)
{
	if (OtherContainer == nullptr)
	{
		return false;
	}
	UItemInstance* OtherItemInstance = OtherContainer->GetItem(OtherIndex);
	UItemInstance* MyItemInstance = GetItem(MyIndex);
	if (!CanReceiveItem(OtherItemInstance, MyIndex))
	{
		return false;
	}
	if (!OtherContainer->CanReceiveItem(MyItemInstance, OtherIndex))
	{
		return false;
	}
	SetItemAt(OtherItemInstance, MyIndex);
	OtherContainer->SetItemAt(MyItemInstance, OtherIndex);
	return true;
}

TArray<FSavedEquipmentData> UEquipmentComponent::GetEquipmentSaveData()
{
	TArray<FSavedEquipmentData> Result;
	// 장비 슬롯 저장
	for (TObjectPtr<UEquipmentSlot>& Slot : EquipmentSlots)
	{
		FSavedEquipmentData Data;
		// 슬롯이 비어있으면 nullptr 저장
		UEquipmentInstance* Equip = Slot ? Slot->GetEquipmentInstance() : nullptr;
		if (IsValid(Equip))
		{
			// 장비슬롯에 장비가 있으면 Parts 소켓 체크
			Data.DataAsset = Equip->GetItemDataAsset();
			for (UItemSlot* Socket : Equip->GetPartsSlots())
			{
				// 소켓이 비어있으면 그대로 nullptr 저장
				UItemInstance* Part = IsValid(Socket) ? Socket->GetItemInstance() : nullptr;
				Data.PartSlots.Add(IsValid(Part) ? Part->GetItemDataAsset() : nullptr);
			}
		}
		Result.Add(Data);
	}
	return Result;
}

TArray<FSavedEquipmentData> UEquipmentComponent::GetGemSaveData()
{
	TArray<FSavedEquipmentData> Result;
	// 스킬 젬 슬롯 저장
	for (TObjectPtr<UEquipmentSlot>& GemSlot : GemSlots)
	{
		FSavedEquipmentData Data;
		// 비어있으면 nullptr 저장
		UEquipmentInstance* Gem = GemSlot ? GemSlot->GetEquipmentInstance() : nullptr;
		if (IsValid(Gem))
		{
			Data.DataAsset = Gem->GetItemDataAsset();
		}
		Result.Add(Data);
	}
	return Result;
}

void UEquipmentComponent::RestoreFromSaveData(const TArray<FSavedEquipmentData>& Data)
{
	//UObject* GI = GetWorld()->GetGameInstance();
	for (int32 i = 0; i < Data.Num(); i++)
	{
		if (Data[i].DataAsset.IsNull())
			continue;
		UItemDataAsset* EquipData = Data[i].DataAsset.LoadSynchronous();
		if (IsValid(EquipData) == false)
			continue;

		UEquipmentInstance* Equip = NewObject<UEquipmentInstance>();
		Equip->Init(EquipData, 1);
		// 파츠 복구
		for (int32 j = 0; j < Data[i].PartSlots.Num(); j++)
		{
			if (Data[i].PartSlots[j].IsNull())
				continue;
			UItemDataAsset* PartData = Data[i].PartSlots[j].LoadSynchronous();
			if (IsValid(PartData) == false)
				continue;
			
			UClass* PartClass = PartData->GetInstanceClass();
			UItemInstance* PartInstance = PartClass
				? NewObject<UItemInstance>(Equip, PartClass)
				: NewObject<UItemInstance>(Equip);
			PartInstance->Init(PartData, 1);
			Equip->SetItemAt(PartInstance, j);

		}
		SetItemAt(Equip, static_cast<int64>(Equip->GetEquipmentType()));
	}
}

void UEquipmentComponent::RestoreGemFromSaveData(const TArray<FSavedEquipmentData>& Data)
{
	//UObject* GI = GetWorld()->GetGameInstance();
	for (int32 i = 0; i < Data.Num(); i++)
	{
		if (Data[i].DataAsset.IsNull())
			continue;
		UItemDataAsset* GemData = Data[i].DataAsset.LoadSynchronous();
		if (IsValid(GemData) == false)
			continue;

		UEquipmentInstance* GemInstance = NewObject<UEquipmentInstance>();
		GemInstance->Init(GemData, 1);
		EquipGemTo(i, GemInstance);
	}
}

void UEquipmentComponent::UnequipWeapon()
{
	if (IsValid(CurrentWeapon) == false)
		return;

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}

TArray<TObjectPtr<UEquipmentSlot>>& UEquipmentComponent::GetSkillGemSlots()
{
	return GemSlots;
}


UEquipmentSlot* UEquipmentComponent::GetEquipmentSlot(EEquipmentType EquipmentType)
{
	int64 Index = static_cast<int64>(EquipmentType);
	if (EquipmentSlots.IsValidIndex(Index))
	{
		return EquipmentSlots[Index];
	}
	return nullptr;
}

void UEquipmentComponent::OnEquipmentStatChanged(EEquipmentType Type, UEquipmentInstance* Instance)
{
	UBuffComponent* BuffManager = GetOwner()->FindComponentByClass<UBuffComponent>();
	if (IsValid(BuffManager) == false)
		return;

	if (TArray<int32>* IDs = EquipmentBuffIDs.Find(Type))
	{
		for (int32 ID : *IDs)
		{
			BuffManager->RemoveBuff(ID);
		}
		IDs->Empty();
	}

	TArray<int32> NewIDs;
	for (auto& Pair : Instance->GetCachedStats())
	{
		NewIDs.Add(BuffManager->AddBuff(Pair.Key, EBuffType::Add, Pair.Value, -1.f));
	}

	EquipmentBuffIDs.Add(Type, NewIDs);
}


//void UItemManager::RestoreEquipment(TArray<TObjectPtr<UEquipmentInstance>> EquipmentData)
//{
//	for (TObjectPtr<UEquipmentInstance>& EquipmentInstance : EquipmentData)
//	{
//		if (EquipmentInstance)
//		{
//			SetItemAt(EquipmentInstance, static_cast<int64>(EquipmentInstance->GetEquipmentType()));
//		}
//	}
//}
