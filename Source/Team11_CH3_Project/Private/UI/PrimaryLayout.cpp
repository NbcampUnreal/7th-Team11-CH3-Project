#include "UI/PrimaryLayout.h"
#include "UI/Subsystem/UIManageSubsystem.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UPrimaryLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if (!LayerTag.IsValid() || !LayerWidget)
	{
		return;
	}

	if (!IsDesignTime())
	{
		LayerWidget->SetTransitionDuration(0.0);

		Layers.Add(LayerTag, LayerWidget);
	}
}

UCommonActivatableWidgetContainerBase* UPrimaryLayout::GetLayerWidget(FGameplayTag LayerTag)
{
	if (TObjectPtr<UCommonActivatableWidgetContainerBase>* Result = Layers.Find(LayerTag))
	{
		return *Result;
	}
	return nullptr;
}

void UPrimaryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	if (UUIManageSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManageSubsystem>())
	{
		UIManager->SetRootLayout(this);
	}
}