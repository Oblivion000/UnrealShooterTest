// Fill out your copyright notice in the Description page of Project Settings.


#include "ACombatEnemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "FPSProjectile_Bullet.h"

AACombatEnemy::AACombatEnemy()
{
    // Create detection sphere
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(RootComponent);
    DetectionSphere->SetSphereRadius(DetectionRadius);

    PlayerTarget = nullptr;
    LastFireTime = 0.0f;
    bPlayerInSight = false;
}

void AACombatEnemy::BeginPlay()
{
    Super::BeginPlay();
    DetectionSphere->SetSphereRadius(DetectionRadius);
}

void AACombatEnemy::Tick(float DeltaTime)
{
    CheckForPlayer();

    if (bPlayerInSight && PlayerTarget)
    {
        // Stop patrolling and face the player
        FVector DirectionToPlayer = (PlayerTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorRotation(DirectionToPlayer.Rotation());

        // Fire at player
        FireAtPlayer();
    }
    else
    {
        // Continue patrolling when player not in sight
        Super::Tick(DeltaTime);
    }
}

void AACombatEnemy::CheckForPlayer()
{
    // Simple player detection - find player character within detection sphere
    UWorld* World = GetWorld();
    if (!World) return;

    ACharacter* Player = World->GetFirstPlayerController()->GetCharacter();
    if (Player && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= DetectionRadius)
    {
        if (CanSeePlayer(Player))
        {
            PlayerTarget = Player;
            bPlayerInSight = true;
            return;
        }
    }

    PlayerTarget = nullptr;
    bPlayerInSight = false;
}

bool AACombatEnemy::CanSeePlayer(AActor* Player)
{
    // Line trace to check if player is visible
    FHitResult HitResult;
    FVector Start = GetActorLocation() + FVector(0, 0, 50); // Eye level
    FVector End = Player->GetActorLocation();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

    // If we hit the player or nothing, we can see them
    return !bHit || HitResult.GetActor() == Player;
}

void AACombatEnemy::FireAtPlayer()
{
    if (!ProjectileClass || !PlayerTarget) return;

    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastFireTime < (1.0f / FireRate)) return;

    // Spawn projectile
    FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
    FVector DirectionToPlayer = (PlayerTarget->GetActorLocation() - MuzzleLocation).GetSafeNormal();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AFPSProjectile_Bullet* Projectile = GetWorld()->SpawnActor<AFPSProjectile_Bullet>(
        ProjectileClass, MuzzleLocation, DirectionToPlayer.Rotation(), SpawnParams);

    if (Projectile)
    {
        Projectile->FireInDirection(DirectionToPlayer);
        LastFireTime = CurrentTime;
    }
}