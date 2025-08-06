// Fill out your copyright notice in the Description page of Project Settings.
//.h is to define and .cpp is to write the logic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h" //keeping alphabetical order helps for organization.
#include "Logging/StructuredLog.h" //for logging
#include "FPSProjectGameMode.generated.h" //generated.h has to be at the bottom of the includes.

/**
 * 
 */
UCLASS()
class GRAPHICS2_API AFPSProjectGameMode : public AGameModeBase //A is infront to define the object as an Actor.
{
	GENERATED_BODY()
	virtual void StartPlay() override;
};
