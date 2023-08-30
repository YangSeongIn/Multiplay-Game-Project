// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInfo.h"
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
#include "../HUD/InventoryWeaponInfo.h"

void UWeaponInfo::NativePreConstruct()
{
	Super::NativePreConstruct();
	/*DragDropSlot = Cast<UDragDropSlot>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropSlot::StaticClass()));

	if (DragDropSlot)
	{
		DragDropSlot->SetWeaponInfoSlot(this);
	}

	UpdateWeaponInfo();*/
}

void UWeaponInfo::NativeConstruct()
{
	Super::NativeConstruct();
	DragDropSlot = Cast<UDragDropSlot>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropSlot::StaticClass()));

	if (DragDropSlot)
	{
		DragDropSlot->SetWeaponInfoSlot(this);
	}
	UpdateWeaponInfo();
}

FReply UWeaponInfo::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	if (ItemID == "") return FReply::Unhandled();
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Reply;
}

FReply UWeaponInfo::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (OutBorder)
	{
		OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
	}
	return Reply;
}

void UWeaponInfo::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
				DragDropSlot->SetEquippedSlotType(EEquippedSlotType::EST_Weapon);
				DragDropSlot->SetItemType(ItemType);
				DragDropSlot->SetWeaponNum(WeaponNum);
			}
		}
	}
	OutOperation = DragDropSlot;
}

bool UWeaponInfo::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop && InventoryComponent)
	{
		switch (SlotForDragDrop->GetEquippedSlotType())
		{
		case EEquippedSlotType::EST_Weapon:
			if (CombatComponent && SlotForDragDrop != DragDropSlot)
			{
				InventoryComponent->TransferWeaponInfos();
			}
			break;
		case EEquippedSlotType::EST_AroundItem:
			if (SlotForDragDrop->GetItemType() == EItemType::EIT_Weapon && CombatComponent)
			{
				if (GetOwningPlayerPawn()->HasAuthority())
				{

					CombatComponent->EquipWeaponByAroundItem(WeaponNum, SlotForDragDrop->GetInherenceName(), SlotForDragDrop->GetItem());
				}
				else
				{
					ServerEquipWeaponByAroundItem(SlotForDragDrop);
				}

				InventoryComponent->AddToSelectedWeaponSlot(SlotForDragDrop->GetItem(), WeaponNum);
			}
			break;
		default:
			break;
		}
	}

	if (OutBorder)
	{
		OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
	}
	return false;
}

void UWeaponInfo::ServerEquipWeaponByAroundItem_Implementation(UDragDropSlot* SlotForDragDrop)
{
	CombatComponent->EquipWeaponByAroundItem(WeaponNum, SlotForDragDrop->GetInherenceName(), SlotForDragDrop->GetItem());
}

void UWeaponInfo::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	if (OutBorder)
	{
		OutBorder->SetBrushColor(FLinearColor(1.f, 0.3f, 0.f, 0.8f));
	}
}

void UWeaponInfo::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (OutBorder)
	{
		OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
	}
}

void UWeaponInfo::SetSlot(UItemDataComponent* ItemDataComponent)
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
			AmmoText->SetText(FText::FromString(FString("- / -")));
		}
	}
}

void UWeaponInfo::UpdateSlot()
{
	
}

void UWeaponInfo::UpdateWeaponInfo()
{
	if (InventoryDataTable)
	{
		FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
		if (ItemStruct)
		{
			if (IconImage)
			{
				IconImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 1));
				IconImage->SetBrushFromTexture(ItemStruct->Thumbnail);
				IconImage->SetVisibility(ESlateVisibility::Visible);
				AmmoText->SetText(FText::FromString(FString::FromInt(AmmoQuantity) + FString(" / ") + FString::FromInt(CarriedAmmoQuantity)));
			}
		}
		else
		{
			if (IconImage)
			{
				IconImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 0));
				AmmoText->SetText(FText::FromString(FString("")));
			}
		}
	}
}

