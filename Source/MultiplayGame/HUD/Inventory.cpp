// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/WrapBox.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryGrid.h"
#include "Components/Image.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	// UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(this, 0), InventoryGrid);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(this, 0), InventoryGrid);
	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(true);
}

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();

	/*AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		InventoryGrid->DisplayInventory(Character->GetInventoryComponent());
	}*/
}

void UInventory::NativeDestruct()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(Controller))
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
		Controller->SetShowMouseCursor(false);
		AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character != nullptr)
		{
			Character->SetInventoryWidgetNull();
		}
	}
	Super::NativeDestruct();
}

FReply UInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		RemoveFromParent();
	}
	return FReply::Handled();
}

void UInventory::SetMeshCaptureImage(UMaterialInterface* Image)
{
	MeshCaptureImage->SetBrushFromMaterial(Image);
}
