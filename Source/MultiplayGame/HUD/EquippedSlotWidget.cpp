// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippedSlotWidget.h"
#include "EquippedSlot.h"

void UEquippedSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (EquippedSlot_Body && EquippedSlot_Head && EquippedSlot_Weapon1 && EquippedSlot_Weapon2)
	{
		EquippedSlot_Head->SetEquippedSlotWidget(this);
		EquippedSlot_Body->SetEquippedSlotWidget(this);
		EquippedSlot_Weapon1->SetEquippedSlotWidget(this);
		EquippedSlot_Weapon2->SetEquippedSlotWidget(this);
	}
}

void UEquippedSlotWidget::SwapEquippedWeaponSlot()
{
	if (EquippedSlot_Weapon1 && EquippedSlot_Weapon2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Equpped")));
		UEquippedSlot* EquippedSlotTemp = EquippedSlot_Weapon1;
		EquippedSlot_Weapon1 = EquippedSlot_Weapon2;
		EquippedSlot_Weapon2 = EquippedSlotTemp;
	}
}
