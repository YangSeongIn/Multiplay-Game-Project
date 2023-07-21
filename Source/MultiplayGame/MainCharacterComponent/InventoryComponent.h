// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);
DECLARE_MULTICAST_DELEGATE(FOnWeaponInfoUpdate);
DECLARE_MULTICAST_DELEGATE(FOnOverlappedItemUpdate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	friend class AMainCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnInventoryUpdate OnInventoryUpdate;
	FOnWeaponInfoUpdate OnWeaponInfoUpdate;
	FOnOverlappedItemUpdate OnOverlappedItemUpdate;

	TTuple<bool, int> AddToInventory(int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName);
	void RemoveFromInventory(FString ItemID, int Quantity);
	TTuple<int, bool> FindSlot(FString ItemID);
	int32 GetMaxStackSize(FString ItemID);
	void AddToStack(int Idx, int Quantity);
	TTuple<bool, int> AnyEmptySlotAvailable();
	bool CreateNewStack(int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName);

	UFUNCTION(BlueprintCallable)
		void DEBUGPrintContents();
	void TransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex);
	void TransferWeaponInfos(UInventoryComponent* SourceInventory);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastUpdate();

	UFUNCTION(Server, Reliable)
		void ServerTransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex);

	void AddToWeaponSlot(FString ItemID, int32 AmmoQuantity, int32 CarriedAmmoQuantity, EItemType ItemType);

	UFUNCTION(Server, Reliable)
		void ServerSwapTwoWeapons();

	void AddOverlappedItem(FString ItemID, int32 Quantity, EItemType ItemType, FString InherenceName, class AItem* Item);
	void RemoveOverlappedItem(FString InherenceName);

	void UpdateWeaponInfoSlot(class UCombatComponent* InventoryComp, class AWeapon* EquippedWeapon, int32 Ammo, int32 CarriedAmmo);

	void AddToSelectedWeaponSlot(class AItem* Item, EWeaponNum WeaponNum);

	void DropWeaponByDragging(EWeaponNum WeaponNum);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;

	UPROPERTY(EditAnywhere)
		int32 InventorySize = 16;

	TArray<FInventorySlotStruct> Slots;

	UPROPERTY(VisibleAnywhere)
	TArray<FEquippedWeaponSlotStruct> WeaponInfos;

	UPROPERTY()
		AMainCharacter* Character;

	TArray<FInventorySlotStruct> OverlappedItems;

	UPROPERTY()
		class UCombatComponent* CombatComponent;

public:
	FORCEINLINE int32 GetInventorySize() const { return InventorySize; };
	FORCEINLINE void SetInventorySize(int32 SizeOfInventory) { InventorySize = SizeOfInventory; };
	UFUNCTION(BlueprintCallable)
		TArray<FInventorySlotStruct> GetContents() { return Slots; };
	FORCEINLINE TArray<FEquippedWeaponSlotStruct> GetWeaponInfos() { return WeaponInfos; };
	FORCEINLINE TArray<FInventorySlotStruct> GetOverlappedItems() { return OverlappedItems; };
	FORCEINLINE void SetCombatComponent(class UCombatComponent* CombatComp) { CombatComponent = CombatComp; }
	class UTexture2D* GetWeaponImage(class AWeapon* WeaponToFind);
};
