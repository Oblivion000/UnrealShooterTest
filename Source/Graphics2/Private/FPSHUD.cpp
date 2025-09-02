// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthWidget.h"
#include "Character/FPSCharacter.h"

void AFPSHUD::BeginPlay()
{
    Super::BeginPlay();

    // Create widgets once at the start
    APlayerController* PlayerController = GetOwningPlayerController();
    if (PlayerController)
    {
        // Create PlayerHUD widget
        /*if (PlayerHUDClass)
        {
            PlayerHUDWidget = CreateWidget(GetWorld(), PlayerHUDClass);
            if (PlayerHUDWidget)
            {
                PlayerHUDWidget->AddToViewport();
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("PlayerHUDClass is not assigned in FPSHUD!"));
        }*/

        if (PlayerHealthWidgetClass)
        {
            PlayerHealthWidgetInstance = CreateWidget<UPlayerHealthWidget>(GetWorld(), PlayerHealthWidgetClass);
            if (PlayerHealthWidgetInstance)
            {
                PlayerHealthWidgetInstance->AddToViewport();
                PlayerHealthWidgetInstance->InitializePlayerWidget(Cast<AFPSCharacter>(GetOwningPawn()));
            }
        }
    }
}

void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

    // Only draw the crosshair here
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
}