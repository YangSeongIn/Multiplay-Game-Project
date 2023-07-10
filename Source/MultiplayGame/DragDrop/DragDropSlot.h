// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDropSlot.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UDragDropSlot : public UDragDropOperation
{
	GENERATED_BODY()

private:
	int32 ContentIndex;
	UPROPERTY()
		class UInventoryComponent* InventoryComponent;

public:
	FORCEINLINE UInventoryComponent* GetInventoryComponent() { return InventoryComponent; };
	FORCEINLINE void SetInventoryComponent(class UInventoryComponent* Inventory) { InventoryComponent = Inventory; };
	FORCEINLINE int32 GetContentIndex() { return ContentIndex; };
	FORCEINLINE void SetContentIndex(int32 Index) { ContentIndex = Index; };
};
