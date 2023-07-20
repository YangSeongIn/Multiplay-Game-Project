// Fill out your copyright notice in the Description page of Project Settings.


#include "AroundItemGrid.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "../HUD/InventorySlot.h"
#include "Components/WrapBox.h"

void UAroundItemGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UAroundItemGrid::DisplayOverlappedItems(UInventoryComponent* InventoryComp)
{
	this->InventoryComponent = InventoryComponent;
	ItemGrid->ClearChildren();
	UpdateInventory();
}

void UAroundItemGrid::UpdateInventory()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		InventoryComponent = Character->GetInventoryComponent();
		if (InventoryComponent)
		{
			if (InventorySlotWidgetClass != nullptr)
			{
				TArray<FInventorySlotStruct> Arr = InventoryComponent->GetOverlappedItems();
				for (int i = 0; i < Arr.Num(); i++)
				{
					UInventorySlot* ItemSlot = Cast<UInventorySlot>(CreateWidget(this, InventorySlotWidgetClass));
					ItemSlot->SetItemID(Arr[i].ItemID);
					ItemSlot->SetQuantity(Arr[i].Quantity);
					ItemSlot->SetInventoryComponent(InventoryComponent);
					ItemSlot->SetSlotIndex(i);
					ItemSlot->SetInherenceName(Arr[i].InherenceName);
					ItemSlot->SetItemType(Arr[i].ItemType);
					ItemSlot->SetEquippedSlotType(EEquippedSlotType::EST_AroundItem);
					ItemSlot->SetItem(Arr[i].Item);
					ItemGrid->AddChildToWrapBox(ItemSlot);
				}
				if (!InventoryComponent->OnOverlappedItemUpdate.IsBound())
				{
					InventoryComponent->OnOverlappedItemUpdate.AddUFunction(this, FName("UpdatedInventory"));
				}
			}
		}
	}
}

void UAroundItemGrid::UpdatedInventory()
{
	ItemGrid->ClearChildren();
	UpdateInventory();
}