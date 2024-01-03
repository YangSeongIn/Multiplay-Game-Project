// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYGAME_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget);

private:
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere)
	class USoundCue* HitSound;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnHitImpact(const FHitResult HitResult);
	void SetBulletMarks(UParticleSystem* Particle, USoundCue* SoundCue, const FHitResult HitResult);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticlesMetal;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticlesBody;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSoundMetal;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSoundBody;
};
