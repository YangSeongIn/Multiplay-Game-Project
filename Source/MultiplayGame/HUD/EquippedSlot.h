// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Types/EquippedSlotType.h"
#include "EquippedSlot.generated.h"

UENUM(BlueprintType)
enum class EWeaponNum : uint8
{
	EWN_Weapon1 UMETA(DisplayName = "Weapon1"),
	EWN_Weapon2 UMETA(DisplayName = "Weapon2"),

	EST_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class MULTIPLAYGAME_API UEquippedSlot : public UUserWidget
{
	GENERATED_BODY()
	
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

	UPROPERTY(EditAnywhere, meta = (EditCondition = "EquippedSlotType == EST_Weapon", EditConditionHides))
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
	FORCEINLINE void SetEquippedSlotWidget(class UEquippedSlotWidget* SlotWidget) { EquippedSlotWidget = SlotWidget; };

};
