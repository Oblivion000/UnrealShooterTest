// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseEnemy.h"
#include "APatrolEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHICS2_API AAPatrolEnemy : public AABaseEnemy
{
	GENERATED_BODY()
	
public:
    AAPatrolEnemy();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
    float PatrolDistance = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
    float PatrolSpeed = 200.0f;

private:
    FVector StartLocation;
    FVector TargetLocation;
    bool bMovingToTarget = true;

    void UpdatePatrol(float DeltaTime);

};
