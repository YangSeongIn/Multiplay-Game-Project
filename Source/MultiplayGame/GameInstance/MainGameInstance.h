// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../SaveGameData/SaveGameData.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "../Customizing/CustomizingInfoStruct.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetSaveGameData(FCustomizingSaveDataStruct CustomizingSaveDataStruct);
	FCustomizingSaveDataStruct GetSaveGameData();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

private:
	UPROPERTY()
	USaveGameData* SaveGameData;

	FString SaveData = "SaveData";

	UPROPERTY(EditAnywhere)
	class UCostomizingDataAsset* CustomizingDataAsset;

public:
	FORCEINLINE class UCostomizingDataAsset* GetCustomizingDataAsset() { return CustomizingDataAsset; };

};
