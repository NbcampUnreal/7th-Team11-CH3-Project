// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatDetailWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/StatComponent.h"
#include "UI/StatRowWidget.h"

void UStatDetailWidget::UpdateStatBox(TMap<EStat, float> Stats)
{
	StatRows.Empty();
	if (IsValid(StatBox) == false)
		return;
	StatBox->ClearChildren();
	if (IsValid(StatRowWidgetClass) == false)
		return;

	for (auto Pair : Stats)
	{
		UStatRowWidget* Row = CreateWidget<UStatRowWidget>(this, StatRowWidgetClass);
		if (IsValid(Row) == false)
			return;

		Row->Update(Pair.Key, Pair.Value);
		StatBox->AddChildToVerticalBox(Row);
		StatRows.Add(Row);
	}
}