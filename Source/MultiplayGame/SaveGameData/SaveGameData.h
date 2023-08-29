// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "SaveGameData.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API USaveGameData : public USaveGame
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	FCustomizingSaveDataStruct CustomizingSaveDataStruct;

public:
	FORCEINLINE FCustomizingSaveDataStruct GetCutomizingSaveData() { return CustomizingSaveDataStruct; };
	FORCEINLINE void SetCutomizingSaveData(FCustomizingSaveDataStruct SaveData) { CustomizingSaveDataStruct = SaveData; };
};
