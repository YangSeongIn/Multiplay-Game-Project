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
#include "../HUD/Announcement.h"
#include "Kismet/GameplayStatics.h"
#include "../GameState/MainGameState.h"
#include "Engine/Texture2D.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../Weapon/Weapon.h"
#include "Components/Image.h"
#include "../CharacterMeshCapture/CharacterMeshCapture.h"
#include "../HUD/CustomizingWidget.h"
#include "../PlayerState/MainPlayerState.h"
#include "../SaveGameData/SaveGameData.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<APlayerHUD>(GetHUD());
	ServerCheckMatchState();

	AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(PlayerState);
	if (MainPlayerState)
	{
		if (UGameplayStatics::DoesSaveGameExist(SaveDataName, 0))
		{
			MainPlayerState->SaveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveDataName, 0));
		}
		else
		{
			MainPlayerState->SaveGameData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
		}
	}

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "GameStartupMap")
	{
		Possess(Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass())));
	}
}

void AMainPlayerController::Init()
{
	if (MeshCapture == nullptr)
	{
		TArray<AActor*> Locs;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MeshCaptureLocation"), Locs);
		if (Locs.Num() > 0 && Locs[0])
		{
			Locs[0]->SetActorHiddenInGame(true);
			UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL,
				TEXT("/Script/Engine.Blueprint'/Game/Blueprints/CharacterMeshCapture/BP_CharacterMeshCapture.BP_CharacterMeshCapture'")));
			if (SpawnActor)
			{
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
				if (GeneratedBP)
				{
					UWorld* World = GetWorld();
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					MeshCapture = World->SpawnActor<ACharacterMeshCapture>(GeneratedBP->GeneratedClass, Locs[0]->GetActorLocation(), Locs[0]->GetActorRotation(), SpawnParams);
				}
			}
		}
	}
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(InPawn);
	if (MainCharacter)
	{
		SetHUDHealth(MainCharacter->GetHealth(), MainCharacter->GetMaxHealth());
		Init();

		FTimerHandle WaitHandle;
		float WaitTime = 0.05f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([WeakThis = TWeakObjectPtr<AMainPlayerController>(this), InPawn]()
			{
				if (WeakThis.IsValid())
				{
					AMainGameMode* GameMode = Cast<AMainGameMode>(WeakThis->GetWorld()->GetAuthGameMode());
					if (GameMode)
					{
						GameMode->OnPlayerPossessCharacter(WeakThis.Get(), InPawn);
					}
				}
				
			}), WaitTime, false);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
	CheckPing(DeltaTime);
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

void AMainPlayerController::HighPingWarning()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->HighPingImage &&
		PlayerHUD->CharacterOverlay->HighPingAnimation;
	if (bHUDValid)
	{
		PlayerHUD->CharacterOverlay->HighPingImage->SetOpacity(1.f);
		PlayerHUD->CharacterOverlay->PlayAnimation(PlayerHUD->CharacterOverlay->HighPingAnimation, 0.f, 5);
	}
}

void AMainPlayerController::StopHighPingWarning()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->HighPingImage &&
		PlayerHUD->CharacterOverlay->HighPingAnimation;
	if (bHUDValid)
	{
		PlayerHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);
		if (PlayerHUD->CharacterOverlay->IsAnimationPlaying(PlayerHUD->CharacterOverlay->HighPingAnimation))
		{
			PlayerHUD->CharacterOverlay->StopAnimation(PlayerHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}

void AMainPlayerController::CheckPing(float DeltaTime)
{
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime == CheckPingFrequency)
	{
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState;
		if (PlayerState)
		{
			if (PlayerState->GetPingInMilliseconds() * 4 > HighPingThreshold)
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
			}
		}
		HighPingRunningTime = 0.f;
	}
	bool bHighPingAnimationPlaying =
		PlayerHUD && PlayerHUD->CharacterOverlay &&
		PlayerHUD->CharacterOverlay->HighPingAnimation &&
		PlayerHUD->CharacterOverlay->IsAnimationPlaying(PlayerHUD->CharacterOverlay->HighPingAnimation);
	if (bHighPingAnimationPlaying)
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

void AMainPlayerController::UpdateWeaponState()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetPawn());
	if (MainCharacter && CharacterOverlay)
	{
		if (MainCharacter->GetWeapon1() == nullptr)
		{
			//CharacterOverlay->ActivateWeapon(false, 0);
			CharacterOverlay->SetWeaponImage(nullptr, 0);
		}
		else if (MainCharacter->GetWeapon1() && MainCharacter->GetWeapon1() == MainCharacter->GetEquippedWeapon())
		{
			CharacterOverlay->ActivateWeapon(true, 0);
		}
		else if (MainCharacter->GetWeapon1() && MainCharacter->GetWeapon1() != MainCharacter->GetEquippedWeapon())
		{
			CharacterOverlay->ActivateWeapon(false, 0);
		}

		if (MainCharacter->GetWeapon2() == nullptr)
		{
			// CharacterOverlay->ActivateWeapon(false, 1);
			CharacterOverlay->SetWeaponImage(nullptr, 1);
		}
		else if (MainCharacter->GetWeapon2() && MainCharacter->GetWeapon2() == MainCharacter->GetEquippedWeapon())
		{
			CharacterOverlay->ActivateWeapon(true, 1);
		}
		else if (MainCharacter->GetWeapon2() && MainCharacter->GetWeapon2() != MainCharacter->GetEquippedWeapon())
		{
			CharacterOverlay->ActivateWeapon(false, 1);
		}
	}
}

void AMainPlayerController::UpdateMeshCaptureCustomizingInfo(FCustomizingSaveDataStruct Data)
{
	/*if (MeshCapture && OwningCharacter)
	{
		MeshCapture->HairMesh = OwningCharacter->Hairs[Data.HairIndex];
		MeshCapture->GoggleMesh = OwningCharacter->Goggles[Data.GoggleIndex];
		MeshCapture->BeardMesh = OwningCharacter->Beards[Data.BeardIndex];
		MeshCapture->UpperBodyMesh = OwningCharacter->UpperBodies[Data.UpperBodyIndex];
		MeshCapture->LowerBodyMesh = OwningCharacter->LowerBodies[Data.LowerBodyIndex];
	}*/
}

void AMainPlayerController::SetWeaponImage(int32 Num)
{
	if (CharacterOverlay)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetPawn());
		if (MainCharacter)
		{
			AWeapon* TargetWeapon = Num == 0 ? MainCharacter->GetWeapon1() : MainCharacter->GetWeapon2();
			UTexture2D* NewTexture = MainCharacter->GetInventoryComponent()->GetWeaponImage(TargetWeapon, true);
			CharacterOverlay->SetWeaponImage(NewTexture, Num);
		}
	}
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

void AMainPlayerController::ServerCheckMatchState_Implementation()
{
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime);
	}
}

void AMainPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (PlayerHUD && MatchState == MatchState::WaitingToStart)
	{
		if (PlayerHUD)
		{
			PlayerHUD->AddAnnouncement();
		}
	}
}

void AMainPlayerController::SetHUDTime()
{
	// set time
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;

	// show time
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}
	CountdownInt = SecondsLeft;
}

// notifying client from server
void AMainPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	OwningCharacter = Cast<AMainCharacter>(GetPawn());

	if (OwningCharacter)
	{
		Init();
	}
}

void AMainPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AMainPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
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
		if (Ammo == -1)
		{
			PlayerHUD->CharacterOverlay->AmmoAmount->SetText(FText::FromString("-"));
		}
		else
		{
			FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
			PlayerHUD->CharacterOverlay->AmmoAmount->SetText(FText::FromString(AmmoText));
		}
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
		if (Ammo == -1)
		{
			PlayerHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString("-"));
		}
		else
		{
			FString CarriedAmmoText = FString::Printf(TEXT("%d"), Ammo);
			PlayerHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(CarriedAmmoText));
		}
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

void AMainPlayerController::SetHUDAnnouncementCountdown(float Warmup)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool bHUDValid = PlayerHUD &&
		PlayerHUD->Announcement &&
		PlayerHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(Warmup / 60.f);
		int32 Seconds = Warmup - Minutes * 60;
		FString WarmupTimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PlayerHUD->Announcement->WarmupTime->SetText(FText::FromString(WarmupTimeText));
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
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AMainPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AMainPlayerController::HandleMatchHasStarted()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		if (PlayerHUD->CharacterOverlay == nullptr) PlayerHUD->AddCharacterOvelay();
		if (PlayerHUD->Announcement)
		{
			PlayerHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void AMainPlayerController::HandleCooldown()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		PlayerHUD->CharacterOverlay->RemoveFromParent();
		if (PlayerHUD->Announcement)
		{
			PlayerHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

UCustomizingWidget* AMainPlayerController::CreateCustomizingWidget(TSubclassOf<UUserWidget> Widget)
{
	if (Widget)
	{
		CustomizingWidget = Cast<UCustomizingWidget>(CreateWidget(GetWorld(), Widget));

	}
	return CustomizingWidget;
}

FCustomizingSaveDataStruct AMainPlayerController::GetSaveGameData()
{
	AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(PlayerState);
	if (MainPlayerState)
	{
		return MainPlayerState->GetSaveGameData();
	}
	return FCustomizingSaveDataStruct{ 0, 0, 0, 0, 0 };
}

void AMainPlayerController::SaveData(FCustomizingSaveDataStruct Data)
{
	AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(PlayerState);
	if (MainPlayerState)
	{
		MainPlayerState->SaveData(Data);
	}
}
