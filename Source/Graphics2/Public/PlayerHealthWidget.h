// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthComponent.h"
#include "PlayerHealthWidget.generated.h"

// Forward declaration instead of include
class AFPSCharacter;

UCLASS()
class GRAPHICS2_API UPlayerHealthWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Function to initialize the widget with player character
    UFUNCTION(BlueprintCallable, Category = "Health Widget")
    void InitializePlayerWidget(AFPSCharacter* InPlayerCharacter);

    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    void UpdateStaminaBar();

protected:
    // Progress bar references - bind these in Blueprint
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* StaminaProgressBar;

    // Reference to the player character we're monitoring
    UPROPERTY()
    AFPSCharacter* PlayerCharacter;

    // Reference to the health component
    UPROPERTY()
    UHealthComponent* HealthComponent;

    // Delegate functions
    UFUNCTION()
    void OnHealthChanged(float CurrentHealth, float MaxHealth);

    UFUNCTION()
    void OnStaminaChanged(float CurrentStamina, float MaxStamina);

    UFUNCTION()
    void OnDeath();

    // Blueprint implementable events for additional customization
    UFUNCTION(BlueprintImplementableEvent, Category = "Health Widget")
    void OnHealthUpdated(float HealthPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "Health Widget")
    void OnStaminaUpdated(float StaminaPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "Health Widget")
    void OnPlayerDied();

private:
};
