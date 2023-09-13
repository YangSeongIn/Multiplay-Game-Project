// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "../Weapon/WeaponTypes.h"
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

	void AddToInventory(class AItem* Item, int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName, EWeaponType WeaponType);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateInventory();
	TTuple<int, bool> FindSlot(FString ItemID);
	int32 GetMaxStackSize(FString ItemID);
	void AddToStack(int Idx, int Quantity);
	TTuple<bool, int> AnyEmptySlotAvailable();
	bool CreateNewStack(int32 Quantity, EItemType ItemType, FString ItemID, FString InherenceName, class AItem* Item);

	UFUNCTION(BlueprintCallable)
	void DEBUGPrintContents();
	UFUNCTION(BlueprintCallable)
	void DEBUGPrintOverlappedItems();
	void TransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex);
	void TransferWeaponInfos(/*UInventoryComponent* SourceInventory*/);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdate();

	/*UFUNCTION(Server, Reliable)
	void ServerTransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex);*/

	void AddToWeaponSlot(class AWeapon* Weapon, FString ItemID, int32 AmmoQuantity, int32 CarriedAmmoQuantity, EItemType ItemType);

	UFUNCTION(Server, Reliable)
	void ServerSwapTwoWeapons();

	//void AddOverlappedItem(FString ItemID, int32 Quantity, EItemType ItemType, FString InherenceName, class AItem* Item);
	//void RemoveOverlappedItem(FString InherenceName);

	void UpdateWeaponInfoSlot(class AWeapon* WeaponToUpdate);
	void UpdateAmmoSlot();

	void AddToSelectedWeaponSlot(class AItem* Item, EWeaponNum WeaponNum);

	void DropWeaponByDragging(EWeaponNum WeaponNum);

	void DropInventoryItemByDragging(int32 ContentIndex);

	void DropPickup(class AItem* ItemToSpawn, const int32& ContentIndex);

	void DropAmmo(class APickup* Pickup, const int32& ContentIndex);

	int32 GetAmmoIndexFromInventory();

	UFUNCTION(Client, Reliable)
	void ServerRemoveItemFromSlot(const int32& ContentIndex);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBroadCast();

	void RemoveAllContents();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere)
	class UDataTable* InventoryDataTable;

	UPROPERTY(EditAnywhere)
	int32 InventorySize = 16;

	UPROPERTY(Replicated)
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
	FORCEINLINE void SetWeaponInfoID(int idx, FString IDToSet) { WeaponInfos[idx].ItemID = IDToSet; };
	FORCEINLINE TArray<FInventorySlotStruct> GetOverlappedItems() { return OverlappedItems; };
	FORCEINLINE void SetCombatComponent(class UCombatComponent* CombatComp) { CombatComponent = CombatComp; }
	class UTexture2D* GetWeaponImage(class AWeapon* WeaponToFind, bool bDetailImage);
};
