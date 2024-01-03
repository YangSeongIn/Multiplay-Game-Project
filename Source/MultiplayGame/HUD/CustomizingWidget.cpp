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
#include "../Data/CostomizingDataAsset.h"
#include "../GameInstance/MainGameInstance.h"

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

void UCustomizingWidget::SetMeshVisibility(USkeletalMesh* MeshToSet, USkeletalMeshComponent* TargetMesh)
{
	TargetMesh->SetVisibility(MeshToSet ? true : false);
	if (MeshToSet)
	{
		TargetMesh->SetSkeletalMesh(MeshToSet);
	}
}


void UCustomizingWidget::SetPartMesh(ECustomizingCategory Category, USkeletalMesh* MeshToSet)
{
	if (Character)
	{
		switch (Category)
		{
		case ECustomizingCategory::ECC_Hair:
			SetMeshVisibility(MeshToSet, Character->HairMesh);
			break;
		case ECustomizingCategory::ECC_Goggle:
			SetMeshVisibility(MeshToSet, Character->GoggleMesh);
			break;
		case ECustomizingCategory::ECC_Beard:
			SetMeshVisibility(MeshToSet, Character->BeardMesh);
			break;
		case ECustomizingCategory::ECC_UpperBody:
			SetMeshVisibility(MeshToSet, Character->UpperBodyMesh);
			break;
		case ECustomizingCategory::ECC_LowerBody:
			SetMeshVisibility(MeshToSet, Character->LowerBodyMesh);
			break;
		default:
			break;
		}
	}
}

void UCustomizingWidget::SetPartIndex(ECustomizingCategory Category, int32 Index)
{
	HighlightingSlot(Index);

	switch (Category)
	{
	case ECustomizingCategory::ECC_Hair:
		CustomizingSaveDataStruct.HairIndex = Index;
		break;
	case ECustomizingCategory::ECC_Goggle:
		CustomizingSaveDataStruct.GoggleIndex = Index;
		break;
	case ECustomizingCategory::ECC_Beard:
		CustomizingSaveDataStruct.BeardIndex = Index;
		break;
	case ECustomizingCategory::ECC_UpperBody:
		CustomizingSaveDataStruct.UpperBodyIndex = Index;
		break;
	case ECustomizingCategory::ECC_LowerBody:
		CustomizingSaveDataStruct.LowerBodyIndex = Index;
		break;
	default:
		break;
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
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	SetCustomizingSlot(GameInstance->GetCustomizingDataAsset()->HairMeshes, ECustomizingCategory::ECC_Hair, CustomizingSaveDataStruct.HairIndex);
}

void UCustomizingWidget::OnClickedBtn_Goggle()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	SetCustomizingSlot(GameInstance->GetCustomizingDataAsset()->GoggleMeshes, ECustomizingCategory::ECC_Goggle, CustomizingSaveDataStruct.GoggleIndex);
}

void UCustomizingWidget::OnClicked_Btn_Beard()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	SetCustomizingSlot(GameInstance->GetCustomizingDataAsset()->BeardsMeshes, ECustomizingCategory::ECC_Beard, CustomizingSaveDataStruct.BeardIndex);
}

void UCustomizingWidget::OnClicked_Btn_UpperBody()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	SetCustomizingSlot(GameInstance->GetCustomizingDataAsset()->UpperBodyMeshes, ECustomizingCategory::ECC_UpperBody, CustomizingSaveDataStruct.UpperBodyIndex);
}

void UCustomizingWidget::OnClicked_Btn_LowerBody()
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	SetCustomizingSlot(GameInstance->GetCustomizingDataAsset()->LowerBodyMeshes, ECustomizingCategory::ECC_LowerBody, CustomizingSaveDataStruct.LowerBodyIndex);
}

void UCustomizingWidget::SetCustomizingSlot(const TArray<FCustomizingInfoStruct>& InfoStructToCustomize, ECustomizingCategory Category, int32 BodyIndex)
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
			CustomizingSlot->SetCategory(Category);
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
