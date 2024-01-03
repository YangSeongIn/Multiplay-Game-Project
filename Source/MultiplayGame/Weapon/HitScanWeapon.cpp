// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSokcet = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSokcet)
	{
		FTransform SocketTranform = MuzzleFlashSokcet->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTranform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		UWorld* World = GetWorld();
		if (World)
		{
			World->LineTraceSingleByChannel(
				FireHit,
				Start,
				End,
				ECollisionChannel::ECC_Visibility
			);
			FVector BeamEnd = End;
			if (FireHit.bBlockingHit)
			{
				BeamEnd = FireHit.ImpactPoint;
				AMainCharacter* Character = Cast<AMainCharacter>(FireHit.GetActor());
				if (Character && HasAuthority() && InstigatorController)
				{
					UGameplayStatics::ApplyDamage(
						Character,
						Damage,
						InstigatorController,
						this,
						UDamageType::StaticClass()
					);
				}
				MulticastSpawnHitImpact(FireHit);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint);
			}
			if (BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, SocketTranform);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}
		}
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SocketTranform);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void AHitScanWeapon::MulticastSpawnHitImpact_Implementation(const FHitResult HitResult)
{
		switch (UGameplayStatics::GetSurfaceType(HitResult))
		{
		case SurfaceType1:
			SetBulletMarks(ImpactParticlesMetal, ImpactSoundMetal, HitResult);
			break;
		case SurfaceType2:
			break;
		case SurfaceType3:
			break;
		case SurfaceType4:
			SetBulletMarks(ImpactParticlesBody, ImpactSoundBody, HitResult);
			break;
		default:
			SetBulletMarks(ImpactParticlesBody, ImpactSoundBody, HitResult);
			break;
		}
}

void AHitScanWeapon::SetBulletMarks(UParticleSystem* Particle, USoundCue* SoundCue, const FHitResult HitResult)
{
	if (Particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	}
	if (SoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, HitResult.ImpactPoint);
	}
}
