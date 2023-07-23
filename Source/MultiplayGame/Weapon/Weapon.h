// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "../Types/EquippedSlotType.h"
#include "../Pickups/Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class MULTIPLAYGAME_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	void SetHUDAmmo();
	virtual void Fire(const FVector& HitTarget);
	void Dropped();
	void AddAmmo(int32 AmmoToAdd);

	// Texture for the weapon crosshair
	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsBottom;

	/**
	* Zoomed FOV while aiming
	*/

	UPROPERTY(EditAnywhere)
		float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
		float ZoomInterpSpeed = 20.f;

	/**
	* Automatic Fire
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireDelay = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		bool bAutomatic = true;

	UPROPERTY(EditAnywhere)
		class USoundCue* EquipSound;

	void EnableCustomDepth(bool bEnable);

	int32 GetCarriedAmmo();

protected:
	virtual void BeginPlay() override;
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

	virtual void OnSphereOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USkeletalMeshComponent* WeaponMesh;

	/*UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class USphereComponent* AreaSphere;*/

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
		EWeaponState WeaponState;

	UFUNCTION()
		void OnRep_WeaponState();
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
		int32 Ammo;

	UFUNCTION()
		void OnRep_Ammo();

	void SpendRound();

	UPROPERTY(EditAnywhere)
		int32 MagCapacity = 30;

	UPROPERTY()
		class AMainCharacter* OwnerCharacter;

	UPROPERTY()
		class AMainPlayerController* OwnerController;

	UPROPERTY(EditAnywhere)
		EWeaponType WeaponType;

	EEquippedSlotType EquippedSlotType = EEquippedSlotType::EST_Weapon;

public:
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; };
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; };
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; };
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; };
	bool IsEmpty();
	bool IsFull();
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; };
	FORCEINLINE int32 GetAmmo() const { return Ammo; };
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; };
	FORCEINLINE EEquippedSlotType GetEquippedSlotType() { return EquippedSlotType; };
};
