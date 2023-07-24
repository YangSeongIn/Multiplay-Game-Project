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

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshOnHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshOnHand"));
	SkeletalMeshOnHand->SetupAttachment(SkeletalMeshComponent, FName("RightHandSocket"));

	SkeletalMeshOnBack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshOnBack"));
	SkeletalMeshOnBack->SetupAttachment(SkeletalMeshComponent, FName("WeaponSocket"));
}

void ACharacterMeshCapture::BeginPlay()
{
	Super::BeginPlay();
	
	SceneCaptureComponent2D->ShowOnlyComponent(SkeletalMeshComponent);
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

