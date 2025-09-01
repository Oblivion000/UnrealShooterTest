// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ABaseEnemy.h"
#include "EnemyHealthWidget.generated.h"

UCLASS()
class GRAPHICS2_API UEnemyHealthWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // Function to initialize the widget with an enemy
    UFUNCTION(BlueprintCallable, Category = "Enemy Health Widget")
    void InitializeEnemyWidget(AABaseEnemy* InEnemy);

    // Function to update the health display
    UFUNCTION(BlueprintCallable, Category = "Enemy Health Widget")
    void UpdateEnemyHealth();

    // Function to show/hide the widget
    UFUNCTION(BlueprintCallable, Category = "Enemy Health Widget")
    void ShowWidget();

    UFUNCTION(BlueprintCallable, Category = "Enemy Health Widget")
    void HideWidget();

protected:
    // Progress bar for enemy health
    UPROPERTY(meta = (BindWidget))
    UProgressBar* EnemyHealthProgressBar;

    // Optional text display for health numbers
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;

    // Optional text for enemy name/type
    UPROPERTY(meta = (BindWidget))
    UTextBlock* EnemyNameText;

    // Reference to the enemy we're monitoring
    UPROPERTY()
    AABaseEnemy* Enemy;

    // Blueprint implementable events
    UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Health Widget")
    void OnEnemyHealthUpdated(float HealthPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Health Widget")
    void OnEnemyDied();

private:
    void UpdateHealthDisplay();
};