// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Monster_DealDamage.h"

#include "Characters/Monster/MonsterBase.h"

void UAN_Monster_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp)
	{
		return;
	}
	if (AActor* Actor = MeshComp->GetOwner())
	{
		if (AMonsterBase* Monster = Cast<AMonsterBase>(Actor))
		{
			Monster->DealDamage();
		}
	}
	
}
