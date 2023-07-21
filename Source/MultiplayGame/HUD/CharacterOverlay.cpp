// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterOverlay.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"



void UCharacterOverlay::ActivateWeapon(bool bIsActive, int32 Num)
{
	UImage*& TargetImage = Num == 0 ? Weapon1Image : Weapon2Image;
	if (TargetImage)
	{
		if (bIsActive)
		{
			TargetImage->SetVisibility(ESlateVisibility::Visible);
			TargetImage->SetOpacity(1.f);
			TargetImage->SetRenderScale(FVector2D(1.25f));
		}
		else
		{
			TargetImage->SetVisibility(ESlateVisibility::Visible);
			TargetImage->SetOpacity(0.5f);
			TargetImage->SetRenderScale(FVector2D(0.75f));
		}
	}
	else
	{
		TargetImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCharacterOverlay::SetWeaponImage(UTexture2D* NewTexture, int32 Num)
{
	UImage*& TargetImage = Num == 0 ? Weapon1Image : Weapon2Image;
	if (TargetImage)
	{
		TargetImage->SetBrushFromTexture(NewTexture);
	}
}
