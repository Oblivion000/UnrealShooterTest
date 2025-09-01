// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Engine/TimerHandle.h"
#include "ABaseEnemy.generated.h"

UCLASS()
class GRAPHICS2_API AABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Health properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    // Damage widget component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    class UWidgetComponent* DamageWidgetComponent;

    // Widget class to spawn for damage display
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> DamageWidgetClass;

    // Timer for hiding damage widget
    FTimerHandle DamageWidgetTimer;

    // Damage and health functions
    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual void Die();

    // Widget display functions
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void ShowDamageWidget(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void HideDamageWidget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Getter for current health
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const { return CurrentHealth; }

    // Getter for max health
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }

    // Check if enemy is alive
    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsAlive() const { return CurrentHealth > 0.0f; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamgeEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
