// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippedSlotWidget.h"
#include "EquippedSlot.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "Components/Image.h"
#include "../DragDrop/DragDropSlot.h"

void UEquippedSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (EquippedSlot_Body && EquippedSlot_Head/* && EquippedSlot_Weapon1 && EquippedSlot_Weapon2*/)
	{
		EquippedSlot_Head->SetEquippedSlotWidget(this);
		EquippedSlot_Body->SetEquippedSlotWidget(this);
		//EquippedSlot_Weapon1->SetEquippedSlotWidget(this);
		//EquippedSlot_Weapon2->SetEquippedSlotWidget(this);
	}
}

void UEquippedSlotWidget::SwapEquippedWeaponSlot()
{
	//if (EquippedSlot_Weapon1 || EquippedSlot_Weapon2)
	//{
	//	//GEngine->
	// ScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("%s"), *EquippedSlot_Weapon1->GetName()));
	//	UEquippedSlot* EquippedSlotTemp = EquippedSlot_Weapon1;
	//	EquippedSlot_Weapon1 = EquippedSlot_Weapon2;
	//	EquippedSlot_Weapon2 = EquippedSlotTemp;

	//	if (EquippedSlot_Weapon1)
	//	{
	//		EquippedSlot_Weapon1->UpdateSlot();
	//	}
	//	if (EquippedSlot_Weapon2)
	//	{
	//		EquippedSlot_Weapon2->UpdateSlot(); 
	//	}

	//	
	//	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("%s"), *EquippedSlot_Weapon1->GetName()));
	//	AMainCharacter* Character = Cast<AMainCharacter>(GetOwningPlayerPawn());
	//	if (Character && Character->GetCombatComponent())
	//	{
	//		if (Character->HasAuthority())
	//		{
	//			Character->GetCombatComponent()->SwapTwoWeapons();
	//		}
	//		else
	//		{
	//			//ServerSwapTwoWeapons(Character);
	//		}
	//	}
	//}

}

//void UEquippedSlotWidget::SetSlotWeapon1(class UDragDropSlot* NewSlot)
//{
//	EquippedSlot_Weapon1->SetItemID(NewSlot->GetItemID());
//	EquippedSlot_Weapon1->UpdateSlot();
//
//}
//
//void UEquippedSlotWidget::SetSlotWeapon2(class UDragDropSlot* NewSlot)
//{
//	EquippedSlot_Weapon2->SetItemID(NewSlot->GetItemID());
//	EquippedSlot_Weapon2->UpdateSlot();
//
//}
//
//void UEquippedSlotWidget::ServerSwapTwoWeapons_Implementation(AMainCharacter* Character)
//{
//	if (Character == nullptr || Character->GetCombatComponent() == nullptr) return;
//	Character->GetCombatComponent()->SwapTwoWeapons();
//}


