// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWeaponInfo.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UInventoryWeaponInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox*	WeaponInfoGrid;

	UPROPERTY()
		class UInventoryComponent* InventoryComponent;
	UPROPERTY()
		class UCombatComponent* CombatComponent;

	/*UPROPERTY(meta = (BindWidget))
		class UWeaponInfo* WeaponInfo1;

	UPROPERTY(meta = (BindWidget))
		class UWeaponInfo* WeaponInfo2;*/

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WeaponInfoClass;

	void DisplayWeaponInfo(class UInventoryComponent* InventoryComp);

	UFUNCTION()
		void UpdateWeaponInfo();

	UFUNCTION()
		void UpdatedWeaponInfo();
};
