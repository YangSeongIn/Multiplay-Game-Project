// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGrid.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../HUD/InventorySlot.h"
#include "../MainCharacterComponent/CombatComponent.h"

void UInventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	// UpdateInventory();
}

void UInventoryGrid::NativeDestruct()
{
	InventoryComponent->OnInventoryUpdate.Clear();
	Super::NativeDestruct();
}

void UInventoryGrid::DisplayInventory(UInventoryComponent* InventoryComp)
{
	this->InventoryComponent = InventoryComponent;
	ItemGrid->ClearChildren();
	UpdateInventory();
}

void UInventoryGrid::UpdateInventory()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		InventoryComponent = Character->GetInventoryComponent();
		if (InventoryComponent)
		{
			if (InventorySlotWidgetClass != nullptr)
			{
				TArray<FInventorySlotStruct> Arr = InventoryComponent->GetContents();
				for (int i = 0; i < Arr.Num(); i++)
				{
					UInventorySlot* ItemSlot = Cast<UInventorySlot>(CreateWidget(this, InventorySlotWidgetClass));
					ItemSlot->SetItemID(Arr[i].ItemID);
					ItemSlot->SetQuantity(Arr[i].Quantity);
					ItemSlot->SetInventoryComponent(InventoryComponent);
					ItemSlot->SetSlotIndex(i);
					ItemSlot->SetItemType(Arr[i].ItemType);
					ItemGrid->AddChildToWrapBox(ItemSlot);
				}
				if (!InventoryComponent->OnInventoryUpdate.IsBound())
				{
					InventoryComponent->OnInventoryUpdate.AddUFunction(this, FName("UpdatedInventory"));
				}
			}
		}
	}
}

void UInventoryGrid::UpdatedInventory()
{
	ItemGrid->ClearChildren();
	UpdateInventory();
}
