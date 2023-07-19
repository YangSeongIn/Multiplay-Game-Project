// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWeaponInfo.h"
#include "Components/VerticalBox.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../HUD/InventorySlot.h"
#include "../HUD/WeaponInfo.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../MainCharacterComponent/CombatComponent.h"

void UInventoryWeaponInfo::NativePreConstruct()
{
	Super::NativePreConstruct();

	//UpdateWeaponInfo();
}

void UInventoryWeaponInfo::DisplayWeaponInfo(UInventoryComponent* InventoryComp)
{
	this->InventoryComponent = InventoryComp;
	WeaponInfoGrid->ClearChildren();
	UpdateWeaponInfo();
}

void UInventoryWeaponInfo::UpdateWeaponInfo()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		InventoryComponent = Character->GetInventoryComponent();
		CombatComponent = Character->GetCombatComponent();
		if (InventoryComponent)
		{
			if (WeaponInfoClass)
			{
				TArray<FEquippedWeaponSlotStruct> Arr = InventoryComponent->GetWeaponInfos();
				for (int i = 0; i < 2; i++)
				{
					UWeaponInfo* WeaponInfo = Cast<UWeaponInfo>(CreateWidget(this, WeaponInfoClass));
					WeaponInfo->SetItemID(Arr[i].ItemID);
					WeaponInfo->SetInventoryComponent(InventoryComponent);
					WeaponInfo->SetCombatComponent(CombatComponent);
					WeaponInfo->SetAmmoQuantity(Arr[i].AmmoQuantity);
					WeaponInfo->SetCarriedAmmoQuantity(Arr[i].CarriedAmmoQuantity);
					WeaponInfo->SetItemType(Arr[i].ItemType);
					WeaponInfo->SetPadding(5.f);
					
					WeaponInfoGrid->AddChildToVerticalBox(WeaponInfo);
				}
				if (!InventoryComponent->OnWeaponInfoUpdate.IsBound())
				{
					InventoryComponent->OnWeaponInfoUpdate.AddUFunction(this, FName("UpdatedWeaponInfo"));
				}
			}
		}
	}
}

void UInventoryWeaponInfo::UpdatedWeaponInfo()
{
	WeaponInfoGrid->ClearChildren();
	UpdateWeaponInfo();
}
