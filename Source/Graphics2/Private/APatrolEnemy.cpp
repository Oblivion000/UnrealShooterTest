// Fill out your copyright notice in the Description page of Project Settings.


#include "APatrolEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

AAPatrolEnemy::AAPatrolEnemy()
{
    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
}

void AAPatrolEnemy::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    TargetLocation = StartLocation + (GetActorForwardVector() * PatrolDistance);
}

void AAPatrolEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdatePatrol(DeltaTime);
}

void AAPatrolEnemy::UpdatePatrol(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();
    FVector Direction;

    if (bMovingToTarget)
    {
        Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

        // Check if we've reached the target
        if (FVector::Dist(CurrentLocation, TargetLocation) < 50.0f)
        {
            bMovingToTarget = false;
        }
    }
    else
    {
        Direction = (StartLocation - CurrentLocation).GetSafeNormal();

        // Check if we've reached the start
        if (FVector::Dist(CurrentLocation, StartLocation) < 50.0f)
        {
            bMovingToTarget = true;
        }
    }

    // Move the enemy
    AddMovementInput(Direction, 1.0f);
}

