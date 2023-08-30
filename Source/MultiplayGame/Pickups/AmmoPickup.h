// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "../Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

UCLASS()
class MULTIPLAYGAME_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
		int32 AmmoAmount = 30;

	UPROPERTY(EditAnywhere)
		EWeaponType WeaponType;

public:
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; };
};
