// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundCue;
class UParticleSystem;

UCLASS()
class MULTIPLAYGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastHitImpact(const FHitResult HitResult);

	UPROPERTY(EditAnywhere)
		float Damage = 20.f;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
		UParticleSystem* Tracer;

	UPROPERTY()
		class UParticleSystemComponent* TracerComponent;

	UPROPERTY()
		UParticleSystem* FinalImpactParticles;
	UPROPERTY()
		USoundCue* FinalImpactSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticlesMetal;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticlesBody;

	UPROPERTY(EditAnywhere)
		USoundCue* ImpactSoundMetal;

	UPROPERTY(EditAnywhere)
		USoundCue* ImpactSoundBody;

	void SetBulletMarks(UParticleSystem* Particle, USoundCue* SoundCue);

public:	
	

};
