// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile_Bullet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "FPSCharacter.generated.h"

UCLASS()
class GRAPHICS2_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile_Bullet> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)

	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMeshComponent; //FPS Mesh Component

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset; //Offset for the muzzle of the weapon

	//Movement Properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkingSpeed = 600.0f; //Default walking speed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintingSpeed = 1200.0f; //Default running speed

	bool bIsSprinting = false; //Flag to check if the character is running

	//Stamina Properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.0f; //Maximum stamina

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina; //Current stamina of the character

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate = 25.0f; //Rate at which stamina drains while sprinting

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate = 20.0f; //Rate at which stamina regenerates when not sprinting

	bool bCanSprint = true; //Flag to check if the character can sprint

	//Health Properties

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent; //Health component to manage health
	
	
	//Grenade Properties


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	TSubclassOf<class AGrenadeActor> GrenadeClass; //Class of the grenade to spawn

	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	int32 MaxGrenades = 3; //Maximum number of grenades the character can carry

	UPROPERTY(VisibleAnywhere, Category = "Grenade")
	int32 CurrentGrenades = 3; //Current number of grenades the character has
	UFUNCTION()
	void MoveForward(float value); //Defining MoveForward functionality

	UFUNCTION()
	void MoveRight(float value); //Defining the Move Right functionality

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void EndJump();

	UFUNCTION()
	void StartSprinting();

	UFUNCTION()
	void StopSprinting();

	UFUNCTION(BlueprintCallable) //Allows for the function to be called in Blueprints
	void Fire();

	UFUNCTION(BlueprintCallable) //Allows for the function to be called in Blueprints
	void ThrowGrenade(); //Function to throw a grenade

	UFUNCTION(BlueprintCallable, Category = "Grenade")
	void AddGrenades(int32 Amount); //Function to add a grenade to the character's inventory
};
