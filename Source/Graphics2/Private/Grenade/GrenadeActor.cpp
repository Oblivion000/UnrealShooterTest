#include "Grenade/GrenadeActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AGrenadeActor::AGrenadeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Mesh setup
	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	SetRootComponent(GrenadeMesh);

	GrenadeMesh->SetSimulatePhysics(false);
	GrenadeMesh->SetEnableGravity(false);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GrenadeMesh->SetCollisionObjectType(ECC_PhysicsBody);
	GrenadeMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// Projectile movement setup
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.6f;
	ProjectileMovement->Friction = 0.2f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->SetUpdatedComponent(GrenadeMesh);
}

// Called when the game starts or when spawned
void AGrenadeActor::BeginPlay()
{
	Super::BeginPlay();

	// Start fuse countdown
	GetWorldTimerManager().SetTimer(FuseTimerHandle, this, &AGrenadeActor::Explode, FuseTime);
}

// Handle grenade explosion logic
void AGrenadeActor::Explode()
{
    const FVector ExplosionLocation = GetActorLocation();

    // 1. Apply radial damage
    UGameplayStatics::ApplyRadialDamage(
        this,
        ExplosionDamage,
        ExplosionLocation,
        ExplosionRadius,
        UDamageType::StaticClass(),
        TArray<AActor*>(), // Ignore actors
        this,
        GetInstigatorController(),
        true
    );

    // 2. Sweep for actors/components to apply force to
    TArray<FHitResult> HitResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);

    if (GetWorld()->SweepMultiByChannel(
        HitResults,
        ExplosionLocation,
        ExplosionLocation, // Start and End the same for overlap sphere
        FQuat::Identity,
        ECC_Pawn, // Or ECC_PhysicsBody if you want to affect physics objects
        Sphere))
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            UPrimitiveComponent* HitComp = Hit.GetComponent();

            if (!HitActor) continue;

            // Push characters
            if (ACharacter* HitCharacter = Cast<ACharacter>(HitActor))
            {
                FVector LaunchDir = (HitCharacter->GetActorLocation() - ExplosionLocation).GetSafeNormal();
                FVector LaunchVelocity = LaunchDir * ExplosionForce;

                HitCharacter->LaunchCharacter(LaunchVelocity, true, true);


                //Camera Shake
				APlayerController* PC = Cast<APlayerController>(HitCharacter->GetController());
                if(PC && ExplosionCameraShake)
                {
					UE_LOG(LogTemp, Warning, TEXT("GrenadeActor: Playing camera shake for player controller."));
                    PC->ClientStartCameraShake(ExplosionCameraShake, 2.0f);
				}
            }
            // Push physics simulating components
            else if (HitComp && HitComp->IsSimulatingPhysics())
            {
                HitComp->AddRadialImpulse(
                    ExplosionLocation,
                    ExplosionRadius,
                    ExplosionForce,
                    ERadialImpulseFalloff::RIF_Linear,
                    true
                );
            }
        }
    }

    // 3. Play explosion effect
    if (ExplosionEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this,
            ExplosionEffect,
            ExplosionLocation,
            GetActorRotation(),
            FVector(1.0f)
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GrenadeActor: ExplosionEffect not set!"));
    }

    // 4. Destroy grenade
    Destroy();
}


void AGrenadeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Optional: Update any visual effects or logic here
}


