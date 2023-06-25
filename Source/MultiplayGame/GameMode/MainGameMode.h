// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

namespace MatchState
{
	extern MULTIPLAYGAME_API const FName Cooldown;	// Match duration has been reached.
}

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMainGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class AMainCharacter* ElimmedCharacter, class AMainPlayerController* VictimController, AMainPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);

	UPROPERTY(EditDefaultsOnly)
		float WarmupTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
		float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
		float CooldownTime = 10.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:
	float CountdownTime = 0.f;
};
