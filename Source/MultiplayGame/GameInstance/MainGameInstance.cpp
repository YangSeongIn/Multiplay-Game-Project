// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../CharacterMeshCapture/CharacterMeshCapture.h"
#include "Net/UnrealNetwork.h"

void UMainGameInstance::SetSaveGameData(FCustomizingSaveDataStruct CustomizingSaveDataStruct)
{
	if (SaveGameData)
	{
		SaveGameData->CustomizingSaveDataStruct = CustomizingSaveDataStruct;
	}
	UGameplayStatics::SaveGameToSlot(SaveGameData, SaveData, 0);
}

FCustomizingSaveDataStruct UMainGameInstance::GetSaveGameData()
{
	if (SaveGameData)
	{
		return SaveGameData->CustomizingSaveDataStruct;
	}
	return FCustomizingSaveDataStruct{ 0, 0, 0, 0, 0 };
}

void UMainGameInstance::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveData, 0))
	{
		SaveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveData, 0));
	}
	else
	{
		SaveGameData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	}
}
