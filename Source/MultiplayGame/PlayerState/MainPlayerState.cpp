// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "../Character/MainCharacter.h"
#include "../PlayerController/MainPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "../SaveGameData/SaveGameData.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainPlayerState, Defeats);
}

void AMainPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	Character = Character == nullptr ? Cast<AMainCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void AMainPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<AMainCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void AMainPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<AMainCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void AMainPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<AMainCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void AMainPlayerState::ApplyCustomizingInfo()
{
	Character = Character == nullptr ? Cast<AMainCharacter>(GetPawn()) : Character;
	if (Character)
	{

	}
}

void AMainPlayerState::UpdateCustomizingInfo()
{
	SaveGameData == nullptr ? Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveDataName, 0)) : SaveGameData;
	if (SaveGameData)
	{
		CustomizingSaveData = SaveGameData->GetCutomizingSaveData();
		ApplyCustomizingInfo();
	}
}

FCustomizingSaveDataStruct AMainPlayerState::GetSaveGameData()
{
	SaveGameData == nullptr ? Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveDataName, 0)) : SaveGameData;
	if (SaveGameData)
	{
		CustomizingSaveData = SaveGameData->GetCutomizingSaveData();
		return CustomizingSaveData;
	};
	return FCustomizingSaveDataStruct{ 0, 0, 0, 0, 0 };
}

void AMainPlayerState::SaveData(FCustomizingSaveDataStruct DataToSave)
{
	SaveGameData == nullptr ? Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveDataName, 0)) : SaveGameData;
	if (SaveGameData)
	{
		SaveGameData->SetCutomizingSaveData(DataToSave);

		UGameplayStatics::SaveGameToSlot(SaveGameData, SaveDataName, 0);
	}
}

void AMainPlayerState::SetMeshWithCustomizingInfo()
{
	FCustomizingSaveDataStruct SaveData = GetSaveGameData();
	Character = Character == nullptr ? Cast<AMainCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Character->MulticastApplyCustomizingInfo(SaveData);
	}
}
