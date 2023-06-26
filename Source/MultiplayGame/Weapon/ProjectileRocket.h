// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYGAME_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileRocket();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere)
		class URocketMovementComponent* RocketMovementComponent;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* RocketMesh;

	UPROPERTY(EditAnywhere)
		float MinimumDamage = Damage * 0.5f;
	
	UPROPERTY(EditAnywhere)
		float DamageInnerRadius = 200.f;

	UPROPERTY(EditAnywhere)
		float DamageOuterRadius = 500.f;

	UPROPERTY(EditAnywhere)
		float DamageFallOff = 1.f;

};
