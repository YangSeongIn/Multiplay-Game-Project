// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"
#include "../HUD/OverheadWidget.h"
#include "Net/UnrealNetwork.h"
#include "../Weapon/Weapon.h"
#include "../MainCharacterComponent/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacterAnimInstance.h"
#include "../MultiplayGame.h"
#include "../PlayerController/MainPlayerController.h"
#include "../HUD/CharacterOverlay.h"
#include "../GameMode/MainGameMode.h"
#include "TimerManager.h"
#include "../PlayerState/MainPlayerState.h"
#include "../Weapon/WeaponTypes.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../HUD/Inventory.h"
#include "../HUD/InventoryGrid.h"
#include "../CharacterMeshCapture/CharacterMeshCapture.h"
#include "../GameState/MainGameState.h"
#include "../GameInstance/MainGameInstance.h"
#include "../HUD/InventoryWeaponInfo.h"
#include "../HUD/AroundItemGrid.h"
#include "../Pickups/Item.h"
#include "../MainCharacterComponent/ItemDataComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../SaveGameData/SaveGameData.h"
#include "../HUD/CustomizingWidget.h"
#include "../Data/CostomizingDataAsset.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	UpperBodyMesh = GetMesh();
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(UpperBodyMesh);
	LowerBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerBodyMesh"));
	LowerBodyMesh->SetupAttachment(UpperBodyMesh);
	BeardMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeardMesh"));
	BeardMesh->SetupAttachment(UpperBodyMesh);
	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(UpperBodyMesh);
	GoggleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GoggleMesh"));
	GoggleMesh->SetupAttachment(UpperBodyMesh);
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(UpperBodyMesh);

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	HeadMesh->SetLeaderPoseComponent(UpperBodyMesh);
	LowerBodyMesh->SetLeaderPoseComponent(UpperBodyMesh);
	BeardMesh->SetLeaderPoseComponent(UpperBodyMesh);
	HairMesh->SetLeaderPoseComponent(UpperBodyMesh);
	GoggleMesh->SetLeaderPoseComponent(UpperBodyMesh);
	HandMesh->SetLeaderPoseComponent(UpperBodyMesh);

	SetReplicateMovement(true);
}

void AMainCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void AMainCharacter::Elim()
{
	if (bElimmed) return;
	if (CombatComponent && CombatComponent->EquippedWeapon)
	{
		CombatComponent->EquippedWeapon->Dropped();
		CombatComponent->SecondaryWeapon->Dropped();
		InventoryComponent->RemoveAllContents();
	}
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&AMainCharacter::ElimTimerFinished,
		ElimDelay
	);
}

void AMainCharacter::MulticastElim_Implementation()
{
	if (MainPlayerController)
	{
		MainPlayerController->SetHUDWeaponAmmo(0);
	}
	bElimmed = true;
	PlayElimMontage();

	// Disable character movement
	GetCharacterMovement()->DisableMovement();
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
	if (MainPlayerController)
	{
		DisableInput(MainPlayerController);
	}
	// Disable Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::ElimTimerFinished()
{
	AMainGameMode* MainGameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();
	if (MainGameMode)
	{
		MainGameMode->RequestRespawn(this, Controller);
	}
}

void AMainCharacter::MulticastApplyCustomizingInfo_Implementation(FCustomizingSaveDataStruct CustomizingSaveData)
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	HairMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->HairMeshes[CustomizingSaveData.HairIndex].Mesh);
	GoggleMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->GoggleMeshes[CustomizingSaveData.GoggleIndex].Mesh);
	BeardMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->BeardsMeshes[CustomizingSaveData.BeardIndex].Mesh);
	UpperBodyMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->UpperBodyMeshes[CustomizingSaveData.UpperBodyIndex].Mesh);
	LowerBodyMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->LowerBodyMeshes[CustomizingSaveData.LowerBodyIndex].Mesh);
}

void AMainCharacter::ClientUpdateMeshCapture_Implementation(FCustomizingSaveDataStruct CustomizingSaveData)
{
	if (Controller == nullptr) return;
	if (ACharacterMeshCapture* MeshCapture = Cast<AMainPlayerController>(Controller)->GetMeshCapture())
	{
		MeshCapture->UpdateMeshCapture(CustomizingSaveData);
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UpdateHUDHealth();

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &AMainCharacter::ReceiveDamage);
		Delegate_OnMontageNotifyBegin.BindUFunction(this, FName("OnMontageNotifyBegin"));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Add(Delegate_OnMontageNotifyBegin);
	}
}

void AMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	MainPlayerState = Cast<AMainPlayerState>(GetPlayerState());
	if (MainPlayerState == nullptr) 
		return;

	MainPlayerState->SetMeshWithCustomizingInfo();
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MainPlayerState = Cast<AMainPlayerState>(GetPlayerState());
	if (MainPlayerState == nullptr)
		return;

	MainPlayerState->SetMeshWithCustomizingInfo();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
	
	HideCameraIfCharacterClose();
	PollInit();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainCharacter::EquipButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMainCharacter::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainCharacter::AimButtonReleased);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMainCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainCharacter::FireButtonReleased);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::ReloadButtonPressed);
		EnhancedInputComponent->BindAction(PrimaryWeaponAction, ETriggerEvent::Triggered, this, &AMainCharacter::SelectPrimaryWeapon);
		EnhancedInputComponent->BindAction(SecondaryWeaponAction, ETriggerEvent::Triggered, this, &AMainCharacter::SelectSecondaryWeapon);
		EnhancedInputComponent->BindAction(TertiaryWeaponAction, ETriggerEvent::Triggered, this, &AMainCharacter::SelectTertiaryWeapon);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AMainCharacter::InventoryKeyPressed);
	}
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMainCharacter, OverlappingItem, COND_OwnerOnly);
	DOREPLIFETIME(AMainCharacter, Health);
	DOREPLIFETIME_CONDITION(AMainCharacter, InventoryWidget, COND_OwnerOnly);
	DOREPLIFETIME(AMainCharacter, OverlappingItems);
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent && InventoryComponent)
	{
		CombatComponent->Character = this;
		CombatComponent->SetInventoryComponent(InventoryComponent);
		InventoryComponent->Character = this;
		InventoryComponent->SetCombatComponent(CombatComponent);
	}
}

void AMainCharacter::Destroyed()
{
	Super::Destroyed();

	AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	bool bMatchNotInProgress = MainGameMode && MainGameMode->GetMatchState() != MatchState::InProgress;
	if (CombatComponent && CombatComponent->EquippedWeapon && bMatchNotInProgress)
	{
		CombatComponent->EquippedWeapon->Destroy();
	}
}

void AMainCharacter::PlayFireMontage(bool bAiming)
{
	if (InventoryWidget || CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AMainCharacter::PlayElimMontage()
{
	if (CombatComponent == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
		FString SectionName;
		if (IsWeaponEquipped())
		{
			SectionName = FString("Death_Equipped_");
			int32 EquippedMaxIndex = UKismetMathLibrary::RandomIntegerInRange(3, 5);
			SectionName += FString::FromInt(EquippedMaxIndex);
		}
		else
		{
			SectionName = FString("Death_Unequipped_");
			int32 UnequippedMaxIndex = UKismetMathLibrary::RandomIntegerInRange(0, 3);
			SectionName += FString::FromInt(UnequippedMaxIndex);
		}
		AnimInstance->Montage_JumpToSection(FName(SectionName));
	}
}

void AMainCharacter::PlayReloadMontage()
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (CombatComponent->EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_RocketLauncher:
			SectionName = FName("Rocket");
			break;
		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("Sniper");
			break;
		case EWeaponType::EWT_Pistol:
			SectionName = FName("Pistol");
			break;
		case EWeaponType::EWT_SubmachineGun:
			SectionName = FName("SubmachineGun");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AMainCharacter::StopReloadMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Stop(0, ReloadMontage);
	}
}

void AMainCharacter::PlayHitReactMontage()
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		//SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	if (InventoryWidget) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainCharacter::StartJump()
{
	Super::Jump();
}

void AMainCharacter::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AMainCharacter::FireButtonPressed()
{
	if (InventoryWidget) return;
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(true);
	}
}

void AMainCharacter::FireButtonReleased()
{
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
}

void AMainCharacter::EquipButtonPressed()
{
	if (CombatComponent && OverlappingItem)
	{
		Equip(OverlappingItem);
	}
}

void AMainCharacter::Equip(AItem* ItemOverlapped)
{
	if (HasAuthority())
	{
		AWeapon* WeaponToEquip = Cast<AWeapon>(ItemOverlapped);
		// weapon
		if (WeaponToEquip)
		{
			CombatComponent->EquipWeapon(WeaponToEquip);
		}
		// item
		else if (ItemOverlapped->GetItemDataComponent())
		{
			ItemOverlapped->GetItemDataComponent()->Interact(this);
		}
		OverlappingItems.Num() == 0 ? OverlappingItem = nullptr : OverlappingItem = OverlappingItems[0];
	}
	else
	{
		ServerEquipButtonPressed();
	}
}

void AMainCharacter::AttachItemOnMeshCapture(const FString SocketName)
{
	if (IsLocallyControlled())
	{
		CharacterMeshCapture = Cast<AMainPlayerController>(Controller)->GetMeshCapture();
		if (SocketName == "RightHandSocket" && GetEquippedWeapon() && CharacterMeshCapture)
		{
			CharacterMeshCapture->SetSkeletalMeshOnHand(GetEquippedWeapon()->GetWeaponMesh()->GetSkeletalMeshAsset());
		}
		else if (SocketName == "WeaponSocket" && GetSecondaryWeapon() && CharacterMeshCapture)
		{
			CharacterMeshCapture->SetSkeletalMeshOnBack(GetSecondaryWeapon()->GetWeaponMesh()->GetSkeletalMeshAsset());
		}
	}
	else
	{
		ServerAttachItemOnMeshCapture(SocketName);
	}
}

void AMainCharacter::DetachItemOnMeshCapture(const FString SocketName)
{
	if (IsLocallyControlled())
	{
		CharacterMeshCapture = Cast<AMainPlayerController>(Controller)->GetMeshCapture();
		if (SocketName == "RightHandSocket" && CharacterMeshCapture)
		{
			CharacterMeshCapture->SetSkeletalMeshOnHand(nullptr);
		}
		else if (SocketName == "WeaponSocket" && CharacterMeshCapture)
		{
			CharacterMeshCapture->SetSkeletalMeshOnBack(nullptr);
		}
	}
	else
	{
		ServerAttachItemOnMeshCapture(SocketName);
	}
}

void AMainCharacter::ServerAttachItemOnMeshCapture_Implementation(const FString& SocketName)
{
	CharacterMeshCapture = Cast<AMainPlayerController>(Controller)->GetMeshCapture();
	if (SocketName == "RightHandSocket" && GetEquippedWeapon() && CharacterMeshCapture)
	{
		CharacterMeshCapture->SetSkeletalMeshOnHand(GetEquippedWeapon()->GetWeaponMesh()->GetSkeletalMeshAsset());
	}
	else if (SocketName == "WeaponSocket" && GetSecondaryWeapon() && CharacterMeshCapture)
	{
		CharacterMeshCapture->SetSkeletalMeshOnBack(GetSecondaryWeapon()->GetWeaponMesh()->GetSkeletalMeshAsset());
	}
}

void AMainCharacter::AimButtonPressed()
{
	if (InventoryWidget) return;
	if (IsWeaponEquipped())
	{
		CombatComponent->SetAiming(true);
	}
}

void AMainCharacter::AimButtonReleased()
{
	if (CombatComponent)
	{
		CombatComponent->SetAiming(false);
	}
}

void AMainCharacter::Esc()
{
	
}

void AMainCharacter::ReloadButtonPressed()
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;
	CombatComponent->Reload();
}

void AMainCharacter::SelectPrimaryWeapon()
{
	if (CombatComponent == nullptr) return;
	if (HasAuthority())
	{
		CombatComponent->SwapWeapon(0);
	}
	else
	{
		ServerWeaponSwapButtonPressed(0);
	}
}

void AMainCharacter::SelectSecondaryWeapon()
{
	if (CombatComponent == nullptr) return;
	if (HasAuthority())
	{
		CombatComponent->SwapWeapon(1);
	}
	else
	{
		ServerWeaponSwapButtonPressed(1);
	}
}

void AMainCharacter::SelectTertiaryWeapon()
{
	
}

void AMainCharacter::InventoryKeyPressed()
{
	if (InventoryWidgetClass)
	{
		if (InventoryWidget)
		{
			InventoryWidget->RemoveFromParent();
			InventoryWidget = nullptr;
		}
		else
		{
			InventoryWidget = Cast<UInventory>(CreateWidget(GetWorld(), InventoryWidgetClass));
			if (InventoryWidget)
			{
				InventoryWidget->InventoryGrid->DisplayInventory(InventoryComponent);
				InventoryWidget->InventoryWeaponInfo->DisplayWeaponInfo(InventoryComponent);
				InventoryWidget->AroundItemGrid->DisplayOverlappedItems(InventoryComponent);
				InventoryWidget->AddToViewport();
			}
		}
	}
}

void AMainCharacter::ServerInventoryWidget_Implementation()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
	}
	else
	{
		InventoryWidget = Cast<UInventory>(CreateWidget(GetWorld(), InventoryWidgetClass));
		if (InventoryWidget)
		{
			InventoryWidget->InventoryGrid->DisplayInventory(InventoryComponent);
			InventoryWidget->InventoryWeaponInfo->DisplayWeaponInfo(InventoryComponent);
			InventoryWidget->AroundItemGrid->DisplayOverlappedItems(InventoryComponent);
			InventoryWidget->AddToViewport();
		}
	}
}

void AMainCharacter::OnRep_InventoryWidget(UInventory* PostInventoryWidget)
{
	if (PostInventoryWidget)
	{
		PostInventoryWidget->RemoveFromParent();
	}
	if (InventoryWidget && InventoryWidget->InventoryGrid && InventoryComponent)
	{
		InventoryWidget->InventoryGrid->DisplayInventory(InventoryComponent);
		InventoryWidget->AddToViewport();
	}
}

float AMainCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void AMainCharacter::AimOffset(float DeltaTime)
{
	if (CombatComponent && CombatComponent->EquippedWeapon == nullptr) return;
	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir)	// not jump & stand
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;

		TurnInPlace(DeltaTime);
	}

	if (Speed > 0.f || bIsInAir) // run & jump
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;

		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void AMainCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void AMainCharacter::SimProxiesTurn()
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;

	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	bRotateRootBone = false;
	CalculateAO_Pitch();
	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void AMainCharacter::Jump()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void AMainCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHUDHealth();
	PlayHitReactMontage();

	if (Health == 0.f)
	{
		AMainGameMode* MainGameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();
		if (MainGameMode)
		{
			MainPlayerController = MainPlayerController == nullptr ? Cast<AMainPlayerController>(Controller) : MainPlayerController;
			AMainPlayerController* AttackerController = Cast<AMainPlayerController>(InstigatorController);
			MainGameMode->PlayerEliminated(this, MainPlayerController, AttackerController);
		}
	}
}

void AMainCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	PlayHitReactMontage();
}

void AMainCharacter::UpdateHUDHealth()
{
	MainPlayerController = MainPlayerController == nullptr ? Cast<AMainPlayerController>(Controller) : MainPlayerController;
	if (MainPlayerController)
	{
		MainPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void AMainCharacter::PollInit()
{
	if (MainPlayerState == nullptr)
	{
		MainPlayerState = GetPlayerState<AMainPlayerState>();
		if (MainPlayerState)
		{
			MainPlayerState->AddToScore(0.f);
			MainPlayerState->AddToDefeats(0);
		}
	}
}

UCustomizingWidget* AMainCharacter::CreateCustomizingWidget()
{
	if (CustomizingWidgetClass)
	{
		CustomizingWidget = Cast<UCustomizingWidget>(CreateWidget(GetWorld(), CustomizingWidgetClass));
		if (CustomizingWidget)
		{
			CustomizingWidget->AddToViewport();
			CustomizingWidget->SetCharacter(this);
			return CustomizingWidget;
		}
	}
	return CustomizingWidget;
}

void AMainCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void AMainCharacter::OnMontageNotifyBegin()
{
	MultiCastMontageMotify();
}

void AMainCharacter::MultiCastMontageMotify_Implementation()
{
	if (GetCurrentMontage() == ElimMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Pause();
	}
}

void AMainCharacter::HideCameraIfCharacterClose()
{
	if (!IsLocallyControlled()) return;
	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Opacity"), 0.3f);
		if (CombatComponent && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponMesh())
		{
			CombatComponent->EquippedWeapon->GetWeaponMesh()->SetScalarParameterValueOnMaterials(FName("Opacity"), 0.4f);
		}
	}
	else
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Opacity"), 1.f);
		if (CombatComponent && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponMesh())
		{
			CombatComponent->EquippedWeapon->GetWeaponMesh()->SetScalarParameterValueOnMaterials(FName("Opacity"), 1.f);
		}
	}
}

void AMainCharacter::ServerEquipButtonPressed_Implementation()
{
	if (CombatComponent)
	{
		AWeapon* WeaponToEquip = Cast<AWeapon>(OverlappingItem);
		// weapon
		if (WeaponToEquip)
		{
			CombatComponent->EquipWeapon(WeaponToEquip);
		}
		// item
		else if (OverlappingItem && OverlappingItem->GetItemDataComponent())
		{
			OverlappingItem->GetItemDataComponent()->Interact(this);
		}
		OverlappingItems.Num() == 0 ? OverlappingItem = nullptr : OverlappingItem = OverlappingItems[0];
	}
}

void AMainCharacter::ServerWeaponSwapButtonPressed_Implementation(int32 WeaponNum)
{
	if (CombatComponent)
	{
		CombatComponent->SwapWeapon(WeaponNum);
	}
}

void AMainCharacter::SetOverlappingItem(AItem* Item)
{
	if (OverlappingItem)
	{
		OverlappingItem->ShowPickupWidget(false);
	}
	OverlappingItem = Item;
	// for server
	if (IsLocallyControlled())
	{
		if (OverlappingItem)
		{
			OverlappingItem->ShowPickupWidget(true);
		}
	}
}

void AMainCharacter::OnRep_OverlappingItem(AItem* LastItem)
{
	if (OverlappingItem)
	{
		OverlappingItem->ShowPickupWidget(true);
	}
	if (LastItem)
	{
		LastItem->ShowPickupWidget(false);
	}
}

bool AMainCharacter::IsWeaponEquipped()
{
	return (CombatComponent && CombatComponent->EquippedWeapon);
}

bool AMainCharacter::IsAiming()
{
	return (CombatComponent && CombatComponent->bAiming);
}

AWeapon* AMainCharacter::GetEquippedWeapon()
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->EquippedWeapon;
}

FVector AMainCharacter::GetHitTarget() const
{
	if (CombatComponent == nullptr) return FVector();
	return CombatComponent->HitTarget;
}

ECombatState AMainCharacter::GetCombatState() const
{
	if (CombatComponent == nullptr) return ECombatState::ECS_MAX;
	return CombatComponent->CombatState;
}

AWeapon* AMainCharacter::GetEquippedWeapon() const
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->EquippedWeapon;
}

AWeapon* AMainCharacter::GetSecondaryWeapon() const
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->SecondaryWeapon;
}

AWeapon* AMainCharacter::GetWeapon1()
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->Weapon1;
}

AWeapon* AMainCharacter::GetWeapon2()
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->Weapon2;
}

int32 AMainCharacter::GetCarriedAmmo(AWeapon* Weapon)
{
	if (CombatComponent->CarriedAmmoMap.Contains(Weapon->GetWeaponType()))
	{
		return CombatComponent->CarriedAmmoMap[Weapon->GetWeaponType()];
	}
	return -1;
}

FVector AMainCharacter::GetGroundLocation()
{
	return GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}
