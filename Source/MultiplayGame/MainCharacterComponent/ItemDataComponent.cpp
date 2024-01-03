// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataComponent.h"
#include "../Character/MainCharacter.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../Weapon/Weapon.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "../Pickups/Pickup.h"

UItemDataComponent::UItemDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

EWeaponType UItemDataComponent::GetWeaponType()
{
	if (ItemType == EItemType::EIT_Ammo)
	{
		return WeaponType;
	}
	return EWeaponType::EWT_MAX;
}


void UItemDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemDataComponent::Interact(AMainCharacter* MainCharacter)
{
	if (MainCharacter == nullptr || MainCharacter->GetInventoryComponent() == nullptr) return;
	if (OwningActor)
	{
		if (ItemType != EItemType::EIT_Weapon)
		{
			AItem* Item = Cast<AItem>(OwningActor);
			if (Item)
			{
				MainCharacter->GetInventoryComponent()->AddToInventory(Item, Quantity, ItemType, ItemID.RowName.ToString(), OwningActor->GetName(), WeaponType);
				AddAmmoToInventory(MainCharacter);
				SetPickupActiveFalse();
			}	
		}
		else
		{
			AWeapon* Weapon = Cast<AWeapon>(OwningActor);
			if (Weapon)
			{
				MainCharacter->GetInventoryComponent()->AddToWeaponSlot(Weapon, ItemID.RowName.ToString(), 0, Weapon->GetCarriedAmmo(), ItemType);
			}
		}
	}
}

void UItemDataComponent::SetPickupActiveFalse()
{
	APickup* Pickup = Cast<APickup>(OwningActor);
	if (Pickup)
	{
		Pickup->SetActive(false);
	}
}

void UItemDataComponent::AddAmmoToInventory(AMainCharacter* MainCharacter)
{
	if (ItemType == EItemType::EIT_Ammo)
	{
		UCombatComponent* Combat = MainCharacter->GetCombatComponent();
		if (Combat)
		{
			Combat->PickupAmmo(WeaponType, Quantity);
		}
	}
}
