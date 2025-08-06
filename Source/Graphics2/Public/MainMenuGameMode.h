// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WBP_MainMenu.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHICS2_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void StartPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget>MainMenuWidgetClass;


private:
	UPROPERTY()
	UWBP_MainMenu* MainMenuInstance;

};
