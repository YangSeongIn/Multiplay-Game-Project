#pragma once

UENUM(BlueprintType)
enum class EEquippedSlotType : uint8
{
	EST_Head UMETA(DisplayName = "Head"),
	EST_Body UMETA(DisplayName = "Body"),
	EST_Weapon UMETA(DisplayName = "Weapon"),
	EST_Inventory UMETA(DisplayName = "Inventory"),
	EST_AroundItem UMETA(DisplayName = "AroundItem"),

	EST_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Head UMETA(DisplayName = "Head"),
	EIT_Body UMETA(DisplayName = "Body"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Ammo UMETA(DisplayName = "Ammo"),

	EIT_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EDragDropInventorySlotType : uint8
{
	EDD_InventorySlot UMETA(DisplayName = "Head"),
	EDD_EquippedSlot UMETA(DisplayName = "Body"),

	EST_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EWeaponNum : uint8
{
	EWN_Weapon1 UMETA(DisplayName = "Weapon1"),
	EWN_Weapon2 UMETA(DisplayName = "Weapon2"),

	EST_MAX UMETA(DisplayName = "DefaultMax")
};