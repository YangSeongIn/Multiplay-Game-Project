// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AroundItemGrid.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UAroundItemGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

private:
	UPROPERTY()
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UWrapBox* ItemGrid;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> InventorySlotWidgetClass;

public:
	UFUNCTION()
		void DisplayOverlappedItems(class UInventoryComponent* InventoryComp);
	UFUNCTION()
		void UpdateInventory();
	UFUNCTION()
		void UpdatedInventory();
};
