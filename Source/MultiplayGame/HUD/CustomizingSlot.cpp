// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizingSlot.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "CustomizingWidget.h"
#include "Engine/SkeletalMesh.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UCustomizingSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UCustomizingSlot::OnClickedButton);
}

void UCustomizingSlot::OnClickedButton()
{
	CustomizingWidget->SetPartMesh(SlotID, Mesh);
	CustomizingWidget->SetPartIndex(SlotID, Index);
}

void UCustomizingSlot::SetImage(UTexture2D* ImageToSet)
{
	if (ImageToSet)
	{
		Thumbnail->SetVisibility(ESlateVisibility::Visible);
		Thumbnail->SetBrushFromTexture(ImageToSet);
		NoneText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		NoneText->SetVisibility(ESlateVisibility::Visible);
		Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCustomizingSlot::SetSlotClicked(bool bIsClicked)
{
	if (OutBorder && CustomizingWidget)
	{
		if (bIsClicked)
		{
			OutBorder->SetBrushColor(FLinearColor(1.f, 0.3f, 0.f, 1.0f));
		}
		else
		{
			OutBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
		}
	}
}
