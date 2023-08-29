// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMeshCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "../HUD/Inventory.h"
#include "Components/Image.h"

ACharacterMeshCapture::ACharacterMeshCapture()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshPlaceHolder = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPlaceHolder"));
	MeshPlaceHolder->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(MeshPlaceHolder);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(SpringArmComponent);

	UpperBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBodyMesh"));
	UpperBodyMesh->SetupAttachment(RootComponent);

	LowerBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerBodyMesh"));
	LowerBodyMesh->SetupAttachment(UpperBodyMesh);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(UpperBodyMesh);

	GoggleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GoggleMesh"));
	GoggleMesh->SetupAttachment(UpperBodyMesh);

	BeardMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeardMesh"));
	BeardMesh->SetupAttachment(UpperBodyMesh);

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(UpperBodyMesh);

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(UpperBodyMesh);

	SkeletalMeshOnHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshOnHand"));
	SkeletalMeshOnHand->SetupAttachment(UpperBodyMesh, FName("RightHandSocket"));

	SkeletalMeshOnBack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshOnBack"));
	SkeletalMeshOnBack->SetupAttachment(UpperBodyMesh, FName("WeaponSocket"));
}

void ACharacterMeshCapture::Client_ApplyCustomizingInfo_Implementation(const TArray<USkeletalMesh*>& Meshes)
{
	HairMesh->SetSkeletalMesh(Meshes[0]);
	GoggleMesh->SetSkeletalMesh(Meshes[1]);
	BeardMesh->SetSkeletalMesh(Meshes[2]);
	UpperBodyMesh->SetSkeletalMesh(Meshes[3]);
	LowerBodyMesh->SetSkeletalMesh(Meshes[4]);
}

void ACharacterMeshCapture::BeginPlay()
{
	Super::BeginPlay();
	
	SceneCaptureComponent2D->ShowOnlyComponent(UpperBodyMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(SkeletalMeshOnHand);
	SceneCaptureComponent2D->ShowOnlyComponent(SkeletalMeshOnBack);
}

void ACharacterMeshCapture::SetCaptureTexture(int32 n)
{
	if (ActTextures.Num() > n)
	{
		SceneCaptureComponent2D->TextureTarget = ActTextures[n];
	}
}

void ACharacterMeshCapture::SetCaptureInventoryImage(UInventory* Inventory, int32 n)
{
	if (Inventory && InvTextures.Num() > n)
	{
		Inventory->SetMeshCaptureImage(InvTextures[n]);
	}
}



void ACharacterMeshCapture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

