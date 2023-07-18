// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataComponent.h"
#include "../Character/MainCharacter.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../Weapon/Weapon.h"

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
	TTuple<bool, int> ItemTuple = MainCharacter->GetInventoryComponent()->AddToInventory(ItemID.RowName.ToString(), Quantity, ItemType);
	if (Weapon/* && Weapon->GetEquippedSlotType() == EEquippedSlotType::EST_Weapon*/)
	{
		MainCharacter->GetInventoryComponent()->AddToWeaponSlot(Weapon, ItemID.RowName.ToString(), Weapon->GetAmmo(), Weapon->GetCarriedAmmo(), ItemType);
	}
}
