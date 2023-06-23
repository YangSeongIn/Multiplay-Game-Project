// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../HUD/PlayerHUD.h"
#include "../HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Character/MainCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../GameMode/MainGameMode.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<APlayerHUD>(GetHUD());
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(InPawn);
	if (MainCharacter)
	{
		SetHUDHealth(MainCharacter->GetHealth(), MainCharacter->GetMaxHealth());
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
}

void AMainPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainPlayerController, MatchState);
}

void AMainPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncFrequency < TimeSyncRunningTime)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AMainPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountdown(MatchTime - GetServerTime());
	}
	CountdownInt = SecondsLeft;
}

void AMainPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
	if (GetPawn()) UE_LOG(LogTemp, Log, TEXT("Server : %s"), *GetPawn()->GetPlayerState()->GetPlayerName());
}

void AMainPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
	if (GetPawn()) UE_LOG(LogTemp, Log, TEXT("Client : %s"), *GetPawn()->GetPlayerState()->GetPlayerName());
}

void AMainPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->HealthBar &&
		PlayerHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		PlayerHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		PlayerHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeCharacterOvelay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void AMainPlayerController::SetHUDScore(float Score)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		PlayerHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeCharacterOvelay = true;
		HUDScore = Score;
	}
}

void AMainPlayerController::SetHUDDefeats(int32 Defeats)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"),Defeats);
		PlayerHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializeCharacterOvelay = true;
		HUDDefeats = Defeats;
	}
}

void AMainPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->AmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		PlayerHUD->CharacterOverlay->AmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AMainPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		FString CarriedAmmoText = FString::Printf(TEXT("%d"), Ammo);
		PlayerHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(CarriedAmmoText));
	}
}

void AMainPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;
		FString MatchCountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PlayerHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(MatchCountdownText));
	}
}

float AMainPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AMainPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AMainPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD)
		{
			PlayerHUD->AddCharacterOvelay();
		}
	}
}

void AMainPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD)
		{
			PlayerHUD->AddCharacterOvelay();
		}
	}
}

void AMainPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (PlayerHUD && PlayerHUD->CharacterOverlay)
		{
			CharacterOverlay = PlayerHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				SetHUDHealth(HUDHealth, HUDMaxHealth);
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
}
