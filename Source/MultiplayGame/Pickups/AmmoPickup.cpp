// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "../Character/MainCharacter.h"

void AAmmoPickup::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	/*AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		UCombatComponent* Combat = MainCharacter->GetCombatComponent();
		if (Combat)
		{
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}*/
}
