// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Monster_DealDamage.h"

void UANS_Monster_DealDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//TODO 훨윈드같이 지속딜 있으면 여기 채워야함
}

void UANS_Monster_DealDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	//TODO 훨윈드같이 지속딜 있으면 여기 채워야함
}
