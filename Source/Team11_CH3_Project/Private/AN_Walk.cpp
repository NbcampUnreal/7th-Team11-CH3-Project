// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Walk.h"

#include "Characters/PlayerCharacter.h"

void UAN_Walk::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp)
	{
		return;
	}
	if (AActor* Actor = MeshComp->GetOwner())
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor))
		{
			PlayerCharacter->HearingNotifyToEnemy(400.0f);
		}
	}
}
