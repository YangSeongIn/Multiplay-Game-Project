// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	int32 PlayerNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		TArray<AActor*> MeshCaptures;

	UFUNCTION(NetMultiCast, Reliable)
		void MulticastOnStart();

	// void SetMeshCapture(UWorld* World, AController* Controller, USkeletalMeshComponent* SkeletalMeshComp, int32& PlayerInherenceNum);
	protected:
		virtual void OnStart() override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AActor* GetMeshCapture(int32 n);
	bool CanAddPlayerNum();
	FORCEINLINE int32 GetPlayerNum() { return PlayerNum; };
	FORCEINLINE void SetPlayerNum(int32 NewNum) { PlayerNum = NewNum; };

};
