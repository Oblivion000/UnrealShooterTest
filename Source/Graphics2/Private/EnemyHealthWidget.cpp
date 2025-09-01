// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ABaseEnemy.h"

void UEnemyHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initially hide the widget
    SetVisibility(ESlateVisibility::Hidden);
}

void UEnemyHealthWidget::InitializeEnemyWidget(AABaseEnemy* InEnemy)
{
    if (!InEnemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyHealthWidget: Attempted to initialize with null enemy"));
        return;
    }

    Enemy = InEnemy;

    // Set enemy name if you have a text block for it
    if (EnemyNameText)
    {
        FString EnemyName = Enemy->GetClass()->GetName();
        // Remove the "A" prefix and "_C" suffix that Blueprint classes often have
        EnemyName = EnemyName.Replace(TEXT("A"), TEXT(""));
        EnemyName = EnemyName.Replace(TEXT("_C"), TEXT(""));
        EnemyNameText->SetText(FText::FromString(EnemyName));
    }

    // Update the health display
    UpdateHealthDisplay();

    UE_LOG(LogTemp, Log, TEXT("EnemyHealthWidget: Successfully initialized for enemy %s"),
        *Enemy->GetName());
}

void UEnemyHealthWidget::UpdateEnemyHealth()
{
    UpdateHealthDisplay();
}

void UEnemyHealthWidget::ShowWidget()
{
    if (Enemy && Enemy->IsAlive())
    {
        SetVisibility(ESlateVisibility::Visible);
        UpdateHealthDisplay();
    }
}

void UEnemyHealthWidget::HideWidget()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UEnemyHealthWidget::UpdateHealthDisplay()
{
    if (!Enemy)
    {
        return;
    }

    // Update health bar
    if (EnemyHealthProgressBar)
    {
        float HealthPercent = Enemy->GetMaxHealth() > 0 ?
            Enemy->GetCurrentHealth() / Enemy->GetMaxHealth() : 0.0f;

        EnemyHealthProgressBar->SetPercent(HealthPercent);

        // Call Blueprint event
        OnEnemyHealthUpdated(HealthPercent);
    }

    // Update health text if present
    if (HealthText)
    {
        FString HealthString = FString::Printf(TEXT("%.0f / %.0f"),
            Enemy->GetCurrentHealth(), Enemy->GetMaxHealth());
        HealthText->SetText(FText::FromString(HealthString));
    }

    // Check if enemy died
    if (!Enemy->IsAlive())
    {
        OnEnemyDied();
        HideWidget();
    }
}