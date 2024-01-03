// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMeshCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "../HUD/Inventory.h"
#include "Components/Image.h"
#include "../Character/MainCharacter.h"
#include "../GameInstance/MainGameInstance.h"
#include "../Data/CostomizingDataAsset.h"

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

void ACharacterMeshCapture::UpdateMeshCapture(AMainCharacter* MainCharacter, FCustomizingSaveDataStruct CustomizingSaveData)
{
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	HairMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->HairMeshes[CustomizingSaveData.HairIndex].Mesh);
	GoggleMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->GoggleMeshes[CustomizingSaveData.GoggleIndex].Mesh);
	BeardMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->BeardsMeshes[CustomizingSaveData.BeardIndex].Mesh);
	UpperBodyMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->UpperBodyMeshes[CustomizingSaveData.UpperBodyIndex].Mesh);
	LowerBodyMesh->SetSkeletalMesh(GameInstance->GetCustomizingDataAsset()->LowerBodyMeshes[CustomizingSaveData.LowerBodyIndex].Mesh);

	MainCharacter->UpperBodyMesh->SetMaterial(0, UpperBodyMesh->GetMaterial(0));
}

void ACharacterMeshCapture::BeginPlay()
{
	Super::BeginPlay();
	
	SceneCaptureComponent2D->ShowOnlyComponent(UpperBodyMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(GoggleMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(BeardMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(HairMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(LowerBodyMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(SkeletalMeshOnHand);
	SceneCaptureComponent2D->ShowOnlyComponent(SkeletalMeshOnBack);
	SceneCaptureComponent2D->ShowOnlyComponent(HandMesh);
	SceneCaptureComponent2D->ShowOnlyComponent(HeadMesh);
}

void ACharacterMeshCapture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
