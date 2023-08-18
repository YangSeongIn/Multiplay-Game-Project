// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYGAME_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoAmount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MatchCountdownText;

	UPROPERTY(meta = (BindWidget))
		class UImage* Weapon1Image;

	UPROPERTY(meta = (BindWidget))
		class UImage* Weapon2Image;

	UPROPERTY(meta = (BindWidget))
		class UImage* HighPingImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* HighPingAnimation;

	void ActivateWeapon(bool bIsActive, int32 Num);
	void SetWeaponImage(class UTexture2D* NewTexture, int32 Num);
};
