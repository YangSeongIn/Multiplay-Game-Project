// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Types/EquippedSlotType.h"
#include "EquippedSlot.generated.h"

UCLASS()
class MULTIPLAYGAME_API UEquippedSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateSlot();

protected:
		virtual void NativePreConstruct() override;
		virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
		virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
		virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
		virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
		virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UBorder* InBorder;

	UPROPERTY(BlueprintReadWrite, Category = "Drag Item Visual", meta = (ExposeOnSpawn = "true"), meta = (AllowPrivateAccess = "true"))
		FString ItemID;

	UPROPERTY()
		class AMainCharacter* Character;

	UPROPERTY(EditAnywhere)
		EEquippedSlotType EquippedSlotType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "EquippedSlotType == EEquippedSlotType::EST_Weapon", EditConditionHides))
		EWeaponNum WeaponNum;

	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UImage* IconImage;

	UPROPERTY(EditAnywhere, Category = "Drag Item Visual")
		TSubclassOf<UUserWidget> DragItemVisualClass;
	UPROPERTY()
		class UDragItemVisual* DragItemVisual;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDragDropOperation> DragDropSlotClass;
	UPROPERTY()
		class UDragDropSlot* DragDropSlot;

	UPROPERTY()
		class UEquippedSlotWidget* EquippedSlotWidget;

	/*UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), meta = (AllowPrivateAccess = "true"))
		FString ItemID;*/

	void SetSlot(class UItemDataComponent* ItemDataComponent);

public:
	FORCEINLINE class UEquippedSlotWidget* GetEquippedSlotWidget() { return EquippedSlotWidget; };
	FORCEINLINE void SetEquippedSlotWidget(class UEquippedSlotWidget* SlotWidget) { EquippedSlotWidget = SlotWidget; };
	FORCEINLINE void SetIconImage(class UImage* NewImage) { IconImage = NewImage; };
	FORCEINLINE class UImage* GetIconImage() { return IconImage; };
	FORCEINLINE void SetItemID(FString NewString) { ItemID = NewString; };
	FORCEINLINE FString GetItemID() { return ItemID; };
	FORCEINLINE EWeaponNum GetWeaponNum() { return WeaponNum; };

};
