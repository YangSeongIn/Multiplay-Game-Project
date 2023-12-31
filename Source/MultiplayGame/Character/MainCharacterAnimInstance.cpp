// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Weapon/Weapon.h"
#include "DrawDebugHelpers.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}
	if (MainCharacter == nullptr) return;

	FVector Velocity = MainCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = MainCharacter->IsWeaponEquipped();
	EquippedWeapon = MainCharacter->GetEquippedWeapon();
	bIsCrouched = MainCharacter->bIsCrouched;
	bAiming = MainCharacter->IsAiming();
	TurningInPlace = MainCharacter->GetTurningInPlace();
	bRotateRootBone = MainCharacter->ShouldRotateRootBone();
	bElimmed = MainCharacter->IsElimmed();

	// Offset Yaw for Strafing
	FRotator AimRotation = MainCharacter->GetBaseAimRotation(); // world is standard. not character.
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 5.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = MainCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -180.f, 180.f);

	AO_Yaw = MainCharacter->GetAO_Yaw();
	AO_Pitch = MainCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && MainCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		MainCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (MainCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - MainCharacter->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 20.f);
		}	
	}

	bUseFABRIK = MainCharacter->GetCombatState() != ECombatState::ECS_Reloading;
	bUseAimOffsets = MainCharacter->GetCombatState() != ECombatState::ECS_Reloading;
	bTransformRightHand = MainCharacter->GetCombatState() != ECombatState::ECS_Reloading;
}
