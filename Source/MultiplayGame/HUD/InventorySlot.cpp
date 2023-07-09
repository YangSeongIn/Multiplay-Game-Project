// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "../Types/InventorySlotStruct.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (InventoryDataTable)
	{
		FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
		if (ItemStruct)
		{
			GLog->Log("11");
			if (IconImage)
			{
				GLog->Log("00");
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

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	/*if (DragItemVisualClass != nullptr)
	{
		DragItemVisual = Cast<UDragItemVisual>(CreateWidget(this, DragItemVisualClass));
		if (DragItemVisual != nullptr)
		{
			DragItemVisual->ItemID = ItemID;
			DragDropOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
			if (DragDropOperation != nullptr)
			{
				DragDropOperation->SetInventoryComponent(InventorySystem);
				DragDropOperation->SetContentIndex(ContentIndex);
				DragDropOperation->DefaultDragVisual = DragItemVisual;
			}
		}
	}
	OutOperation = DragDropOperation;*/
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	/*UItemDragDropOperation* DDO = Cast<UItemDragDropOperation>(InOperation);
	if (DDO != nullptr)
	{
		if (DDO->GetContentIndex() != ContentIndex || DDO->GetInventoryComponent() != InventorySystem)
		{
			GLog->Log("Drop success!");
			InventorySystem->ServerTransferSlots(DDO->GetContentIndex(), DDO->GetInventoryComponent(), ContentIndex);
		}
		else
		{
			GLog->Log("Drop fail!");
		}
	}
	else
	{
		GLog->Log("Drop null!");
	}*/
	return false;
}

void UInventorySlot::SetSlot()
{
	if (InventoryDataTable)
	{
		FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
		if (ItemStruct)
		{
			GLog->Log("11");
			if (IconImage)
			{
				GLog->Log("00");
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