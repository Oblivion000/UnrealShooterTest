// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"
#include "Blueprint/UserWidget.h"

void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

	APlayerController* PlayerController = GetOwningPlayerController();

    if (CrosshairTexture)
    {
        // Find the center of our canvas.
        FVector2D Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

        // Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
        FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSizeX() * 0.5f), Center.Y - (CrosshairTexture->GetSizeY() * 0.5f));

        // Draw the crosshair at the centerpoint.
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->GetResource(), FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }

    if (PlayerHUDClass && PlayerController) 
    {
		PlayerHUDWidget = CreateWidget(GetWorld(), PlayerHUDClass);
		PlayerStatsHUDWidget = CreateWidget<UPlayerStatsHUD>(PlayerController, PlayerStatsHUDClass);
        if (PlayerHUDWidget)
        {
            PlayerHUDWidget->AddToViewport();
		}
        if (PlayerStatsHUDWidget)
        {
            PlayerStatsHUDWidget->AddToViewport();
		}
    }
}