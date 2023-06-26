// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "../Character/MainCharacter.h"
#include "../MultiplayGame.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
	CollisionBox->bReturnMaterialOnMove = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp && Hit.bBlockingHit)
	{
		MulticastHitImpact(Hit);
	}

	Destroy();
}

void AProjectile::MulticastHitImpact_Implementation(const FHitResult HitResult)
{
	switch (UGameplayStatics::GetSurfaceType(HitResult))
	{
	case SurfaceType1:
		GLog->Log("Metal");
		SetBulletMarks(ImpactParticlesMetal, ImpactSoundMetal);
		break;
	case SurfaceType2:
		break;
	case SurfaceType3:
		break;
	case SurfaceType4:
		GLog->Log("Body");
		SetBulletMarks(ImpactParticlesBody, ImpactSoundBody);
		break;
	default:
		GLog->Log("Default");
		SetBulletMarks(ImpactParticlesMetal, ImpactSoundMetal);
		break;
	}
}


void AProjectile::SetBulletMarks(UParticleSystem* Particle, USoundCue* SoundCue)
{
	if (Particle)
	{
		FinalImpactParticles = Particle;
	}
	if (SoundCue)
	{
		FinalImpactSound = SoundCue;
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (FinalImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FinalImpactParticles, GetActorTransform());
	}
	if (FinalImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FinalImpactSound, GetActorLocation());
	}
}

