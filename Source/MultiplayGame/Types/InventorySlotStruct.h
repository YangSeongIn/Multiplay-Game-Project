#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquippedSlotType.h"
#include "InventorySlotStruct.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		int32 Quantity;
	UPROPERTY(BlueprintReadWrite)
		EItemType ItemType;
	UPROPERTY(BlueprintReadWrite)
		FString ItemID;
	UPROPERTY(BlueprintReadWrite)
		FString InherenceName;
	UPROPERTY(BlueprintReadWrite)
		class AItem* Item;
};

USTRUCT(Blueprintable)
struct FEquippedWeaponSlotStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int32 AmmoQuantity;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int32 CarriedAmmoQuantity;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		EItemType ItemType;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		EWeaponNum WeaponNum;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FString ItemID;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class AWeapon* Weapon;
};

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 StackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* DetailThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* ItemClass;
	
};