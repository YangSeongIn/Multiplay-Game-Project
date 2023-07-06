// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../HUD/PlayerHUD.h"
#include "../Weapon//WeaponTypes.h"
#include "../Types/CombatState.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGHT 80000.f

class AWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLAYGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class AMainCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	void Reload();
	UFUNCTION(BlueprintCallable)
		void FinishReloading();

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);
	void SwapWeapon(int32 WeaponNum);

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
		void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
		void OnRep_EquippedWeapon();

	UFUNCTION()
		void OnRep_Weapons(TArray<AWeapon*> PreviousWeapons);

	void FireButtonPressed(bool bPressed);

	void Fire();

	UFUNCTION(Server, Reliable)
		void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
		void ServerReload();

	void HandleReload();

	int32 AmountToReload();

	UFUNCTION(Server, Reliable)
		void ServerSwapWeapon(int32 WeaponNum);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSwapWeapon(int32 WeaponNum);

	UFUNCTION(Server, Reliable)
		void ServerAttachActorToBack(AActor* ActorToAttach, const class USkeletalMeshSocket* WeaponSocket);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAttachActorToBack(AActor* ActorToAttach, const class USkeletalMeshSocket* WeaponSocket);

	UFUNCTION(Server, Reliable)
		void ServerAttachActorToRightHand(AActor* ActorToAttach, const class USkeletalMeshSocket* HandSocket);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAttachActorToRightHand(AActor* ActorToAttach, const class USkeletalMeshSocket* HandSocket);

private:
	UPROPERTY()
		class AMainCharacter* Character;
	UPROPERTY()
		class AMainPlayerController* Controller;
	UPROPERTY()
		class APlayerHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
		AWeapon* EquippedWeapon;

	//UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Weapons)
	//	TArray<AWeapon*> Weapons;

	UPROPERTY(VisibleAnywhere)
		TMap<AWeapon*, int32> WeaponLoc;

	UPROPERTY()
		bool bUsedBackSocket1 = false;

	UPROPERTY()
		bool bUsedBackSocket2 = false;

	UPROPERTY(VisibleAnywhere)
		TMap<AActor*, int32> WeaponSocketLoc;

	UPROPERTY(Replicated)
		bool bAiming;

	UPROPERTY(EditAnywhere)
		float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
		float AimWalkSpeed;
	
	bool bFireButtonPressed;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;

	FHUDPackage HUDPackage;

	/**
	* Aiming and FOV
	*/

	// Field of view when not aiming, set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/**
	* Automatic fire
	*/

	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
		int32 CarriedAmmo;

	UFUNCTION()
		void OnRep_CarriedAmmo();

	UPROPERTY(EditAnywhere)
		TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
		int32 StartingARAmmo = 45;

	UPROPERTY(EditAnywhere)
		int32 StartingRocketAmmo = 12;

	UPROPERTY(EditAnywhere)
		int32 MaxARAmmo = 90;

	UPROPERTY(EditAnywhere)
		int32 MaxRocketAmmo = 24;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
		ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
		void OnRep_CombatState();

	void UpdateAmmoValues();

	void DropWeapon(AWeapon* WeaponToDrop);
	void AttachActorToRightHand(AActor* ActorToAttach);
	void UpdateCarriedAmmo();
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);
	void ReloadEmptyWeapon();
	void EquipOnHand(AWeapon* WeaponToEquip);
	void EquipOnBack(AWeapon* WeaponToEquip);
	void AttachActorToBack(AActor* ActorToAttach);

public:
	int32 GetEmptyWeapon();


};
