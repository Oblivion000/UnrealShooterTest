// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsHUD.h"
#include "Components/ProgressBar.h"

void UPlayerStatsHUD::UpdateHealthBar(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
}

void UPlayerStatsHUD::UpdateStaminaBar(float StaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercent);
	}
}

