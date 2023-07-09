// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite)
		class UInventoryComponent* InventoryComponent;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UWrapBox* ItemGrid;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> InventorySlotWidgetClass;

	UFUNCTION()
		void DisplayInventory(class UInventoryComponent* InventoryComp);
	UFUNCTION()
		void UpdateInventory();
	UFUNCTION()
		void UpdatedInventory();
};
