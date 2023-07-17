// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWeaponInfo.h"
#include "Components/VerticalBox.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../HUD/InventorySlot.h"
#include "../HUD/WeaponInfo.h"
#include "../MainCharacterComponent/InventoryComponent.h"

void UInventoryWeaponInfo::NativePreConstruct()
{
	Super::NativePreConstruct();

	//UpdateWeaponInfo();
}

void UInventoryWeaponInfo::DisplayWeaponInfo(UInventoryComponent* InventoryComp)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("3")));
	this->InventoryComponent = InventoryComp;
	WeaponInfoGrid->ClearChildren();
	UpdateWeaponInfo();
}

void UInventoryWeaponInfo::UpdateWeaponInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("4")));
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		InventoryComponent = Character->GetInventoryComponent();
		if (InventoryComponent)
		{
			if (WeaponInfoClass)
			{
				TArray<FEquippedWeaponSlotStruct> Arr = InventoryComponent->GetWeaponInfos();
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("5")));
				for (int i = 0; i < Arr.Num(); i++)
				{
					UWeaponInfo* WeaponInfo = Cast<UWeaponInfo>(CreateWidget(this, WeaponInfoClass));
					WeaponInfo->SetItemID(Arr[i].ItemID);
					WeaponInfo->SetInventoryComponent(InventoryComponent);
					WeaponInfo->SetAmmoQuantity(Arr[i].AmmoQuantity);
					WeaponInfo->SetCarriedAmmoQuantity(Arr[i].CarriedAmmoQuantity);
					WeaponInfoGrid->AddChildToVerticalBox(WeaponInfo);
					GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,FString::Printf(TEXT("ItemID : %s"), *Arr[i].ItemID));
				}
				InventoryComponent->OnWeaponInfoUpdate.AddUFunction(this, FName("UpdatedWeaponInfo"));
			}
		}
	}
}

void UInventoryWeaponInfo::UpdatedWeaponInfo()
{
	WeaponInfoGrid->ClearChildren();
	UpdateWeaponInfo();
}
