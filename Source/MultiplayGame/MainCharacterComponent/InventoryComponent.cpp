// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Weapon/Weapon.h"
#include "../Character/MainCharacter.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "../Pickups/Item.h"
#include "ItemDataComponent.h"
#include "../Pickups/Pickup.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

UTexture2D* UInventoryComponent::GetWeaponImage(AWeapon* WeaponToFind, bool bDetailImage)
{
	FString ItemIDToFind = "";
	if (WeaponToFind && WeaponToFind->GetItemDataComponent())
	{
		ItemIDToFind = WeaponToFind->GetItemDataComponent()->GetItemID().RowName.ToString();
		if (ItemIDToFind != "")
		{
			FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemIDToFind), "");
			if (ItemStruct)
			{
				if (bDetailImage)
				{
					return ItemStruct->DetailThumbnail;
				}
				else
				{
					return ItemStruct->Thumbnail;
				}
			}
			else
			{
				return nullptr;
			}
		}
	}
	return nullptr;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InventorySize; i++) 
	{
		FInventorySlotStruct s{ 0, EItemType::EIT_MAX, "", "", nullptr };
		Slots.Add(s);
	}

	for (int i = 0; i < 2; i++) 
	{
		FEquippedWeaponSlotStruct EWSlot({ -1, -1, EItemType::EIT_MAX, EWeaponNum(i), "", nullptr});
		WeaponInfos.Add(EWSlot);
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UInventoryComponent::AddToWeaponSlot(AWeapon* Weapon, FString ItemID, int32 AmmoQuantity, int32 CarriedAmmoQuantity, EItemType ItemType)
{
	if (WeaponInfos[0].ItemID == "")
	{
		WeaponInfos[0].ItemID = ItemID;
		WeaponInfos[0].AmmoQuantity = AmmoQuantity;
		WeaponInfos[0].CarriedAmmoQuantity = CarriedAmmoQuantity;
		WeaponInfos[0].ItemType = ItemType;
		WeaponInfos[0].Weapon = Weapon;
		WeaponInfos[0].WeaponNum = EWeaponNum::EWN_Weapon1;
	}
	else if (WeaponInfos[1].ItemID == "")
	{
		WeaponInfos[1].ItemID = ItemID;
		WeaponInfos[1].AmmoQuantity = AmmoQuantity;
		WeaponInfos[1].CarriedAmmoQuantity = CarriedAmmoQuantity;
		WeaponInfos[1].ItemType = ItemType;
		WeaponInfos[1].Weapon = Weapon;
		WeaponInfos[1].WeaponNum = EWeaponNum::EWN_Weapon2;
	}
	if (OnWeaponInfoUpdate.IsBound())
	{
		OnWeaponInfoUpdate.Broadcast();
	}
}

void UInventoryComponent::AddToSelectedWeaponSlot(AItem* Item, EWeaponNum WeaponNum)
{
	int32 n = WeaponNum == EWeaponNum::EWN_Weapon1 ? 0 : 1;
	UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
	AWeapon* Weapon = Cast<AWeapon>(Item);
	if (ItemDataComponent && Weapon && CombatComponent)
	{
		WeaponInfos[n].ItemID = ItemDataComponent->GetItemID().RowName.ToString();
		WeaponInfos[n].AmmoQuantity = Weapon->GetAmmo();
		WeaponInfos[n].CarriedAmmoQuantity = CombatComponent->GetCarriedAmmo(Weapon->GetWeaponType());
		WeaponInfos[n].ItemType = EItemType::EIT_Weapon;
		WeaponInfos[n].Weapon = Cast<AWeapon>(Item);
	}
	if (OnWeaponInfoUpdate.IsBound())
	{
		OnWeaponInfoUpdate.Broadcast();
	}
}

TTuple<bool, int> UInventoryComponent::AddToInventory(AItem* Item, int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName)
{
	int QuantityRemaining = Quantity;
	bool bHasFailed = false;
	while (QuantityRemaining > 0 && !bHasFailed) {
		TTuple<int, bool> CurrentSlot = FindSlot(ItemID);
		if (CurrentSlot.Get<1>())
		{
			AddToStack(CurrentSlot.Get<0>(), 1);
			QuantityRemaining -= 1;
			Slots[CurrentSlot.Get<0>()].Item = Item;
		}
		else
		{
			if (AnyEmptySlotAvailable().Get<0>())
			{
				bool bIsSuccessToCreate = CreateNewStack(1, ItemType, ItemID, InherenceName, Item);
				if (bIsSuccessToCreate)
				{
					QuantityRemaining -= 1;
				}
				else bHasFailed = true;
			}
			else bHasFailed = true;
		}
	}
	
	if (OnInventoryUpdate.IsBound())
	{
		OnInventoryUpdate.Broadcast();
	}
	return MakeTuple(!bHasFailed, QuantityRemaining);
}

void UInventoryComponent::RemoveFromInventory(FString ItemID, int Quantity)
{
	//Slots.Remove(Item);
}

TTuple<int, bool> UInventoryComponent::FindSlot(FString ItemID)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		FInventorySlotStruct x = Slots[i];
		if (x.ItemID == ItemID)
		{
			if (x.Quantity < GetMaxStackSize(ItemID))
			{
				return MakeTuple(i, true);
			}
		}
	}
	return MakeTuple(-1, false);
}

int32 UInventoryComponent::GetMaxStackSize(FString ItemID)
{
	FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
	if (ItemStruct) return ItemStruct->StackSize;
	return -1;
}

void UInventoryComponent::AddToStack(int Idx, int Quantity)
{
	Slots[Idx].Quantity += Quantity;
}

TTuple<bool, int> UInventoryComponent::AnyEmptySlotAvailable()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].Quantity == 0) return MakeTuple(true, i);
	}
	return MakeTuple(false, -1);
}

bool UInventoryComponent::CreateNewStack(int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName, AItem* Item)
{
	if (AnyEmptySlotAvailable().Get<0>())
	{
		Slots[AnyEmptySlotAvailable().Get<1>()] = { Quantity, ItemType, ItemID, InherenceName, Item };
		return true;
	}
	else return false;
}

void UInventoryComponent::DEBUGPrintContents()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		FInventorySlotStruct x = Slots[i];
		GEngine->AddOnScreenDebugMessage(
			-1,
			5,
			FColor::Blue,
			FString::Printf(TEXT("%d : %s, %d"), i, *x.ItemID, x.Quantity)
		);
	}
}

void UInventoryComponent::TransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex)
{
	if (SourceInventory == nullptr || Slots.Num() <= SourceIndex) return;
	FInventorySlotStruct SlotContentTemp = SourceInventory->Slots[SourceIndex];
	if (TargetIndex >= 0)
	{
		if (Slots[TargetIndex].ItemID != SlotContentTemp.ItemID)
		{
			SourceInventory->Slots[SourceIndex] = Slots[TargetIndex];
			Slots[TargetIndex] = SlotContentTemp;
			MulticastUpdate();
			SourceInventory->MulticastUpdate();
		}
	}
}

void UInventoryComponent::TransferWeaponInfos(/*UInventoryComponent* SourceInventory*/)
{
	if (WeaponInfos[0].ItemID != "" || WeaponInfos[1].ItemID != "")
	{
		FEquippedWeaponSlotStruct WeaponSlotStructTemp = WeaponInfos[0];
		WeaponInfos[0] = WeaponInfos[1];
		WeaponInfos[1] = WeaponSlotStructTemp;
		WeaponInfos[0].WeaponNum = EWeaponNum::EWN_Weapon1;
		WeaponInfos[1].WeaponNum = EWeaponNum::EWN_Weapon2;

		if (OnWeaponInfoUpdate.IsBound())
		{
			OnWeaponInfoUpdate.Broadcast();
		}
		if (Character && Character->GetCombatComponent())
		{
			if (Character->HasAuthority())
			{
				Character->GetCombatComponent()->SwapTwoWeapons();
			}
			else
			{
				ServerSwapTwoWeapons();
			}
		}
	}
}

void UInventoryComponent::ServerSwapTwoWeapons_Implementation()
{
	if (Character && Character->GetCombatComponent())
	{
		Character->GetCombatComponent()->SwapTwoWeapons();
	}
}

void UInventoryComponent::MulticastUpdate_Implementation()
{
	if (OnInventoryUpdate.IsBound())
	{
		OnInventoryUpdate.Broadcast();
	}
}

void UInventoryComponent::DropWeaponByDragging(EWeaponNum WeaponNum)
{
	if (CombatComponent)
	{
		CombatComponent->DropWeaponByDragging(WeaponNum);
		int32 Num = WeaponNum == EWeaponNum::EWN_Weapon1 ? 0 : 1;
		WeaponInfos[Num] = { -1, -1, EItemType::EIT_MAX, EWeaponNum(Num), "", nullptr };
		if (OnWeaponInfoUpdate.IsBound())
		{
			OnWeaponInfoUpdate.Broadcast();
		}
	}
}

void UInventoryComponent::DropInventoryItemByDragging(int32 ContentIndex)
{
	AItem* ItemToSpawn = Slots[ContentIndex].Item;
	UWorld* World = GetOwner()->GetWorld();
	if (World && ItemToSpawn && Character)
	{
		APickup* Pickup = Cast<APickup>(ItemToSpawn);
		if (Pickup)
		{
			Pickup->SetActive(true, Character->GetGroundLocation());
		}
	}

	Slots[ContentIndex] = { 0, EItemType::EIT_MAX, "", "", nullptr };

	if (OnInventoryUpdate.IsBound() && OnOverlappedItemUpdate.IsBound())
	{
		OnInventoryUpdate.Broadcast();
		OnOverlappedItemUpdate.Broadcast();
	}
}

void UInventoryComponent::ServerTransferSlots_Implementation(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex)
{
	TransferSlots(SourceIndex, SourceInventory, TargetIndex);
}

void UInventoryComponent::AddOverlappedItem(FString ItemID, int32 Quantity, EItemType ItemType, FString InherenceName, AItem* Item)
{
	FInventorySlotStruct SlotStruct;
	SlotStruct.ItemID = ItemID;
	SlotStruct.Quantity = Quantity;
	SlotStruct.ItemType = ItemType;
	SlotStruct.InherenceName = InherenceName;
	SlotStruct.Item = Item;
	OverlappedItems.Add(SlotStruct);
}

void UInventoryComponent::RemoveOverlappedItem(FString InherenceName)
{
	for (int32 i = 0; i < OverlappedItems.Num(); i++)
	{
		if (OverlappedItems[i].InherenceName == InherenceName)
		{
			OverlappedItems.RemoveAt(i);
		}
	}
}

void UInventoryComponent::UpdateWeaponInfoSlot(AWeapon* EquippedWeapon, int32 Ammo, int32 CarriedAmmo)
{
	for (FEquippedWeaponSlotStruct& Slot : WeaponInfos)
	{
		if (Slot.Weapon == EquippedWeapon)
		{
			Slot.AmmoQuantity = Ammo;
			Slot.CarriedAmmoQuantity = CarriedAmmo;
		}
	}
	if (OnWeaponInfoUpdate.IsBound())
	{
		OnWeaponInfoUpdate.Broadcast();
	}
}
