// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "../Character/MainCharacter.h"
#include "../PlayerController/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "../PlayerState/MainPlayerState.h"
#include "../GameState/MainGameState.h"
#include "../CharacterMeshCapture/CharacterMeshCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Net/UnrealNetwork.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");	// Match duration has been reached.
}

AMainGameMode::AMainGameMode()
{
	bDelayedStart = true;

}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();

	/*UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterMeshCapture::StaticClass(), MeshCaptures);*/
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void AMainGameMode::OnPlayerPossessCharacter(AMainPlayerController* PossessedController, APawn* PossessedPawn)
{
	Controllers.Add(PossessedController);
	for (AMainPlayerController* CurrentController : Controllers)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(CurrentController->GetPawn());
		if (Character)
		{
			Character->MulticastApplyCustomizingInfo(CurrentController->GetSaveGameData());
		}
	}
}

AActor* AMainGameMode::GetMeshCapture(int32 n)
{
	if (MeshCaptures.Num() <= n) return nullptr;
	return MeshCaptures[n];
}

void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
}

void AMainGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(*It);
		if (MainPlayerController)
		{
			MainPlayerController->OnMatchStateSet(MatchState);
		}
	}
}

void AMainGameMode::PlayerEliminated(AMainCharacter* ElimmedCharacter, AMainPlayerController* VictimController, AMainPlayerController* AttackerController)
{
	AMainPlayerState* AttackerPlayerState = AttackerController ? Cast<AMainPlayerState>(AttackerController->PlayerState) : nullptr;
	AMainPlayerState* VictimPlayerState = VictimController ? Cast<AMainPlayerState>(VictimController->PlayerState) : nullptr;

	AMainGameState* MainGameState = GetGameState<AMainGameState>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && MainGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
		MainGameState->UpdateTopScore(AttackerPlayerState);
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AMainGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void AMainGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameMode, MeshCaptures);
}
