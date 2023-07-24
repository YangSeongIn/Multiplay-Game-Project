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
				TTuple<bool, int> ItemTuple = MainCharacter->GetInventoryComponent()->AddToInventory(Item, Quantity, ItemType, ItemID.RowName.ToString(), OwningActor->GetName());
				if (ItemType == EItemType::EIT_Ammo)
				{
					UCombatComponent* Combat = MainCharacter->GetCombatComponent();
					if (Combat)
					{
						Combat->PickupAmmo(WeaponType, Quantity);
					}

					// Hard cording. Fix later.
					APickup* Pickup = Cast<APickup>(OwningActor);
					if (Pickup)
					{
						Pickup->SetActive(false);
					}
				}
			}	
		}
		else
		{
			AWeapon* Weapon = Cast<AWeapon>(OwningActor);
			if (Weapon)
			{
				MainCharacter->GetInventoryComponent()->AddToWeaponSlot(Weapon, ItemID.RowName.ToString(), Weapon->GetAmmo(), Weapon->GetCarriedAmmo(), ItemType);
			}
		}
	}
}
