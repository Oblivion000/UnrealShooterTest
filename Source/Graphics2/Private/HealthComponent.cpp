// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = MaxHealth; // Initialize current health to max health

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	//CurrentHealth = MaxHealth; // Initialize current health to max health

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || IsDead())
	{
		return; // No damage taken or already dead
	}

	// Subtract damage, but clamp so health never goes below zero
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth); // Broadcast health change event

	if (CurrentHealth <= 0.0f)
	{
		OnDeath.Broadcast(); // Broadcast death event
		HandleDeath();       // Handle death logic
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || IsDead()) return; // No healing or already dead
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth); // Broadcast health change event
}

void UHealthComponent::HandleDeath()
{
	OnDeath.Broadcast(); // Broadcast death event
}

