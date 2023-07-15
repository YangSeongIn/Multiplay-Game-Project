// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquippedSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UEquippedSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UVerticalBox* VerticalBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UEquippedSlot* EquippedSlot_Head;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UEquippedSlot* EquippedSlot_Body;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UEquippedSlot* EquippedSlot_Weapon1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UEquippedSlot* EquippedSlot_Weapon2;


public:
	void SwapEquippedWeaponSlot();

};
