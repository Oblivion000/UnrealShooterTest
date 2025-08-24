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

void AABaseEnemy::TakeDamage(float DamageAmount)
{
    if (!IsAlive() || DamageAmount <= 0.0f) return;

	float OldHealth = CurrentHealth;

    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

    //Added log for debugging
	UE_LOG(LogTemp, Warning, TEXT("Enemy took damage: %f, Current Health: %f"), DamageAmount, OldHealth, CurrentHealth);

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

