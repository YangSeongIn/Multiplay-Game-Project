// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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

	/**
	* Replication notifies
	*/
	virtual void OnRep_Score() override;
	UFUNCTION()
		void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

private:
	UPROPERTY()
		class AMainCharacter* Character;
	UPROPERTY()
		class AMainPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
		int32 Defeats;
	
};
