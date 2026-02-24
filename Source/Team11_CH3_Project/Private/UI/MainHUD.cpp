#include "UI/MainHUD.h"
// #include "UI/PrimaryLayout.h"
#include "Blueprint/UserWidget.h"
// #include "UI/Subsystem/UIManageSubsystem.h"


void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (!RootLayoutClass)
	{
		UE_LOG(LogTemp, Error, TEXT("AMyHUD: RootLayoutClass가 설정되지 않았습니다! (BP_MyHUD에서 설정 필요)"));
		return;
	}

	// 3. RootLayout을 생성
	RootLayoutInstance = CreateWidget<UPrimaryLayout>(GetOwningPlayerController(), RootLayoutClass);
	if (!RootLayoutInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AMyHUD: RootLayout 인스턴스 생성 실패."));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("AMyHUD: RootLayout 인스턴스 생성 완료."));

	// 4. 추가
	RootLayoutInstance->AddToViewport();
	*/
}

void AMainHUD::OnRootLayoutReady()
{
	/*
	FString CurrentLevelName = GetWorld()->GetName();
	if (CurrentLevelName.Contains("Frontend"))
	{
		if (UUIManageSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManageSubsystem>())
		{
			UIManager->ShowWidget(FGameplayTag::RequestGameplayTag(TEXT("UI.Request.MainMenu")));
		}
		else
		{
			UIManager->ShowWidget(FGameplayTag::RequestGameplayTag(TEXT("UI.Request.ShowHUD")));
		}
	}
	*/
}
