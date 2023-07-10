// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMeshCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

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
}

void ACharacterMeshCapture::BeginPlay()
{
	Super::BeginPlay();
	
	SceneCaptureComponent2D->ShowOnlyComponent(SkeletalMeshComponent);
}

void ACharacterMeshCapture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

