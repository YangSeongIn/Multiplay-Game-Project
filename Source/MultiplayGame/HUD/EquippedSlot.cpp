// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippedSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../Character/MainCharacter.h"
#include "Engine/DataTable.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../Weapon/Weapon.h"
#include "../MainCharacterComponent/ItemDataComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "../HUD/DragItemVisual.h"
#include "../DragDrop/DragDropSlot.h"
#include "Components/Border.h"
#include "../HUD/InventoryEquippedVerticalBox.h"
#include "EquippedSlotWidget.h"
#include "../MainCharacterComponent/CombatComponent.h"

void UEquippedSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	DragDropSlot = Cast<UDragDropSlot>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropSlot::StaticClass()));

	/*if (DragDropSlot)
	{
		DragDropSlot->SetEquippedSlot(this);
	}*/

	UpdateSlot();
}

void UEquippedSlot::UpdateSlot()
{
	if (InventoryDataTable)
	{

		Character = Cast<AMainCharacter>(GetOwningPlayerPawn());
		if (Character)
		{
			if (EquippedSlotType == EEquippedSlotType::EST_Weapon)
			{
				if (WeaponNum == EWeaponNum::EWN_Weapon1 && Character->GetWeapon1())
				{
					SetSlot(Character->GetWeapon1()->GetItemDataComponent());
				}
				else if (WeaponNum == EWeaponNum::EWN_Weapon2 && Character->GetWeapon2())
				{
					SetSlot(Character->GetWeapon2()->GetItemDataComponent());
				}
				else
				{
					SetSlot(nullptr);
				}
			}
			else
			{
				SetSlot(nullptr);
			}
		}
	}
}

FReply UEquippedSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	if (ItemID == "") return FReply::Unhandled();
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Reply;
}

void UEquippedSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (DragItemVisualClass)
	{
		DragItemVisual = Cast<UDragItemVisual>(CreateWidget(this, DragItemVisualClass));
		if (DragItemVisual)
		{
			DragItemVisual->SetItemID(ItemID);
			if (DragDropSlot)
			{
				DragDropSlot->SetEquippedSlotType(EquippedSlotType);
				DragDropSlot->DefaultDragVisual = DragItemVisual;
			}
		}
	}
	OutOperation = DragDropSlot;
}

bool UEquippedSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (SlotForDragDrop->GetEquippedSlotType() == EEquippedSlotType::EST_Weapon && SlotForDragDrop != DragDropSlot)
		{
			if (EquippedSlotWidget)
			{
				EquippedSlotWidget->SwapEquippedWeaponSlot();
				return true;
			}
		}
	}
	return false;
}

void UEquippedSlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (InBorder)
		{
			InBorder->SetBrushColor(FLinearColor(1.f, 0.3f, 0.f));
		}
	}
}

void UEquippedSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (InBorder)
		{
			InBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
		}
	}
}

void UEquippedSlot::SetSlot(UItemDataComponent* ItemDataComponent)
{
	if (ItemDataComponent)
	{
		ItemID = ItemDataComponent->GetItemID().RowName.ToString();
		FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
		if (ItemStruct)
		{
			if (IconImage)
			{
				IconImage->SetBrushFromTexture(ItemStruct->Thumbnail);
				IconImage->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if (IconImage)
			{
				IconImage->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else
	{
		if (IconImage)
		{
			IconImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
