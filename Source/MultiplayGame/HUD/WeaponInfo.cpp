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
	DragDropSlot = Cast<UDragDropSlot>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropSlot::StaticClass()));

	if (DragDropSlot)
	{
		DragDropSlot->SetWeaponInfoSlot(this);
	}

	//UpdateWeaponInfo();
}

void UWeaponInfo::NativeConstruct()
{
	Super::NativeConstruct();
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
			}
		}
	}
	OutOperation = DragDropSlot;
}

bool UWeaponInfo::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (SlotForDragDrop->GetEquippedSlotType() == EEquippedSlotType::EST_Weapon && SlotForDragDrop != DragDropSlot)
		{
			if (InventoryComponent)
			{
				InventoryComponent->TransferWeaponInfos(InventoryComponent);
				return true;
			}
		}
		else if (SlotForDragDrop->GetEquippedSlotType() == EEquippedSlotType::EST_AroundItem && SlotForDragDrop->GetItemType() == EItemType::EIT_Weapon)
		{
			if (CombatComponent && InventoryComponent)
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
		}
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
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (OutBorder)
		{
			OutBorder->SetBrushColor(FLinearColor(1.f, 0.3f, 0.f));
		}
	}
}

void UWeaponInfo::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (OutBorder)
		{
			OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
		}
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
	//if (InventoryDataTable)
	//{

	//	//Character = Cast<AMainCharacter>(GetOwningPlayerPawn());
	//	if (Character)
	//	{
	//		if (EquippedSlotType == EEquippedSlotType::EST_Weapon)
	//		{
	//			if (WeaponNum == EWeaponNum::EWN_Weapon1 && Character->GetWeapon1())
	//			{
	//				SetSlot(Character->GetWeapon1()->GetItemDataComponent());
	//			}
	//			else if (WeaponNum == EWeaponNum::EWN_Weapon2 && Character->GetWeapon2())
	//			{
	//				SetSlot(Character->GetWeapon2()->GetItemDataComponent());
	//			}
	//			else
	//			{
	//				SetSlot(nullptr);
	//			}
	//		}
	//		else
	//		{
	//			SetSlot(nullptr);
	//		}
	//	}
	//}
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
				IconImage->SetBrushFromTexture(ItemStruct->Thumbnail);
				IconImage->SetVisibility(ESlateVisibility::Visible);
				AmmoText->SetText(FText::FromString(FString::FromInt(AmmoQuantity) + FString(" / ") + FString::FromInt(CarriedAmmoQuantity)));
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
}

