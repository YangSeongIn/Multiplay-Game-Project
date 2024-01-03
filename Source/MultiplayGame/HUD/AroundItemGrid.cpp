// Fill out your copyright notice in the Description page of Project Settings.


#include "AroundItemGrid.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "../HUD/InventorySlot.h"
#include "Components/WrapBox.h"
#include "../DragDrop/DragDropSlot.h"
#include "../Pickups/Item.h"
#include "../MainCharacterComponent/ItemDataComponent.h"

void UAroundItemGrid::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAroundItemGrid::NativeDestruct()
{
	InventoryComponent->OnOverlappedItemUpdate.Clear();
	Super::NativeDestruct();
}

bool UAroundItemGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop && InventoryComponent)
	{
		switch(SlotForDragDrop->GetEquippedSlotType())
		{ 
		case EEquippedSlotType::EST_Weapon:
			InventoryComponent->DropWeaponByDragging(SlotForDragDrop->GetWeaponNum());
			break;
		case EEquippedSlotType::EST_Inventory:
			InventoryComponent->ServerRemoveItemFromSlot(SlotForDragDrop->GetContentIndex());
			break;
		default:
			break;
		}
	}
	return false;
}

void UAroundItemGrid::DisplayOverlappedItems(UInventoryComponent* InventoryComp)
{
	ClientUpdateInventory();
}

void UAroundItemGrid::ClientUpdateInventory_Implementation()
{
	this->InventoryComponent = InventoryComponent;
	ItemGrid->ClearChildren();
	UpdateInventory();
}

void UAroundItemGrid::UpdateInventory()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character == nullptr) return;
	InventoryComponent = Character->GetInventoryComponent();
	if (InventoryComponent == nullptr || InventorySlotWidgetClass == nullptr) return;
	TArray<AItem*> Arr = Character->GetOverlappingItems();
	for (int i = 0; i < Arr.Num(); i++)
	{
		UInventorySlot* ItemSlot = Cast<UInventorySlot>(CreateWidget(this, InventorySlotWidgetClass));
		ItemSlot->SetItemID(Arr[i]->GetItemDataComponent()->GetItemID().RowName.ToString());
		ItemSlot->SetQuantity(Arr[i]->GetItemDataComponent()->GetQuantity());
		ItemSlot->SetInventoryComponent(InventoryComponent);
		ItemSlot->SetSlotIndex(i);
		ItemSlot->SetInherenceName(Arr[i]->GetName());
		ItemSlot->SetItemType(Arr[i]->GetItemDataComponent()->GetItemType());
		ItemSlot->SetEquippedSlotType(EEquippedSlotType::EST_AroundItem);
		ItemSlot->SetItem(Arr[i]);
		ItemGrid->AddChildToWrapBox(ItemSlot);
	}
	if (!InventoryComponent->OnOverlappedItemUpdate.IsBound())
	{
		InventoryComponent->OnOverlappedItemUpdate.AddUFunction(this, FName("UpdatedInventory"));
	}
}

void UAroundItemGrid::UpdatedInventory()
{
	ItemGrid->ClearChildren();
	UpdateInventory();
}
