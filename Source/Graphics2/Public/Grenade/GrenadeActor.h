// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "NiagaraSystem.h"
#include "GrenadeActor.generated.h"


UCLASS()
class GRAPHICS2_API AGrenadeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	//Explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float FuseTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionDamage = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionForce = 1000.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	TSubclassOf<class UDamageType> DamageTypeClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion Effects")
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion Effects")
	TSubclassOf<UCameraShakeBase> ExplosionCameraShake;

private:
	FTimerHandle FuseTimerHandle;

	void Explode();

	void SpawnExplosionEffect();

	void ApplyExplosionForce(const FVector& ExplosionCenter);
};
