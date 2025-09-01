// Fill out your copyright notice in the Description page of Project Settings.

#include "ABaseEnemy.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"

// Sets default values
AABaseEnemy::AABaseEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Initialize Health Value
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    //Create damage widget component
    DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
    if (DamageWidgetComponent)
    {
        DamageWidgetComponent->SetupAttachment(RootComponent);
        DamageWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // Adjust the position as needed
        DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
        DamageWidgetComponent->SetVisibility(false); // Initially hidden
    }
}

// Called when the game starts or when spawned
void AABaseEnemy::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth; // Initialize current health
}

// Called every frame
void AABaseEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Override Unreal's built-in damage system - this will handle grenade damage automatically
float AABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (!IsAlive() || DamageAmount <= 0.0f)
        return 0.0f;

    float OldHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

    UE_LOG(LogTemp, Warning, TEXT("Enemy took %f damage from %s. Current Health: %f"),
        DamageAmount, DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"), CurrentHealth);

    ShowDamageWidget(DamageAmount);

    if (CurrentHealth <= 0.0f)
    {
        Die();
    }

    // Call parent implementation for any additional processing
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    return DamageAmount;
}

// Keep your existing custom TakeDamage function for other systems that might use it
void AABaseEnemy::TakeDamage(float DamageAmount)
{
    if (DamageAmount <= 0.0f || !IsAlive())
    {
        return; // No damage taken or already dead
    }

    // Subtract damage, but clamp so health never goes below zero
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);

    //Added log for debugging
    UE_LOG(LogTemp, Warning, TEXT("Enemy took %f damage (custom). Current Health: %f"), DamageAmount, CurrentHealth);

    ShowDamageWidget(DamageAmount);

    if (CurrentHealth <= 0.0f)
    {
        Die();
    }
}

void AABaseEnemy::Die()
{
    // Handle death logic here
    // Could trigger animations, drop loot, etc.
    UE_LOG(LogTemp, Warning, TEXT("Enemy died!"));
}

void AABaseEnemy::ShowDamageWidget(float DamageAmount)
{
    if (DamageWidgetComponent)
    {
        DamageWidgetComponent->SetVisibility(true);

        // Clear existing timer and set new one
        GetWorld()->GetTimerManager().ClearTimer(DamageWidgetTimer);
        GetWorld()->GetTimerManager().SetTimer(DamageWidgetTimer,
            this, &AABaseEnemy::HideDamageWidget, 2.0f, false);
    }
}

void AABaseEnemy::HideDamageWidget()
{
    if (DamageWidgetComponent)
    {
        DamageWidgetComponent->SetVisibility(false);
    }
}

