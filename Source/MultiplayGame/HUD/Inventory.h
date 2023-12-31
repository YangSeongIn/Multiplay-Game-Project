// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(meta = (BindWidget))
		class UInventoryGrid* InventoryGrid;

	UPROPERTY(meta = (BindWidget))
		class UImage* MeshCaptureImage;

	UPROPERTY(meta = (BindWidget))
		class UInventoryWeaponInfo* InventoryWeaponInfo;

	UPROPERTY(meta = (BindWidget))
		class UAroundItemGrid* AroundItemGrid;

public:
	void SetMeshCaptureImage(class UMaterialInterface* Image);

};
