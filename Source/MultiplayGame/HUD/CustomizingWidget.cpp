// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizingWidget.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "CustomizingSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/CustomizingCharacter.h"
#include "Animation/AnimInstance.h"
#include "../GameInstance/MainGameInstance.h"

void UCustomizingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Hair->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_Hair);
	Btn_Goggle->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClickedBtn_Goggle);
	Btn_Beard->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_Beard);
	Btn_UpperBody->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_UpperBody);
	Btn_LowerBody->OnClicked.AddDynamic(this, &UCustomizingWidget::OnClicked_Btn_LowerBody);

	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		CustomizingSaveDataStruct = GameInstance->GetSaveGameData();
	}
}

void UCustomizingWidget::SetPartMesh(FString SlotID, USkeletalMesh* MeshToSet)
{
	if (MainMesh)
	{
		
		if (SlotID == "Hair")
		{
			SetMeshVisibility(MeshToSet, MainMesh->HairMesh);
		}
		else if (SlotID == "Goggle")
		{
			SetMeshVisibility(MeshToSet, MainMesh->GoggleMesh);
		}
		else if (SlotID == "Beard")
		{
			SetMeshVisibility(MeshToSet, MainMesh->BeardMesh);
		}
		else if (SlotID == "UpperBody")
		{
			SetMeshVisibility(MeshToSet, MainMesh->UpperBodyMesh);
		}
		else if (SlotID == "LowerBody")
		{
			SetMeshVisibility(MeshToSet, MainMesh->LowerBodyMesh);
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
		//CustomizingSa CurrentHair = Index;
		CustomizingSaveDataStruct.HairIndex = Index;
	}
	else if (SlotID == "Goggle")
	{
		//CurrentGoggle = Index;
		CustomizingSaveDataStruct.GoggleIndex = Index;
	}
	else if (SlotID == "Beard")
	{
		//CurrentBeard = Index;
		CustomizingSaveDataStruct.BeardIndex = Index;
	}
	else if (SlotID == "UpperBody")
	{
		//CurrentUpperBody = Index;
		CustomizingSaveDataStruct.UpperBodyIndex = Index;
	}
	else if (SlotID == "LowerBody")
	{
		//CurrentLowerBody = Index;
		CustomizingSaveDataStruct.LowerBodyIndex = Index;
	}

	if (GameInstance)
	{
		GameInstance->SetSaveGameData(CustomizingSaveDataStruct);
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
	Slots.Empty();
	Grid->ClearChildren();
	for (int32 i = 0; i < GameInstance->Hairs.Num(); i++) {
		CustomizingSlot = Cast<UCustomizingSlot>(CreateWidget(this, SlotClass));
		if (CustomizingSlot)
		{
			CustomizingSlot->SetImage(GameInstance->Hairs[i].Thumbnail);
			CustomizingSlot->SetCustomMesh(GameInstance->Hairs[i].Mesh);
			CustomizingSlot->SetSlotID("Hair");
			CustomizingSlot->SetIndex(i);
			CustomizingSlot->SetCustomizingWidget(this);
			Slots.Add(CustomizingSlot);
			if (i == CustomizingSaveDataStruct.HairIndex)
			{
				CustomizingSlot->SetSlotClicked(true);
			}
			Grid->AddChildToWrapBox(CustomizingSlot);
		}
	}
}

void UCustomizingWidget::OnClickedBtn_Goggle()
{
	Slots.Empty();
	Grid->ClearChildren();
	for (int32 i = 0; i < GameInstance->Goggles.Num(); i++) {
		CustomizingSlot = Cast<UCustomizingSlot>(CreateWidget(this, SlotClass));
		if (CustomizingSlot)
		{
			CustomizingSlot->SetImage(GameInstance->Goggles[i].Thumbnail);
			CustomizingSlot->SetCustomMesh(GameInstance->Goggles[i].Mesh);
			CustomizingSlot->SetSlotID("Goggle");
			CustomizingSlot->SetIndex(i);
			CustomizingSlot->SetCustomizingWidget(this);
			Slots.Add(CustomizingSlot);
			if (i == CustomizingSaveDataStruct.GoggleIndex)
			{
				CustomizingSlot->SetSlotClicked(true);
			}
			Grid->AddChildToWrapBox(CustomizingSlot);
		}
	}
}

void UCustomizingWidget::OnClicked_Btn_Beard()
{
	Slots.Empty();
	Grid->ClearChildren();
	for (int32 i = 0; i < GameInstance->Beards.Num(); i++) {
		CustomizingSlot = Cast<UCustomizingSlot>(CreateWidget(this,SlotClass));
		if (CustomizingSlot)
		{
			CustomizingSlot->SetImage(GameInstance->Beards[i].Thumbnail);
			CustomizingSlot->SetCustomMesh(GameInstance->Beards[i].Mesh);
			CustomizingSlot->SetSlotID("Beard");
			CustomizingSlot->SetIndex(i);
			CustomizingSlot->SetCustomizingWidget(this);
			Slots.Add(CustomizingSlot);
			if (i == CustomizingSaveDataStruct.BeardIndex)
			{
				CustomizingSlot->SetSlotClicked(true);
			}
			Grid->AddChildToWrapBox(CustomizingSlot);
		}
	}
}

void UCustomizingWidget::OnClicked_Btn_UpperBody()
{
	Slots.Empty();
	Grid->ClearChildren();
	for (int32 i = 0; i < GameInstance->UpperBodies.Num(); i++) {
		CustomizingSlot = Cast<UCustomizingSlot>(CreateWidget(this, SlotClass));
		if (CustomizingSlot)
		{
			CustomizingSlot->SetImage(GameInstance->UpperBodies[i].Thumbnail);
			CustomizingSlot->SetCustomMesh(GameInstance->UpperBodies[i].Mesh);
			CustomizingSlot->SetSlotID("UpperBody");
			CustomizingSlot->SetIndex(i);
			CustomizingSlot->SetCustomizingWidget(this);
			Slots.Add(CustomizingSlot);
			if (i == CustomizingSaveDataStruct.UpperBodyIndex)
			{
				CustomizingSlot->SetSlotClicked(true);
			}
			Grid->AddChildToWrapBox(CustomizingSlot);
		}
	}
}

void UCustomizingWidget::OnClicked_Btn_LowerBody()
{
	Slots.Empty();
	Grid->ClearChildren();
	for (int32 i = 0; i < GameInstance->LowerBodies.Num(); i++) {
		CustomizingSlot = Cast<UCustomizingSlot>(CreateWidget(this, SlotClass));
		if (CustomizingSlot)
		{
			CustomizingSlot->SetImage(GameInstance->LowerBodies[i].Thumbnail);
			CustomizingSlot->SetCustomMesh(GameInstance->LowerBodies[i].Mesh);
			CustomizingSlot->SetSlotID("LowerBody");
			CustomizingSlot->SetIndex(i);
			CustomizingSlot->SetCustomizingWidget(this);
			Slots.Add(CustomizingSlot);
			if (i == CustomizingSaveDataStruct.LowerBodyIndex)
			{
				CustomizingSlot->SetSlotClicked(true);
			}
			Grid->AddChildToWrapBox(CustomizingSlot);
		}
	}
}
