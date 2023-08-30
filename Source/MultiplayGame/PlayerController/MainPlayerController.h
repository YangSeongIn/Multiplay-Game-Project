// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float GetServerTime(); // Sync with server world clock
	virtual void ReceivedPlayer() override;	// Sync with server clock as soons as possible
	void OnMatchStateSet(FName State);
	void HandleMatchHasStarted();
	void HandleCooldown();
	void SetWeaponImage(int32 Num);
	void UpdateWeaponState();

	void UpdateMeshCaptureCustomizingInfo(FCustomizingSaveDataStruct Data);

	UPROPERTY()
	TObjectPtr<class AMainCharacter> OwningCharacter = nullptr;

	UFUNCTION(BlueprintCallable)
	class UCustomizingWidget* CreateCustomizingWidget(TSubclassOf<UUserWidget> Widget);

	UPROPERTY()
	class UCustomizingWidget* CustomizingWidget;

	void SaveData(FCustomizingSaveDataStruct Data);

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();
	virtual void OnRep_Pawn() override;

	/**
	* Sync time between client and server
	*/

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
		void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
		void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f;	// abs(server time - client time)

	UPROPERTY(EditAnywhere, Category = Time)
		float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);
	void PollInit();

	UFUNCTION(Server, Reliable)
		void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
		void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float StartingTime);

	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaTime);

private:
	UPROPERTY()
		class APlayerHUD* PlayerHUD;

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
		FName MatchState;

	UFUNCTION()
		void OnRep_MatchState();

	UPROPERTY()
		class UCharacterOverlay* CharacterOverlay;

	bool bInitializeCharacterOvelay = false;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;

	float HighPingRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
		float HighPingDuration = 5.f;

	UPROPERTY(EditAnywhere)
		float CheckPingFrequency = 20.f;

	UPROPERTY(EditAnywhere)
		float HighPingThreshold = 50.f;

	float PingAnimationRunningTime = 0.f;

	UPROPERTY()
	class ACharacterMeshCapture* MeshCapture;

	void Init();

	FString SaveDataName = "SaveData";

public:
	FORCEINLINE class ACharacterMeshCapture* GetMeshCapture() { return MeshCapture; };
	FCustomizingSaveDataStruct GetSaveGameData();
};
