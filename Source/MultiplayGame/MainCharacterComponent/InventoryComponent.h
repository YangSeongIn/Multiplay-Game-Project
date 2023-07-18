// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);
DECLARE_MULTICAST_DELEGATE(FOnWeaponInfoUpdate);

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

	TTuple<bool, int> AddToInventory(FString ItemID, int Quantity, EItemType ItemType);
	void RemoveFromInventory(FString ItemID, int Quantity);
	TTuple<int, bool> FindSlot(FString ItemID);
	int32 GetMaxStackSize(FString ItemID);
	void AddToStack(int Idx, int Quantity);
	TTuple<bool, int> AnyEmptySlotAvailable();
	bool CreateNewStack(FString ItemID, int Quantity);

	UFUNCTION(BlueprintCallable)
		void DEBUGPrintContents();
	void TransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex);
	void TransferWeaponInfos(UInventoryComponent* SourceInventory);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastUpdate();

	UFUNCTION(Server, Reliable)
		void ServerTransferSlots(int SourceIndex, UInventoryComponent* SourceInventory, int TargetIndex);

	void AddToWeaponSlot(class AWeapon* Weapon, FString ItemID, int32 AmmoQuantity, int32 CarriedAmmoQuantity, EItemType ItemType);

	UFUNCTION(Server, Reliable)
		void ServerSwapTwoWeapons();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;

	UPROPERTY(EditAnywhere)
		int32 InventorySize = 16;

	TArray<FInventorySlotStruct> Slots;

	TArray<FEquippedWeaponSlotStruct> WeaponInfos;

	UPROPERTY()
		AMainCharacter* Character;
public:
	FORCEINLINE int32 GetInventorySize() const { return InventorySize; };
	FORCEINLINE void SetInventorySize(int32 SizeOfInventory) { InventorySize = SizeOfInventory; };
	UFUNCTION(BlueprintCallable)
		TArray<FInventorySlotStruct> GetContents() { return Slots; };
	TArray<FEquippedWeaponSlotStruct> GetWeaponInfos() { return WeaponInfos; };
};
