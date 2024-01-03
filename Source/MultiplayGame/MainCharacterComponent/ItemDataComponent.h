// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "../Weapon/WeaponTypes.h"
#include "ItemDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYGAME_API UItemDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemDataComponent();
	friend class AItem;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact(class AMainCharacter* MainCharacter);
	void SetPickupActiveFalse();
	void AddAmmoToInventory(class AMainCharacter* MainCharacter);

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
		int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FDataTableRowHandle ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		EItemType ItemType;

	UPROPERTY()
		AActor* OwningActor;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType == EItemType::EIT_Ammo", EditConditionHides))
		EWeaponType WeaponType;

public:
	FORCEINLINE FDataTableRowHandle GetItemID() { return ItemID; };
	FORCEINLINE int32 GetQuantity() { return Quantity; };
	FORCEINLINE EItemType GetItemType() { return ItemType; };
	EWeaponType GetWeaponType();
	FORCEINLINE void SetQuantity(int32 QuantityToSet) { Quantity = QuantityToSet; };
};
