// Fill out your copyright notice in the Description page of Project Settings.


#include "DragItemVisual.h"
#include "../Types/InventorySlotStruct.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"

void UDragItemVisual::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (InventoryDataTable)
	{
		FItemStruct* ItemStruct = InventoryDataTable->FindRow<FItemStruct>(FName(ItemID), "");
		if (ItemStruct)
		{
			ItemIcon->SetBrushFromTexture(ItemStruct->Thumbnail);
		}
	}
}

void UDragItemVisual::NativeConstruct()
{
	Super::NativeConstruct();

}
