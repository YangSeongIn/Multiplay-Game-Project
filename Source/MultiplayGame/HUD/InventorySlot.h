// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	int32 ContentIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget))
		class UImage* IconImage;
	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget))
		class UTextBlock* QuantityText;

	UPROPERTY(BlueprintReadWrite, Category = "Drag Item Visual", meta = (ExposeOnSpawn = "true"))
		FString ItemID;
	UPROPERTY(BlueprintReadWrite, Category = "Drag Item Visual", meta = (ExposeOnSpawn = "true"))
		int Quantity;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		class UInventoryComponent* InventoryComponent;

	/*UPROPERTY(EditAnywhere, Category = "Drag Item Visual")
		TSubclassOf<UUserWidget> DragItemVisualClass;
	UPROPERTY()
		class UDragItemVisual* DragItemVisual;*/
	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;

	/*UPROPERTY(EditAnywhere)
		TSubclassOf<UDragDropOperation> DragDropOperationClass;
	UPROPERTY()
		class UItemDragDropOperation* DragDropOperation;*/

protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetSlot();

};
