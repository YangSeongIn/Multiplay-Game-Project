// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryEquippedVerticalBox.h"
#include "../HUD/EquippedSlot.h"

void UInventoryEquippedVerticalBox::SwapSlot()
{
	UEquippedSlot* EquippedSlotTemp = EquippedSlot1;
	EquippedSlot1 = EquippedSlot2;
	EquippedSlot2 = EquippedSlotTemp;
}
