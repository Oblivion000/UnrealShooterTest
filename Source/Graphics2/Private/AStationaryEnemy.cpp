// Fill out your copyright notice in the Description page of Project Settings.


#include "AStationaryEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

AAStationaryEnemy::AAStationaryEnemy()
{
    // Call parent constructor first
    // Disable movement for this enemy
    GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AAStationaryEnemy::BeginPlay()
{
    Super::BeginPlay();
    // This enemy just sits there - no additional setup needed
}

void AAStationaryEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // No movement logic needed
}