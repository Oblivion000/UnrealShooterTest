// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "WBP_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHICS2_API UWBP_MainMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:

	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* FadeOut;

	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();
	
};
