// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "MainPlayerState.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYGAME_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;

	UFUNCTION()
	void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

	void ApplyCustomizingInfo();
	void UpdateCustomizingInfo();
	FCustomizingSaveDataStruct GetSaveGameData();
	void SaveData(FCustomizingSaveDataStruct DataToSave);

	UPROPERTY()
	class USaveGameData* SaveGameData;
	FString SaveDataName = "SaveData";

	void SetMeshWithCustomizingInfo();

private:
	UPROPERTY()
	class AMainCharacter* Character;
	UPROPERTY()
	class AMainPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	UPROPERTY()
	FCustomizingSaveDataStruct CustomizingSaveData;

};
