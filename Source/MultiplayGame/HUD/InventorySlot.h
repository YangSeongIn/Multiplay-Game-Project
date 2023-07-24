// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Types/EquippedSlotType.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlot();

protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

private:
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UImage* IconImage;
	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* QuantityText;
	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UBorder* OutBorder;
	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UBorder* InBorder;

	UPROPERTY(BlueprintReadWrite, Category = "Drag Item Visual", meta = (ExposeOnSpawn = "true"), meta = (AllowPrivateAccess = "true"))
		FString ItemID;
	UPROPERTY(BlueprintReadWrite, Category = "Drag Item Visual", meta = (ExposeOnSpawn = "true"), meta = (AllowPrivateAccess = "true"))
		int Quantity;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Drag Item Visual")
		TSubclassOf<UUserWidget> DragItemVisualClass;
	UPROPERTY()
		class UDragItemVisual* DragItemVisual;
	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDragDropOperation> DragDropSlotClass;
	UPROPERTY()
		class UDragDropSlot* DragDropSlot;

	FString InherenceName;

	UPROPERTY()
		class AItem* Item;

	EEquippedSlotType EquippedSlotType = EEquippedSlotType::EST_Inventory;

public:
	FORCEINLINE void SetIcon(class UImage* TargetImage) { IconImage = TargetImage; };
	void SetQuantity(int32 TargetQuantity);
	FORCEINLINE void SetSlotIndex(int32 TargetIndex) { SlotIndex = TargetIndex; };
	FORCEINLINE void SetItemID(FString TargetID) { ItemID = TargetID; };
	FORCEINLINE void SetInventoryComponent(class UInventoryComponent* TargetComponent) { InventoryComponent = TargetComponent; };
	FORCEINLINE void SetItemType(EItemType NewType) { ItemType = NewType; };
	FORCEINLINE void SetInherenceName(FString NewName) { InherenceName = NewName; };
	FORCEINLINE void SetEquippedSlotType(EEquippedSlotType SlotTypeToEquip) { EquippedSlotType = SlotTypeToEquip; };
	FORCEINLINE FString GetInherenceName() { return InherenceName; };
	FORCEINLINE void SetItem(class AItem* ItemToSet) { Item = ItemToSet; };
	FORCEINLINE EItemType GetItemType() { return ItemType; };
};
