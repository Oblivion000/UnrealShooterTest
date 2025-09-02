// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Grenade/GrenadeActor.h"
#include "HealthComponent.h"
#include "FPSHUD.h"
#include "PlayerHealthWidget.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr); //Check if the camera component is not null

	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent())); //Attach the camera to the capsule component
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight)); //Set the camera location relative to the capsule component
	FPSCameraComponent->bUsePawnControlRotation = true; //Allow the camera to rotate with the pawn

	if (!FPSCameraComponent) 
	{
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
		FPSCameraComponent->SetupAttachment((GetCapsuleComponent())); //CastChecked<USceneComponent, UCapsuleComponent> was before GetCapsuleComponent
		FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
		FPSCameraComponent->bUsePawnControlRotation = true;
	}

	FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMeshComponent != nullptr); //Check if the mesh component is not null

	//Only the owning player will see the mesh
	FPSMeshComponent->SetOnlyOwnerSee(true);

	//Atach the mesh to the camera component
	FPSMeshComponent->SetupAttachment(FPSCameraComponent);

	//Disable some environmental shadows to preserve illusion of having a single mesh.
	FPSMeshComponent->bCastDynamicShadow = false;
	FPSMeshComponent->CastShadow = false;

	//Owning player can't see the mesh in the first person view
	GetMesh()->SetOwnerNoSee(true);

	//Setup for Health Component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	CurrentStamina = MaxStamina; // Initialize stamina

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AFPSCharacter::OnHealthChanged);
	}

	// Create and initialize the health widget
	if (PlayerHealthWidgetClass)  // Note: changed variable name
	{
		PlayerHealthWidget = CreateWidget<UPlayerHealthWidget>(GetWorld(), PlayerHealthWidgetClass);
		if (PlayerHealthWidget)
		{
			PlayerHealthWidget->AddToViewport();
			PlayerHealthWidget->InitializePlayerWidget(this);
		}
	}
	
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting) 
	{
		//Stamina Drain
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		if (CurrentStamina <= 0.0f) 
		{
			CurrentStamina = 0.0f; //Ensure stamina doesn't go below zero
			bCanSprint = false; //Disable sprinting if stamina is depleted
			StopSprinting(); //Stop sprinting if stamina is depleted
		}
	}
	else 
	{
		//Stamina Regeneration
		if (CurrentStamina < MaxStamina) 
		{
			CurrentStamina += StaminaRegenRate * DeltaTime;
			if (CurrentStamina >= 50.0f) 
			{
				bCanSprint = true; //Re-enable sprinting if stamina is full
			}

			if (CurrentStamina > MaxStamina) 
			{
				CurrentStamina = MaxStamina; //Ensure stamina doesn't exceed maximum
			}
		}
	}
	//UpdateHUD();

	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

	//Check for Enemy Hover
	CheckForEnemyHover();


}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	//Look
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &AFPSCharacter::AddControllerPitchInput);

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, & AFPSCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::EndJump);

	//Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::StopSprinting);

	
	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	
	//Throw Grenade
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AFPSCharacter::ThrowGrenade);
}

void AFPSCharacter::MoveForward(float value)
{
	//1. This is the Unreal Tutorial Way
	FVector Direction = GetActorForwardVector();
	AddMovementInput(Direction, value);

	//AddMovementInput(GetActorForwardVector(), value); //This is the more modern way of doing it.
}

void AFPSCharacter::MoveRight(float value)
{
	FVector Direction = GetActorRightVector();
	AddMovementInput(Direction, value);

	//AddMovementInput(GetActorRightVector(), value); //This is the more modern way of doing it.
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::EndJump()
{
	bPressedJump = false;
}

void AFPSCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	UpdateHUD(); // This will update the health bar when health changes
}

void AFPSCharacter::StartSprinting()
{
	if (bCanSprint && CurrentStamina > 0.0f)
	{
		//Set the character's movement speed to the sprinting speed
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
}
void AFPSCharacter::StopSprinting()
{
	//Set the character's movement speed back to the walking speed
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void AFPSCharacter::Fire()
{
	if (!ProjectileClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	FVector TargetPoint = bHit ? HitResult.Location : TraceEnd;

	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	FVector LaunchDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();
	FRotator MuzzleRotation = LaunchDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AFPSProjectile_Bullet* Projectile = World->SpawnActor<AFPSProjectile_Bullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (Projectile)
	{
		Projectile->FireInDirection(LaunchDirection);
	}
}


void AFPSCharacter::ThrowGrenade()
{
	if (GrenadeClass && CurrentGrenades >0)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC) return;

		// Get camera location and rotation
		FVector CameraLocation;
		FRotator CameraRotation;
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector CameraForward = CameraRotation.Vector();

		// Spawn the grenade slightly in front of the camera
		FVector SpawnLocation = CameraLocation + CameraForward * 100.f;
		FRotator SpawnRotation = CameraRotation; // This makes sure it faces where you're looking

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the grenade actor
		AGrenadeActor* Grenade = GetWorld()->SpawnActor<AGrenadeActor>(GrenadeClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (Grenade && Grenade->ProjectileMovement)
		{
			// Launch it forward with some upward arc
			FVector LaunchVelocity = CameraForward * 1000.f + FVector(0.f, 0.f, 300.f);
			Grenade->ProjectileMovement->Velocity = LaunchVelocity;
			Grenade->ProjectileMovement->Activate();
		}

		//Reduce Amount of Grenades
		CurrentGrenades--;
	}
}

void AFPSCharacter::AddGrenades(int32 Amount)
{
	CurrentGrenades += Amount;
	if (CurrentGrenades > MaxGrenades)
	{
		CurrentGrenades = MaxGrenades; //Ensure grenades don't exceed maximum
	}
}

void AFPSCharacter::UpdateHUD()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		AFPSHUD* HUD = Cast<AFPSHUD>(PC->GetHUD());
		//if (HUD && HUD->PlayerStatsHUDWidget)  // Direct access, no getter
		//{
		//	// Update health bar
		//	if (HealthComponent)
		//	{
		//		float HealthPercent = HealthComponent->GetCurrentHealth() / HealthComponent->GetMaxHealth();
		//		HUD->PlayerStatsHUDWidget->UpdateHealthBar(HealthPercent);
		//	}

		//	// Update stamina bar
		//	float StaminaPercent = CurrentStamina / MaxStamina;
		//	HUD->PlayerStatsHUDWidget->UpdateStaminaBar(StaminaPercent);
		//}

		if (HUD && HUD->PlayerHealthWidgetInstance) // Make sure this exists
		{
			// Health: pass current and max health
			HUD->PlayerHealthWidgetInstance->UpdateHealthBar(
				HealthComponent->GetCurrentHealth(),
				HealthComponent->GetMaxHealth()
			);

			// Stamina: just call the function (it reads from PlayerCharacter)
			HUD->PlayerHealthWidgetInstance->UpdateStaminaBar();
		}


	}
}

void AFPSCharacter::CheckForEnemyHover()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 1000.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Pawn, QueryParams);

	AABaseEnemy* HitEnemy = nullptr;
	if (bHit)
	{
		HitEnemy = Cast<AABaseEnemy>(HitResult.GetActor());
	}

	// If we're looking at a different enemy or no enemy
	if (HitEnemy != CurrentHoveredEnemy)
	{
		// Hide current widget if exists
		if (CurrentEnemyWidget)
		{
			CurrentEnemyWidget->HideWidget();
		}

		CurrentHoveredEnemy = HitEnemy;

		// Show new widget if we're looking at an enemy
		if (CurrentHoveredEnemy && CurrentHoveredEnemy->IsAlive())
		{
			if (!CurrentEnemyWidget && EnemyHealthWidgetClass)
			{
				CurrentEnemyWidget = CreateWidget<UEnemyHealthWidget>(GetWorld(), EnemyHealthWidgetClass);
				if (CurrentEnemyWidget)
				{
					CurrentEnemyWidget->AddToViewport();
				}
			}

			if (CurrentEnemyWidget)
			{
				CurrentEnemyWidget->InitializeEnemyWidget(CurrentHoveredEnemy);
				CurrentEnemyWidget->ShowWidget();
			}
		}
	}
	// Update the widget if we're still looking at the same enemy
	else if (CurrentHoveredEnemy && CurrentEnemyWidget)
	{
		CurrentEnemyWidget->UpdateEnemyHealth();
	}
}





