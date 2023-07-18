// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "../Types/EquippedSlotType.h"
#include "DragDropSlot.generated.h"

UCLASS()
class MULTIPLAYGAME_API UDragDropSlot : public UDragDropOperation
{
	GENERATED_BODY()

private:
	int32 ContentIndex;
	FString ItemID;
	UPROPERTY()
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		EEquippedSlotType EquippedSlotType;

	EItemType ItemType;


	UPROPERTY()
		class UWeaponInfo* WeaponInfoSlot;

public:
	FORCEINLINE UInventoryComponent* GetInventoryComponent() { return InventoryComponent; };
	FORCEINLINE void SetInventoryComponent(class UInventoryComponent* Inventory) { InventoryComponent = Inventory; };
	FORCEINLINE int32 GetContentIndex() { return ContentIndex; };
	FORCEINLINE void SetContentIndex(int32 Index) { ContentIndex = Index; };
	FORCEINLINE EEquippedSlotType GetEquippedSlotType() { return EquippedSlotType; };
	FORCEINLINE void SetEquippedSlotType(EEquippedSlotType Type) { EquippedSlotType = Type; };
	FORCEINLINE FString GetItemID() { return ItemID; };
	FORCEINLINE void SetItemID(FString NewID) { ItemID = NewID; };
	FORCEINLINE class UWeaponInfo* GetEquippedSlot() { return WeaponInfoSlot; };
	FORCEINLINE void SetWeaponInfoSlot(class UWeaponInfo* NewSlot) { WeaponInfoSlot = NewSlot; };
	FORCEINLINE void SetItemType(EItemType NewType) { ItemType = NewType; };
	FORCEINLINE EItemType GetItemType() { return ItemType; };
};
