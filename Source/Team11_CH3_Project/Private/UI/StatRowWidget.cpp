// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatRowWidget.h"

#include "Components/TextBlock.h"

void UStatRowWidget::Update(EStat Stat, float Amount)
{
	if (UEnum* EnumPtr = StaticEnum<EStat>())
	{
		FText DisplayText = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Stat));
		if (StatName)
		{
			StatName->SetText(DisplayText);
		}
		if (StatAmount)
		{
			StatAmount->SetText(FText::AsNumber(Amount));
		}
	}
}
