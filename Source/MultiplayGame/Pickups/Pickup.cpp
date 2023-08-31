// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "../Weapon/WeaponTypes.h"
#include "Net/UnrealNetwork.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetEnableGravity(true);
	PickupMesh->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);

	AreaSphere->SetupAttachment(RootComponent);

	/*OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(PickupMesh);
	OverlapSphere->SetSphereRadius(150.f);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);*/

}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (HasAuthority())
	{
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereOverlapBegin);
	}*/
}

void APickup::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void APickup::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComp,OtherActor, OtherComp, OtherBodyIndex);
}

void APickup::BindOverlapTimerFinished()
{
	//OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereOverlapBegin);
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::Destroyed()
{
	Super::Destroyed();

	
}

void APickup::SetActive(bool bIsActive, FVector LocationToSpawn)
{
	MulticastSetActive(bIsActive);
	if (bIsActive)
	{
		SetActorLocation(LocationToSpawn);
	}
}

void APickup::MulticastSetActive_Implementation(bool bIsActive)
{
	if (bIsActive)
	{
		PickupMesh->SetVisibility(true);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}
		PickupMesh->SetVisibility(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

