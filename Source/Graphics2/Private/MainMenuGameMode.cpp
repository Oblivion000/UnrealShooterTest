// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "WBP_MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void AMainMenuGameMode::StartPlay()
{
	Super::StartPlay();

	if (!MainMenuWidgetClass) return;

	MainMenuInstance = Cast<UWBP_MainMenu>(CreateWidget(GetWorld(), MainMenuWidgetClass));
	if (!MainMenuInstance) return;

	MainMenuInstance->AddToViewport();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MainMenuInstance->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}

	UE_LOG(LogTemp, Warning, TEXT("MainMenuGameMode: StartPlay called"));
}

