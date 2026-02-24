#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

UCLASS()
class TEAM11_CH3_PROJECT_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
/*
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPrimaryLayout> RootLayoutClass;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryLayout> RootLayoutInstance;
*/

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnRootLayoutReady();

};
