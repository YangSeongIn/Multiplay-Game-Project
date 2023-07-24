// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../Types/TurningInPlace.h"
#include "../Interfaces/InteractWithCrosshairsInterface.h"
#include "../Types/CombatState.h"
#include "MainCharacter.generated.h"

UCLASS()
class MULTIPLAYGAME_API AMainCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();
	void PlayReloadMontage();
	void StopReloadMontage();
	virtual void OnRep_ReplicatedMovement() override;
	void Elim();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastElim();

	UFUNCTION(Server, Reliable)
		void ServerSetMeshCapture();

	UFUNCTION(Server, Reliable)
		void ServerInventoryWidget();

	UFUNCTION(Client, Reliable)
		void ClientSetMeshCapture(int32 n, class ACharacterMeshCapture* MeshCapture);

	/*UFUNCTION(Server, Reliable)
		void ServerAddPlayerNum();*/

	void EquipButtonPressed();

	void AttachItemOnMeshCapture(const FString SocketName);
	void DetachItemOnMeshCapture(const FString SocketName);

	UFUNCTION(Server, Reliable)
		void ServerAttachItemOnMeshCapture(const FString& SocketName);

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void CrouchButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();
	
	void AimButtonPressed();
	void AimButtonReleased();
	void Esc();
	void ReloadButtonPressed();
	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	void PlayHitReactMontage();
	void SelectPrimaryWeapon();
	void SelectSecondaryWeapon();
	void SelectTertiaryWeapon();
	void InventoryKeyPressed();
	virtual void Jump() override;
	UFUNCTION()
		void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();

	// Poll for any relevant classes and initialize our HUD
	void PollInit();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* OverheadWidget;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_OverlappingItem) // Using Callback when property update
		class AItem* OverlappingItem;

	UFUNCTION()
		void OnRep_OverlappingItem(class AItem* Item);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	UFUNCTION(Server, Reliable)
		void ServerEquipButtonPressed();

	UFUNCTION(Server, Reliable)
		void ServerWeaponSwapButtonPressed(int32 WeaponNum);

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UFUNCTION()
		void OnMontageNotifyBegin();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastMontageMotify();

	FScriptDelegate Delegate_OnMontageNotifyBegin;

	/**
	* Animation montages
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		UAnimMontage* ReloadMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
		float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/**
	* Player Health
	*/

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
		float Health = 100.f;

	UFUNCTION()
		void OnRep_Health();

	UPROPERTY()
		class AMainPlayerController* MainPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
		float ElimDelay = 5.f;

	void ElimTimerFinished();

	UPROPERTY()
		class AMainPlayerState* MainPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* EscAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PrimaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SecondaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* TertiaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACharacterMeshCapture> CharacterMeshCaptureClass;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryWidget)
		class UInventory* InventoryWidget;

	UFUNCTION()
		void OnRep_InventoryWidget(class UInventory* PostInventoryWidget);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerInherenceNum, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 PlayerInherenceNum;

	UFUNCTION()
		void OnRep_PlayerInherenceNum();

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CharacterMeshCapture, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class ACharacterMeshCapture* CharacterMeshCapture;

	UFUNCTION()
		void OnRep_CharacterMeshCapture();

public:
	void SetOverlappingItem(class AItem* Item);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; };
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; };
	class AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; };
	FORCEINLINE void SetTurningInPlace(ETurningInPlace Type) { TurningInPlace = Type; };
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; };
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; };
	FORCEINLINE bool IsElimmed() const { return bElimmed; };
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; };
	ECombatState GetCombatState() const;
	UCombatComponent* GetCombatComponent() const { return CombatComponent; };
	class AWeapon* GetEquippedWeapon() const;
	class AWeapon* GetSecondaryWeapon() const;
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; };
	FORCEINLINE void SetInventoryWidgetNull() { InventoryWidget = nullptr; };
	FORCEINLINE void SetMeshCapture(class ACharacterMeshCapture* NewMeshCapture) { CharacterMeshCapture = NewMeshCapture; };
	AWeapon* GetWeapon1();
	AWeapon* GetWeapon2();
	int32 GetCarriedAmmo(class AWeapon* Weapon);
	FORCEINLINE FVector GetGroundLocation();
};
