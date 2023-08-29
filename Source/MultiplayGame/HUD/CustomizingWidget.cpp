// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizingWidget.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "CustomizingSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/CustomizingCharacter.h"
#include "Animation/AnimInstance.h"
#include "../GameInstance/MainGameInstance.h"
#include "../Character/MainCharacter.h"
#include "../SaveGameData/SaveGameData.h"
#include "GameFramework/PlayerState.h"
#include "../PlayerController/MainPlayerController.h"
#include "../PlayerState/MainPlayerState.h"

void UCustomizingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Hair->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_Hair);
	Btn_Goggle->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClickedBtn_Goggle);
	Btn_Beard->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_Beard);
	Btn_UpperBody->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_UpperBody);
	Btn_LowerBody->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_LowerBody);

	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetOwningPlayer());
	if (MainPlayerController)
	{
		CustomizingSaveDataStruct = MainPlayerController->GetSaveGameData();
	}
}

void UCustomizingWidget::SetPartMesh(FString SlotID, USkeletalMesh* MeshToSet)
{
	if (Character)
	{
		if (SlotID == "Hair")
		{
			SetMeshVisibility(MeshToSet, Character->HairMesh);
		}
		else if (SlotID == "Goggle")
		{
			SetMeshVisibility(MeshToSet, Character->GoggleMesh);
		}
		else if (SlotID == "Beard")
		{
			SetMeshVisibility(MeshToSet, Character->BeardMesh);
		}
		else if (SlotID == "UpperBody")
		{
			SetMeshVisibility(MeshToSet, Character->UpperBodyMesh);
		}
		else if (SlotID == "LowerBody")
		{
			SetMeshVisibility(MeshToSet, Character->LowerBodyMesh);
		}
	}
}

void UCustomizingWidget::SetMeshVisibility(USkeletalMesh* MeshToSet, USkeletalMeshComponent* TargetMesh)
{
	if (MeshToSet == nullptr)
	{
		TargetMesh->SetVisibility(false);
	}
	else
	{
		TargetMesh->SetVisibility(true);
		TargetMesh->SetSkeletalMesh(MeshToSet);
	}
}

void UCustomizingWidget::SetPartIndex(FString SlotID, int32 Index)
{
	HighlightingSlot(Index);
	if (SlotID == "Hair")
	{
		CustomizingSaveDataStruct.HairIndex = Index;
	}
	else if (SlotID == "Goggle")
	{
		CustomizingSaveDataStruct.GoggleIndex = Index;
	}
	else if (SlotID == "Beard")
	{
		CustomizingSaveDataStruct.BeardIndex = Index;
	}
	else if (SlotID == "UpperBody")
	{
		CustomizingSaveDataStruct.UpperBodyIndex = Index;
	}
	else if (SlotID == "LowerBody")
	{
		CustomizingSaveDataStruct.LowerBodyIndex = Index;
	}
	
	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetOwningPlayer());
	if (MainPlayerController)
	{
		MainPlayerController->SaveData(CustomizingSaveDataStruct);
	}
}

void UCustomizingWidget::HighlightingSlot(int32 TargetIdx)
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (TargetIdx == i)
		{
			Slots[i]->SetSlotClicked(true);
		}
		else
		{
			Slots[i]->SetSlotClicked(false);
		}
	}
}

void UCustomizingWidget::OnClicked_Btn_Hair()
{
	SetCustomizingSlot(Character->Hairs, "Hair", CustomizingSaveDataStruct.HairIndex);
}

void UCustomizingWidget::OnClickedBtn_Goggle()
{
	SetCustomizingSlot(Character->Goggles, "Goggle", CustomizingSaveDataStruct.GoggleIndex);
}

void UCustomizingWidget::OnClicked_Btn_Beard()
{
	SetCustomizingSlot(Character->Beards, "Beard", CustomizingSaveDataStruct.BeardIndex);
}

void UCustomizingWidget::OnClicked_Btn_UpperBody()
{
	SetCustomizingSlot(Character->UpperBodies, "UpperBody", CustomizingSaveDataStruct.UpperBodyIndex);
}

void UCustomizingWidget::OnClicked_Btn_LowerBody()
{
	SetCustomizingSlot(Character->LowerBodies, "LowerBody", CustomizingSaveDataStruct.LowerBodyIndex);
}

void UCustomizingWidget::SetCustomizingSlot(const TArray<FCustomizingInfoStruct>& InfoStructToCustomize, FString SlotID, int32 BodyIndex)
{
	Slots.Empty();
	Grid->ClearChildren();
	if (Character == nullptr) return;
	for (int32 i = 0; i < InfoStructToCustomize.Num(); i++) {
		CustomizingSlot = Cast<UCustomizingSlot>(CreateWidget(this, SlotClass));
		if (CustomizingSlot)
		{
			CustomizingSlot->SetImage(InfoStructToCustomize[i].Thumbnail);
			CustomizingSlot->SetCustomMesh(InfoStructToCustomize[i].Mesh);
			CustomizingSlot->SetSlotID(SlotID);
			CustomizingSlot->SetIndex(i);
			CustomizingSlot->SetCustomizingWidget(this);
			Slots.Add(CustomizingSlot);
			if (i == BodyIndex)
			{
				CustomizingSlot->SetSlotClicked(true);
			}
			Grid->AddChildToWrapBox(CustomizingSlot);
		}
	}
}
