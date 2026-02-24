#include "UI/Subsystem/UIManageSubsystem.h"
#include "UI/PrimaryLayout.h"
#include "UI/DataAssets/UIMapDataAsset.h"

#include "Blueprint/UserWidget.h"

#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h" 



void UUIManageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (UIMapDataAsset)
    {
        CachedWidgetMap = UIMapDataAsset->WidgetMap;
        UE_LOG(LogTemp, Log, TEXT("UIManageSubsystem: %d개의 UI 맵 로드됨"), CachedWidgetMap.Num());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UIManageSubsystem: UIMapDataAsset이 설정 필요 (Subsystem Defaults에서 지정하세요)"));
    }
}

void UUIManageSubsystem::EnsureRootLayout()
{
    if (RootLayout) return;

    if (RootLayoutClass.IsNull())
    {
        UE_LOG(LogTemp, Error, TEXT("UIManageSubsystem: RootLayoutClass가 설정되지 않음"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManageSubsystem: World가 없음"));
        return;
    }

    TSubclassOf<UPrimaryLayout> LayoutCls = RootLayoutClass.LoadSynchronous();
    if (!LayoutCls)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManageSubsystem: RootLayoutClass 로드 실패"));
        return;
    }

    RootLayout = CreateWidget<UPrimaryLayout>(World, LayoutCls);
    if (!RootLayout)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManageSubsystem: RootLayout CreateWidget 실패"));
        return;
    }

    RootLayout->AddToViewport(0);
    UE_LOG(LogTemp, Log, TEXT("UIManageSubsystem: RootLayout 생성 및 AddToViewport 완료"));
}

UCommonActivatableWidget* UUIManageSubsystem::ShowWidget(FGameplayTag RequestTag)
{
    EnsureRootLayout();
    if (!RootLayout)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowWidget(%s): RootLayout이 없음"), *RequestTag.ToString());
        return nullptr;
    }

    const FUIWidgetPolicy* Policy = CachedWidgetMap.Find(RequestTag);
    if (!Policy)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowWidget(%s): UI Policy에 등록된 태그가 없음"), *RequestTag.ToString());
        return nullptr;
    }

    const FGameplayTag& LayerTag = Policy->TargetLayer;
    if (!LayerTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowWidget(%s): Policy.TargetLayer가 비어있음"), *RequestTag.ToString());
        return nullptr;
    }

    UCommonActivatableWidgetContainerBase* Stack = RootLayout->GetLayerWidget(LayerTag);
    if (!Stack)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowWidget(%s): RootLayout에서 %s 레이어 스택이 없음"),
            *RequestTag.ToString(), *LayerTag.ToString());
        return nullptr;
    }

    if (Policy->WidgetClass.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowWidget(%s): Policy.WidgetClass가 비어있음"), *RequestTag.ToString());
        return nullptr;
    }

    TSubclassOf<UCommonActivatableWidget> WidgetClass = Policy->WidgetClass.LoadSynchronous();
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowWidget(%s): %s 클래스를 로드할 수 없음"),
            *RequestTag.ToString(), *Policy->WidgetClass.ToString());
        return nullptr;
    }

    UCommonActivatableWidget* Created = Stack->AddWidget<UCommonActivatableWidget>(WidgetClass);

    UE_LOG(LogTemp, Log, TEXT("ShowWidget(%s): %s 레이어에 %s 위젯 Push 완료"),
        *RequestTag.ToString(), *LayerTag.ToString(), *GetNameSafe(WidgetClass));

    return Created;
}

bool UUIManageSubsystem::IsLayerBusy(FGameplayTag RequestTag) const
{
    if (!RootLayout) return false;

    const FUIWidgetPolicy* Policy = CachedWidgetMap.Find(RequestTag);
    if (!Policy) return false;

    const FGameplayTag& LayerTag = Policy->TargetLayer;
    if (!LayerTag.IsValid()) return false;

    UCommonActivatableWidgetContainerBase* Stack = RootLayout->GetLayerWidget(LayerTag);
    if (!Stack) return false;

    return (Stack->GetActiveWidget() != nullptr);
}

void UUIManageSubsystem::SetRootLayout(UPrimaryLayout* InRootLayout)
{
    RootLayout = InRootLayout;
    UE_LOG(LogTemp, Log, TEXT("UIManageSubsystem: RootLayout 등록(SetRootLayout)"));
}