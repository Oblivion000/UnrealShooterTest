// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "APatrolEnemy.h"
#include "Components/SphereComponent.h"
#include "ACombatEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHICS2_API AACombatEnemy : public AAPatrolEnemy
{
	GENERATED_BODY()

public:
    AACombatEnemy();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Detection sphere
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USphereComponent* DetectionSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float DetectionRadius = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float FireRate = 1.0f; // Shots per second

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class AFPSProjectile_Bullet> ProjectileClass;

private:
    AActor* PlayerTarget;
    float LastFireTime;
    bool bPlayerInSight;

    void CheckForPlayer();
    bool CanSeePlayer(AActor* Player);
    void FireAtPlayer();
};