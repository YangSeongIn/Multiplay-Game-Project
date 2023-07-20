// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "../Character/MainCharacter.h"
#include "../Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../PlayerController/MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "ItemDataComponent.h"
#include "InventoryComponent.h"
#include "../Pickups/Item.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;

}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UCombatComponent, CombatState);
	DOREPLIFETIME(UCombatComponent, Weapon1);
	DOREPLIFETIME(UCombatComponent, Weapon2);
	DOREPLIFETIME(UCombatComponent, Weapons);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if (Character->GetFollowCamera())
		{
			DefaultFOV = Character->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
		if (Character->HasAuthority())
		{
			InitializeCarriedAmmo();
		}
	}


	Weapons.SetNum(2);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		ServerFire(HitTarget);
		if (EquippedWeapon)
		{
			CrosshairShootingFactor += 0.75f;
		}
		StartFireTimer();

		// Fix Text

		InventoryComponent->UpdateWeaponInfoSlot(this, EquippedWeapon, EquippedWeapon->GetAmmo(), GetCarriedAmmo(EquippedWeapon->GetWeaponType()));
	}
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay
	);
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;
	if (bFireButtonPressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}
	if (EquippedWeapon->IsEmpty())
	{
		Reload();
	}
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFire(TraceHitTarget);
}

void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr) return;
	if (Character && CombatState == ECombatState::ECS_Unoccupied)
	{
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::GetWeapon(class AWeapon* WeaponToEquip)
{

}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	// weapon inventory full
	if (Weapon1 && Weapon2)
	{
		AWeapon*& WeaponTemp = EquippedWeapon == Weapon1 ? Weapon1 : Weapon2;
		EquippedWeapon->Dropped();
		WeaponTemp = WeaponToEquip;
		EquipOnHand(WeaponTemp);

		//InventoryComponent->AddToWeaponSlot(EquippedWeapon->GetItemDataComponent()->GetItemID().RowName.ToString(), EquippedWeapon->GetAmmo(), EquippedWeapon->GetCarriedAmmo(), EquippedWeapon->GetItemDataComponent()->GetItemType());
	}
	// Weapon slot is empty
	else if (Weapon1 == nullptr && Weapon2 == nullptr)
	{
		Weapon1 = WeaponToEquip;
		EquipOnHand(Weapon1);

		//InventoryComponent->AddToWeaponSlot(Weapon1->GetItemDataComponent()->GetItemID().RowName.ToString(), Weapon1->GetAmmo(), Weapon1->GetCarriedAmmo(), Weapon1->GetItemDataComponent()->GetItemType());
	}
	// Empty weapon slot exist
	else
	{
		if (Weapon1 == nullptr) 
		{
			Weapon1 = WeaponToEquip;
			//InventoryComponent->AddToWeaponSlot(Weapon1->GetItemDataComponent()->GetItemID().RowName.ToString(), Weapon1->GetAmmo(), Weapon1->GetCarriedAmmo(), Weapon1->GetItemDataComponent()->GetItemType());
		}
		else if (Weapon2 == nullptr)
		{
			Weapon2 = WeaponToEquip;
			//InventoryComponent->AddToWeaponSlot(Weapon2->GetItemDataComponent()->GetItemID().RowName.ToString(), Weapon2->GetAmmo(), Weapon2->GetCarriedAmmo(), Weapon2->GetItemDataComponent()->GetItemType());
		}

		if (EquippedWeapon == nullptr)
		{
			EquipOnHand(WeaponToEquip);
		}
		else
		{
			EquipOnBack(WeaponToEquip);
		}
	}

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;

	if (WeaponToEquip->GetItemDataComponent())
	{
		WeaponToEquip->GetItemDataComponent()->Interact(Character);
	}
}

void UCombatComponent::EquipOnHand(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	CombatState = ECombatState::ECS_Unoccupied;
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	PlayEquipWeaponSound(WeaponToEquip);
	ReloadEmptyWeapon();
}

void UCombatComponent::EquipOnBack(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBack(WeaponToEquip);
	PlayEquipWeaponSound(WeaponToEquip);
	SecondaryWeapon->SetOwner(Character);
}

// when swap weapon, this function doesn't work
void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachActorToRightHand(EquippedWeapon);
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
		PlayEquipWeaponSound(EquippedWeapon);
		EquippedWeapon->SetHUDAmmo();
		UpdateCarriedAmmo();
	}
}

void UCombatComponent::OnRep_SecondaryWeapon(AWeapon* PrevWeapon)
{
	if (SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
		AttachActorToBack(SecondaryWeapon);

		if (PrevWeapon == nullptr)
		{
			SecondaryWeapon->SetOwner(Character);
			PlayEquipWeaponSound(SecondaryWeapon);
		}	
	}
}

void UCombatComponent::SwapWeapon(int32 WeaponNum)
{
	if (Weapons[WeaponNum] == EquippedWeapon) return;
	if (EquippedWeapon == nullptr || SecondaryWeapon == nullptr) return;
	AWeapon* WeaponTemp = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = WeaponTemp;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	PlayEquipWeaponSound(EquippedWeapon);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	CombatState = ECombatState::ECS_Unoccupied;

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);

	AttachActorToRightHand(EquippedWeapon);
	AttachActorToBack(SecondaryWeapon);

	MulticastSwapWeapon(EquippedWeapon, SecondaryWeapon);
}

void UCombatComponent::MulticastSwapWeapon_Implementation(AWeapon* WeaponToHand, AWeapon* WeaponToBack)
{

	Character->StopReloadMontage();

}

void UCombatComponent::SwapTwoWeapons()
{
	AWeapon* EquippedWeaponTemp = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = EquippedWeaponTemp;

	AWeapon* WeaponTemp = Weapon1;
	Weapon1 = Weapon2;
	Weapon2 = WeaponTemp;

	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		PlayEquipWeaponSound(EquippedWeapon);
		EquippedWeapon->SetHUDAmmo();
		UpdateCarriedAmmo();
		AttachActorToRightHand(EquippedWeapon);
	}

	CombatState = ECombatState::ECS_Unoccupied;
	if (SecondaryWeapon)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
		AttachActorToBack(SecondaryWeapon);
	}
}

void UCombatComponent::AttachActorToBack(AActor* ActorToAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* WeaponSocket = Character->GetMesh()->GetSocketByName(FName("WeaponSocket"));
	if (WeaponSocket)
	{
		WeaponSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::EquipWeaponByAroundItem(EWeaponNum NumOfWeapon, FString InherenceName, AItem* DraggedItem)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("weapon num : %d"), NumOfWeapon));
	AWeapon* DraggedWeapon = Cast<AWeapon>(DraggedItem);
	AWeapon*& TargetWeapon = NumOfWeapon == EWeaponNum::EWN_Weapon1 ? Weapon1 : Weapon2;
	AWeapon*& RestWeapon = NumOfWeapon != EWeaponNum::EWN_Weapon1 ? Weapon1 : Weapon2;
	if (DraggedWeapon)
	{
		if (TargetWeapon)
		{
			// TargetWeapon -> Hand
			if (TargetWeapon == EquippedWeapon)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("11")));
				EquippedWeapon = nullptr;	
				TargetWeapon->Dropped();
				TargetWeapon = DraggedWeapon;
				EquipOnHand(DraggedWeapon);
			}
			// TargetWeapon -> Back
			else if (TargetWeapon != EquippedWeapon)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("22")));
				SecondaryWeapon = nullptr;
				TargetWeapon->Dropped();
				TargetWeapon = DraggedWeapon;
				EquipOnBack(DraggedWeapon);
			}
		}
		else
		{
			if (RestWeapon && RestWeapon == EquippedWeapon)
			{
				TargetWeapon = DraggedWeapon;
				EquipOnBack(DraggedWeapon);
			}
			else if (RestWeapon && RestWeapon == SecondaryWeapon)
			{
				TargetWeapon = DraggedWeapon;
				EquipOnHand(DraggedWeapon);
			}
			else if (NumOfWeapon == EWeaponNum::EWN_Weapon1)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("null 1")));
				Weapon1 = DraggedWeapon;
				EquipOnHand(DraggedWeapon);
			}
			else if (NumOfWeapon == EWeaponNum::EWN_Weapon2)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("null 2")));
				Weapon2 = DraggedWeapon;
				EquipOnBack(DraggedWeapon);
			}
		}
	
		// EquipBackOrHand(DraggedWeapon);
		/*if (Character && Character->HasAuthority())
		{
			EquipBackOrHand(DraggedWeapon);
		}
		else
		{
			ServerEquipWeaponByAroundItem(DraggedWeapon);
		}*/
	}
}

void UCombatComponent::ServerEquipWeaponByAroundItem_Implementation(AWeapon* WeaponToDrag)
{
	EquipBackOrHand(WeaponToDrag);
}

void UCombatComponent::EquipBackOrHand(AWeapon* WeaponToDrag)
{
	if (EquippedWeapon)
	{
		EquipOnBack(WeaponToDrag);
	}
	else
	{
		EquipOnHand(WeaponToDrag);
	}
}

void UCombatComponent::DropWeapon(AWeapon* WeaponToDrop)
{
	/*if (WeaponToDrop)
	{
		if (WeaponToDrop == Weapon1) 
		{
			Weapon1 = nullptr;
			Weapon1->Dropped();
		}
		else if (WeaponToDrop == Weapon2) 
		{
			Weapon2 = nullptr;
			Weapon2->Dropped();
		}
		
	}*/
	if (WeaponToDrop == nullptr) return;
	WeaponToDrop = nullptr;
	WeaponToDrop->Dropped();
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr)
	{
		Controller->SetHUDCarriedAmmo(-1);
		return;
	}
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip)
{
	if (EquippedWeapon && WeaponToEquip && WeaponToEquip->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			WeaponToEquip->EquipSound,
			Character->GetActorLocation()
		);
	}
}

void UCombatComponent::ReloadEmptyWeapon()
{
	if (EquippedWeapon && EquippedWeapon->IsEmpty())
	{
		Reload();
	}
}

void UCombatComponent::Reload()
{
	if (CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon && !EquippedWeapon->IsFull())
	{
		ServerReload();
	}
}

void UCombatComponent::ServerReload_Implementation()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;

	CombatState = ECombatState::ECS_Reloading;
	HandleReload();
}

void UCombatComponent::FinishReloading()
{
	if (Character == nullptr) return;
	if (Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}
	if (bFireButtonPressed)
	{
		Fire();
	}

	if (InventoryComponent)
	{
		InventoryComponent->UpdateWeaponInfoSlot(this, EquippedWeapon, EquippedWeapon->GetAmmo(), GetCarriedAmmo(EquippedWeapon->GetWeaponType()));
	}
}

void UCombatComponent::UpdateAmmoValues()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	int32 ReloadAmount = AmountToReload();	// Used Amount
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
	EquippedWeapon->AddAmmo(-ReloadAmount);
}

void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	if (CarriedAmmoMap.Contains(WeaponType))
	{
		CarriedAmmoMap[WeaponType] =  FMath::Clamp(CarriedAmmoMap[WeaponType] + AmmoAmount, 0, MaxARAmmo);

		UpdateCarriedAmmo();
	}
	if (EquippedWeapon && EquippedWeapon->IsEmpty() && EquippedWeapon->GetWeaponType() == WeaponType)
	{
		Reload();
	}
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Reloading:
		HandleReload();
		break;
	case ECombatState::ECS_Unoccupied:
		if (bFireButtonPressed)
		{
			Fire();
		}
		break;
	default:
		break;
	}
}

void UCombatComponent::HandleReload()
{
	Character->PlayReloadMontage();
}

int32 UCombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;
	int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least);
	}

	return 0;
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		}

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGHT;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr) return;
	Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		HUD = HUD == nullptr ? Cast<APlayerHUD>(Controller->GetHUD()) : HUD;
		if (HUD)
		{
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairBottom = EquippedWeapon->CrosshairsBottom;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairTop = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
			}
			// [0, MaxWalkSpeed] -> [0, 1]
			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			if (bAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			HUDPackage.CrosshairSpread =
				0.5f +
				CrosshairVelocityFactor +
				CrosshairInAirFactor -
				CrosshairAimFactor +
				CrosshairShootingFactor;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	if (Character && Character->GetFollowCamera())
	{
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;	// without this, server(also clients) does not aiming
	ServerSetAiming(bIsAiming);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr) return false;
	return !EquippedWeapon->IsEmpty() && bCanFire && CombatState == ECombatState::ECS_Unoccupied;
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	Controller = Controller == nullptr ? Cast<AMainPlayerController>(Character->Controller) : nullptr;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
}
