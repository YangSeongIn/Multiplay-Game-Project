// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Weapon/Weapon.h"
#include "../Character/MainCharacter.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "../Pickups/Item.h"
#include "ItemDataComponent.h"
#include "../Pickups/Pickup.h"
#include "../Pickups/AmmoPickup.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Slots);
}

UTexture2D* UInventoryComponent::GetWeaponImage(AWeapon* WeaponToFind, bool bDetailImage)
{
	FString ItemIDToFind = "";

	if (WeaponToFind == nullptr || WeaponToFind->GetItemDataComponent() == nullptr) 
		return nullptr;

	ItemIDToFind = WeaponToFind->GetItemDataComponent()->GetItemID().RowName.ToString();

	if (ItemIDToFind == "") 
		return nullptr;

	FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemIDToFind), "");

	if (ItemStruct == nullptr) 
		return nullptr;

	if (bDetailImage)
	{
		return ItemStruct->DetailThumbnail;
	}
	else
	{
		return ItemStruct->Thumbnail;
	}
	return nullptr;
}

// when player die
void UInventoryComponent::RemoveAllContents()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		//DropInventoryItemByDragging(i);
		AItem* ItemToSpawn = Slots[i].Item;
		DropPickup(ItemToSpawn, i);
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

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
	UpdateWeaponInfoSlot(CombatComponent->GetEquippedWeapon());
	UpdateWeaponInfoSlot(CombatComponent->GetSecondaryWeapon());
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
	UpdateWeaponInfoSlot(CombatComponent->GetEquippedWeapon());
	UpdateWeaponInfoSlot(CombatComponent->GetSecondaryWeapon());
}

void UInventoryComponent::AddToInventory(AItem* Item, int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName, EWeaponType WeaponType)
{
	FInventorySlotStruct Slot{ Quantity, ItemType, ItemID, InherenceName, Item, WeaponType };
	Slots.Add(Slot);

	if (OnInventoryUpdate.IsBound())
	{
		OnInventoryUpdate.Broadcast();
	}
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

void UInventoryComponent::DEBUGPrintOverlappedItems()
{
	for (int i = 0; i < OverlappedItems.Num(); i++)
	{
		FInventorySlotStruct x = OverlappedItems[i];
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

		UpdateWeaponInfoSlot(CombatComponent->GetEquippedWeapon());
		UpdateWeaponInfoSlot(CombatComponent->GetSecondaryWeapon());
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
		UpdateWeaponInfoSlot(CombatComponent->GetEquippedWeapon());
		UpdateWeaponInfoSlot(CombatComponent->GetSecondaryWeapon());
	}
}

void UInventoryComponent::DropInventoryItemByDragging(int32 ContentIndex)
{
	AItem* ItemToSpawn = Slots[ContentIndex].Item;
	UWorld* World = GetOwner()->GetWorld();
	if (World && ItemToSpawn && Character)
	{
		DropPickup(ItemToSpawn, ContentIndex);
	}

	AWeapon* EquippedWeapon = CombatComponent->GetEquippedWeapon();
	if (EquippedWeapon)
	{
		UpdateWeaponInfoSlot(EquippedWeapon);
	}

	//MulticastBroadCast();
	OnInventoryUpdate.Broadcast();
	OnOverlappedItemUpdate.Broadcast();
}

void UInventoryComponent::MulticastBroadCast_Implementation()
{
	OnInventoryUpdate.Broadcast();
	OnOverlappedItemUpdate.Broadcast();
}

void UInventoryComponent::ServerRemoveItemFromSlot_Implementation(const int32& ContentIndex)
{
	AItem* ItemToSpawn = Slots[ContentIndex].Item;
	UWorld* World = GetOwner()->GetWorld();
	if (World && ItemToSpawn && Character)
	{
		DropPickup(ItemToSpawn, ContentIndex);
	}

	AWeapon* EquippedWeapon = CombatComponent->GetEquippedWeapon();
	if (EquippedWeapon)
	{
		UpdateWeaponInfoSlot(EquippedWeapon);
	}

	//MulticastBroadCast();
	OnInventoryUpdate.Broadcast();
	OnOverlappedItemUpdate.Broadcast();
}

void UInventoryComponent::DropPickup(AItem* ItemToSpawn, const int32& ContentIndex)
{
	APickup* Pickup = Cast<APickup>(ItemToSpawn);
	if (Pickup)
	{
		DropAmmo(Pickup, ContentIndex);
		Pickup->SetActive(true, Character->GetGroundLocation());

		Slots.RemoveAt(ContentIndex);


		if (CombatComponent->GetEquippedWeapon())
		{
			CombatComponent->GetEquippedWeapon()->SetHUDAmmo();
		}
	}
}

void UInventoryComponent::DropAmmo(APickup* Pickup, const int32& ContentIndex)
{
	AAmmoPickup* AmmoPickup = Cast<AAmmoPickup>(Pickup);
	if (CombatComponent && AmmoPickup && AmmoPickup->GetItemDataComponent())
	{
		CombatComponent->SubAmmo(AmmoPickup->GetWeaponType(), Slots[ContentIndex].Quantity);
		AmmoPickup->GetItemDataComponent()->SetQuantity(Slots[ContentIndex].Quantity);
		UpdateWeaponInfoSlot(CombatComponent->GetEquippedWeapon());
		UpdateWeaponInfoSlot(CombatComponent->GetSecondaryWeapon());
	}
}

void UInventoryComponent::UpdateAmmoSlot()
{
	int32 Index = GetAmmoIndexFromInventory();
	if (Index == -1) return;
	Slots[Index].Quantity--;

	if (Slots[Index].Quantity <= 0)
	{
		Slots.RemoveAt(Index);
	}

	OnInventoryUpdate.Broadcast();
	//MulticastBroadCast();
}

int32 UInventoryComponent::GetAmmoIndexFromInventory()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].ItemType == EItemType::EIT_Ammo && CombatComponent && CombatComponent->GetEquippedWeapon() && Slots[i].WeaponType == CombatComponent->GetEquippedWeapon()->GetWeaponType())
		{
			return  i;
		}
	}
	return -1;
}

//void UInventoryComponent::ServerTransferSlots_Implementation(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex)
//{
//	TransferSlots(SourceIndex, SourceInventory, TargetIndex);
//}
//
//void UInventoryComponent::AddOverlappedItem(FString ItemID, int32 Quantity, EItemType ItemType, FString InherenceName, AItem* Item)
//{
//	FInventorySlotStruct SlotStruct;
//	SlotStruct.ItemID = ItemID;
//	SlotStruct.Quantity = Quantity;
//	SlotStruct.ItemType = ItemType;
//	SlotStruct.InherenceName = InherenceName;
//	SlotStruct.Item = Item;
//	OverlappedItems.Add(SlotStruct);
//}
//
//void UInventoryComponent::RemoveOverlappedItem(FString InherenceName)
//{
//	for (int32 i = 0; i < OverlappedItems.Num(); i++)
//	{
//		if (OverlappedItems[i].InherenceName == InherenceName)
//		{
//			OverlappedItems.RemoveAt(i);
//			return;
//		}
//	}
//}

void UInventoryComponent::UpdateWeaponInfoSlot(AWeapon* WeaponToUpdate)
{
	bool bCheckWeapon1 = CombatComponent->GetEquippedWeapon() 
		&& CombatComponent->GetEquippedWeapon()->GetItemDataComponent() 
		&& CombatComponent->GetCarriedAmmoMap().Contains(CombatComponent->GetEquippedWeapon()->GetWeaponType());
	if (bCheckWeapon1)
	{
		WeaponInfos[0].AmmoQuantity = CombatComponent->GetEquippedWeapon()->GetAmmo();
		WeaponInfos[0].CarriedAmmoQuantity = CombatComponent->GetCarriedAmmoMap()[CombatComponent->GetEquippedWeapon()->GetWeaponType()];
		WeaponInfos[0].ItemID = CombatComponent->GetEquippedWeapon()->GetItemDataComponent()->GetItemID().RowName.ToString();
		WeaponInfos[0].ItemType = CombatComponent->GetEquippedWeapon()->GetItemDataComponent()->GetItemType();
		WeaponInfos[0].Weapon = CombatComponent->GetEquippedWeapon();
		WeaponInfos[0].WeaponNum = EWeaponNum::EWN_Weapon1;
	}
	bool bCheckWeapon2 = CombatComponent->GetSecondaryWeapon()
		&& CombatComponent->GetSecondaryWeapon()->GetItemDataComponent()
		&& CombatComponent->GetCarriedAmmoMap().Contains(CombatComponent->GetSecondaryWeapon()->GetWeaponType());
	if (bCheckWeapon2)
	{
		WeaponInfos[1].AmmoQuantity = CombatComponent->GetSecondaryWeapon()->GetAmmo();
		WeaponInfos[1].CarriedAmmoQuantity = CombatComponent->GetCarriedAmmoMap()[CombatComponent->GetSecondaryWeapon()->GetWeaponType()];
		WeaponInfos[1].ItemID = CombatComponent->GetSecondaryWeapon()->GetItemDataComponent()->GetItemID().RowName.ToString();
		WeaponInfos[1].ItemType = CombatComponent->GetSecondaryWeapon()->GetItemDataComponent()->GetItemType();
		WeaponInfos[1].Weapon = CombatComponent->GetSecondaryWeapon();
		WeaponInfos[1].WeaponNum = EWeaponNum::EWN_Weapon2;
	}
	
	if (OnWeaponInfoUpdate.IsBound())
	{
		OnWeaponInfoUpdate.Broadcast();
	}
}
