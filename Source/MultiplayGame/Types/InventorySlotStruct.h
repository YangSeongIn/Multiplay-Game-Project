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
		FString ItemID;
	UPROPERTY(BlueprintReadWrite)
		int32 Quantity;
	UPROPERTY(BlueprintReadWrite)
		EItemType ItemType;
};

USTRUCT(BlueprintType)
struct FEquippedWeaponSlotStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString ItemID;
	UPROPERTY(BlueprintReadWrite)
		int32 AmmoQuantity;
	UPROPERTY(BlueprintReadWrite)
		int32 CarriedAmmoQuantity;
	UPROPERTY(BlueprintReadWrite)
		EItemType ItemType;
};

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 StackSize;
};