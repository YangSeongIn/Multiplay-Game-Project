// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Types/EquippedSlotType.h"
#include "WeaponInfo.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UWeaponInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot();

	UFUNCTION()
		void UpdateWeaponInfo();
	
	UFUNCTION(Server, Reliable)
		void ServerEquipWeaponByAroundItem(class UDragDropSlot* SlotForDragDrop);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

private:
	UPROPERTY(meta = (BindWidget))
		class UBorder* OutBorder;

	UPROPERTY(BlueprintReadWrite, Category = "Drag Item Visual", meta = (ExposeOnSpawn = "true"), meta = (AllowPrivateAccess = "true"))
		FString ItemID;

	UPROPERTY()
		class AMainCharacter* Character;

	UPROPERTY(EditAnywhere)
		EEquippedSlotType EquippedSlotType;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), EditAnywhere, meta = (EditCondition = "EquippedSlotType == EEquippedSlotType::EST_Weapon", EditConditionHides))
		EWeaponNum WeaponNum;

	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoText;

	UPROPERTY(EditAnywhere, Category = "Drag Item Visual")
		TSubclassOf<UUserWidget> DragItemVisualClass;
	UPROPERTY()
		class UDragItemVisual* DragItemVisual;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDragDropOperation> DragDropSlotClass;
	UPROPERTY()
		class UDragDropSlot* DragDropSlot;

	UPROPERTY()
		class UInventoryWeaponInfo* InventoryWeaponInfo;

	void SetSlot(class UItemDataComponent* ItemDataComponent);

	UPROPERTY()
		class UInventoryComponent* InventoryComponent;

	int32 AmmoQuantity;
	int32 CarriedAmmoQuantity;

	EItemType ItemType;

	UPROPERTY()
		class UCombatComponent* CombatComponent;

	class AWeapon* Weapon;

public:
	FORCEINLINE class UInventoryWeaponInfo* GetInventoryWeaponInfo() { return InventoryWeaponInfo; };
	FORCEINLINE void SetInventoryWeaponInfo(class UInventoryWeaponInfo* WeaponInfo) { InventoryWeaponInfo = WeaponInfo; };
	FORCEINLINE void SetIconImage(class UImage* NewImage) { IconImage = NewImage; };
	FORCEINLINE class UImage* GetIconImage() { return IconImage; };
	FORCEINLINE void SetItemID(FString NewString) { ItemID = NewString; };
	FORCEINLINE FString GetItemID() { return ItemID; };
	FORCEINLINE EWeaponNum GetWeaponNum() { return WeaponNum; };
	FORCEINLINE void SetWeaponNum(EWeaponNum NumOfWeapon) { WeaponNum = NumOfWeapon; };
	FORCEINLINE void SetInventoryComponent(class UInventoryComponent* InventoryComp) { InventoryComponent = InventoryComp; };
	FORCEINLINE void SetAmmoQuantity(int32 Quantity) { AmmoQuantity = Quantity; };
	FORCEINLINE void SetCarriedAmmoQuantity(int32 Quantity) { CarriedAmmoQuantity = Quantity; };
	FORCEINLINE void SetItemType(EItemType NewType) { ItemType = NewType; };
	FORCEINLINE void SetCombatComponent(class UCombatComponent* CombatComp) { CombatComponent = CombatComp; };
	FORCEINLINE void SetWeapon(class AWeapon* WeaponToSet) { Weapon = WeaponToSet; };
};
