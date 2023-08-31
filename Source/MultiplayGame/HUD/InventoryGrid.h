// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Types/EquippedSlotType.h"
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
	virtual void NativeDestruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

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

	UFUNCTION(Server, Reliable)
	void ServerBinding();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBinding();
};
