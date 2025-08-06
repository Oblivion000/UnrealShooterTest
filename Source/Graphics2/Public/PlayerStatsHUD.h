// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsHUD.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class GRAPHICS2_API UPlayerStatsHUD : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	
	//Binding to update Health and Stamina Bars

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float HealthPercent);

	UFUNCTION(BlueprintCallable)
	void UpdateStaminaBar(float StaminaPercent);

protected:

	//Binding th Progress Bars from UMG

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;


};
