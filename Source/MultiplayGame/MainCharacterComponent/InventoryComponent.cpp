// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Weapon/Weapon.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InventorySize; i++) {
		FInventorySlotStruct s{ "", 0 };
		Slots.Add(s);
	}

	for (int i = 0; i < 2; i++) {
		FEquippedWeaponSlotStruct EWSlot({ "", -1, -1 });
		WeaponInfos.Add(EWSlot);
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UInventoryComponent::AddToWeaponSlot(AWeapon* Weapon, FString ItemID, int32 AmmoQuantity, int32 CarriedAmmoQuantity)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("2, %d"), WeaponInfos.Num()));
	if (WeaponInfos[0].ItemID == "")
	{
		WeaponInfos[0].ItemID = ItemID;
		WeaponInfos[0].AmmoQuantity = AmmoQuantity;
		WeaponInfos[0].CarriedAmmoQuantity = CarriedAmmoQuantity;
	}
	else if (WeaponInfos[1].ItemID == "")
	{
		WeaponInfos[1].ItemID = ItemID;
		WeaponInfos[1].AmmoQuantity = AmmoQuantity;
		WeaponInfos[1].CarriedAmmoQuantity = CarriedAmmoQuantity;
	}
}

TTuple<bool, int> UInventoryComponent::AddToInventory(FString ItemID, int Quantity)
{
	int QuantityRemaining = Quantity;
	bool bHasFailed = false;
	while (QuantityRemaining > 0 && !bHasFailed) {
		TTuple<int, bool> CurrentSlot = FindSlot(ItemID);
		if (CurrentSlot.Get<1>())
		{
			AddToStack(CurrentSlot.Get<0>(), 1);
			QuantityRemaining -= 1;
		}
		else
		{
			if (AnyEmptySlotAvailable().Get<0>())
			{
				bool bIsSuccessToCreate = CreateNewStack(ItemID, 1);
				if (bIsSuccessToCreate)
				{
					QuantityRemaining -= 1;
				}
				else bHasFailed = true;
			}
			else bHasFailed = true;
		}
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

bool UInventoryComponent::CreateNewStack(FString ItemID, int Quantity)
{
	if (AnyEmptySlotAvailable().Get<0>())
	{
		Slots[AnyEmptySlotAvailable().Get<1>()] = { ItemID, Quantity };
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

void UInventoryComponent::MulticastUpdate_Implementation()
{
	if (OnInventoryUpdate.IsBound())
	{
		OnInventoryUpdate.Broadcast();
	}
}

void UInventoryComponent::ServerTransferSlots_Implementation(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex)
{
	TransferSlots(SourceIndex, SourceInventory, TargetIndex);
}


