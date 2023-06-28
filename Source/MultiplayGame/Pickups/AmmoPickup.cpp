// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "../Character/MainCharacter.h"

AAmmoPickup::AAmmoPickup()
{
}

void AAmmoPickup::BeginPlay()
{

}

void AAmmoPickup::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	GLog->Log("111");
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		UCombatComponent* Combat = MainCharacter->GetCombatComponent();
		if (Combat)
		{
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
		Destroy();
	}
}

void AAmmoPickup::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
