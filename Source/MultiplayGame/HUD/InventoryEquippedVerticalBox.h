// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "InventoryEquippedVerticalBox.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UInventoryEquippedVerticalBox : public UVerticalBox
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UEquippedSlot* EquippedSlot1;
	UPROPERTY(EditAnywhere)
		class UEquippedSlot* EquippedSlot2;

public:
	void SwapSlot();
};
