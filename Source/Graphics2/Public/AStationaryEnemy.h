// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseEnemy.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AStationaryEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHICS2_API AAStationaryEnemy : public AABaseEnemy
{
	GENERATED_BODY()
	
public:
    AAStationaryEnemy();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};
