// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsHUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHICS2_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
	

protected:
    // This will be drawn at the center of the screen.
    UPROPERTY(EditDefaultsOnly)
    UTexture2D* CrosshairTexture;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDClass;

    UPROPERTY()
	UUserWidget* PlayerHUDWidget;

public:
    // Primary draw call for the HUD.
    virtual void DrawHUD() override;

private:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPlayerStatsHUD> PlayerStatsHUDClass;

	UPROPERTY()
	UPlayerStatsHUD* PlayerStatsHUDWidget;
};
