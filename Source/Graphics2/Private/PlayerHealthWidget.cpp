// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthWidget.h"
#include "Components/ProgressBar.h"
#include "Character/FPSCharacter.h"

void UPlayerHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();

}

void UPlayerHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Update stamina bar every frame since stamina changes in Tick
    UpdateStaminaBar();
}

void UPlayerHealthWidget::InitializePlayerWidget(AFPSCharacter* InPlayerCharacter)
{
    if (!InPlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerHealthWidget: Attempted to initialize with null FPSCharacter"));
        return;
    }

    PlayerCharacter = InPlayerCharacter;
    HealthComponent = PlayerCharacter->HealthComponent;

    if (!HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerHealthWidget: Player character has no HealthComponent"));
        return;
    }

    // Unbind from previous health component if any
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.RemoveDynamic(this, &UPlayerHealthWidget::OnHealthChanged);
        HealthComponent->OnDeath.RemoveDynamic(this, &UPlayerHealthWidget::OnDeath);
    }

    // Bind to the health component's delegates
    HealthComponent->OnHealthChanged.AddDynamic(this, &UPlayerHealthWidget::OnHealthChanged);
    HealthComponent->OnDeath.AddDynamic(this, &UPlayerHealthWidget::OnDeath);

    // Bind to stamina delegate
    PlayerCharacter->OnStaminaChanged.AddDynamic(this, &UPlayerHealthWidget::OnStaminaChanged);

    // Initialize both bars with current values
    UpdateHealthBar(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
    UpdateStaminaBar();

    UE_LOG(LogTemp, Log, TEXT("PlayerHealthWidget: Successfully bound to FPSCharacter"));
}

void UPlayerHealthWidget::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
    UpdateHealthBar(CurrentHealth, MaxHealth);

    // Call Blueprint event for additional customization
    float HealthPercent = MaxHealth > 0 ? CurrentHealth / MaxHealth : 0.0f;
    OnHealthUpdated(HealthPercent);
}

void UPlayerHealthWidget::OnDeath()
{
    UE_LOG(LogTemp, Log, TEXT("PlayerHealthWidget: Player died"));

    // Set health bar to 0
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(0.0f);
    }

    // Call Blueprint event
    OnPlayerDied();
}

void UPlayerHealthWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    if (!HealthProgressBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerHealthWidget: HealthProgressBar is null! Make sure it's bound in Blueprint."));
        return;
    }

    float HealthPercent = MaxHealth > 0 ? CurrentHealth / MaxHealth : 0.0f;
    HealthProgressBar->SetPercent(HealthPercent);
}

void UPlayerHealthWidget::UpdateStaminaBar()
{
    if (!StaminaProgressBar || !PlayerCharacter)
    {
        return;
    }

    float StaminaPercent = PlayerCharacter->MaxStamina > 0 ?
        PlayerCharacter->CurrentStamina / PlayerCharacter->MaxStamina : 0.0f;

    StaminaProgressBar->SetPercent(StaminaPercent);

    // Call Blueprint event for additional customization
    OnStaminaUpdated(StaminaPercent);
}

void UPlayerHealthWidget::OnStaminaChanged(float CurrentStamina, float MaxStamina)
{
    if (!StaminaProgressBar)
    {
        UE_LOG(LogTemp, Error, TEXT("StaminaProgressBar is null!"));
        return;
    }

    float StaminaPercent = MaxStamina > 0 ? CurrentStamina / MaxStamina : 0.0f;
    StaminaProgressBar->SetPercent(StaminaPercent);

    UE_LOG(LogTemp, Warning, TEXT("Stamina Updated: %f%% (%f/%f)"),
        StaminaPercent * 100.0f, CurrentStamina, MaxStamina);

    OnStaminaUpdated(StaminaPercent);
}

