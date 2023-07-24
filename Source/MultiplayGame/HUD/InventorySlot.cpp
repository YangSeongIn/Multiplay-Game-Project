// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "../Types/InventorySlotStruct.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../HUD/DragItemVisual.h"
#include "../DragDrop/DragDropSlot.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "Components/Border.h"
#include "EquippedSlot.h"
#include "EquippedSlotWidget.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (InventoryDataTable)
	{
		FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
		if (ItemStruct)
		{
			if (IconImage)
			{
				IconImage->SetBrushFromTexture(ItemStruct->Thumbnail);
				IconImage->SetVisibility(ESlateVisibility::Visible);
			}
			if (QuantityText)
			{
				QuantityText->SetText(FText::FromString(FString::FromInt(Quantity)));
				QuantityText->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if (IconImage)
			{
				IconImage->SetVisibility(ESlateVisibility::Hidden);
			}
			if (QuantityText)
			{
				QuantityText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

FReply UInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	if (ItemID == "") return FReply::Unhandled();
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Reply;
}

FReply UInventorySlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (OutBorder && InBorder)
	{
		OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
		InBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
	}
	return Reply;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (DragItemVisualClass)
	{
		DragItemVisual = Cast<UDragItemVisual>(CreateWidget(this, DragItemVisualClass));
		if (DragItemVisual)
		{
			DragItemVisual->SetItemID(ItemID);
			DragDropSlot = Cast<UDragDropSlot>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropSlot::StaticClass()));
			if (DragDropSlot)
			{
				DragDropSlot->SetInventoryComponent(InventoryComponent);
				DragDropSlot->SetContentIndex(SlotIndex);
				DragDropSlot->DefaultDragVisual = DragItemVisual;
				DragDropSlot->SetItemID(ItemID);
				DragDropSlot->SetItemType(ItemType);
				DragDropSlot->SetInherenceName(InherenceName);
				DragDropSlot->SetItem(Item);
				DragDropSlot->SetEquippedSlotType(EquippedSlotType);
			}
		}
	}
	OutOperation = DragDropSlot;
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (SlotForDragDrop->GetEquippedSlotType() == EEquippedSlotType::EST_Inventory)
		{
			if (SlotForDragDrop->GetContentIndex() != SlotIndex || SlotForDragDrop->GetInventoryComponent() != InventoryComponent)
			{
				InventoryComponent->ServerTransferSlots(SlotForDragDrop->GetContentIndex(), SlotForDragDrop->GetInventoryComponent(), SlotIndex);
			}
		}
		else if (SlotForDragDrop->GetEquippedSlotType() == EEquippedSlotType::EST_AroundItem)
		{
			AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Character)
			{
				Character->EquipButtonPressed();
			}
		}
	}
	return false;
}

void UInventorySlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (OutBorder)
		{
			OutBorder->SetBrushColor(FLinearColor(1.f, 0.3f, 0.f));
			InBorder->SetBrushColor(FLinearColor(1.f, 0.3f, 0.f));
		}
	}
}

void UInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UDragDropSlot* SlotForDragDrop = Cast<UDragDropSlot>(InOperation);
	if (SlotForDragDrop)
	{
		if (OutBorder && InBorder)
		{
			OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
			InBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f));
		}
	}
}

void UInventorySlot::SetQuantity(int32 TargetQuantity)
{
	// QuantityText->SetText(FText::FromString(FString::FromInt(TargetQuantity)));
	Quantity = TargetQuantity;
}

void UInventorySlot::SetSlot()
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
			}
			if (QuantityText)
			{
				QuantityText->SetText(FText::FromString(FString::FromInt(Quantity)));
				QuantityText->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if (IconImage)
			{
				IconImage->SetVisibility(ESlateVisibility::Hidden);
			}
			if (QuantityText)
			{
				QuantityText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}