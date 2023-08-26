// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizingCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "../HUD/CustomizingWidget.h"
#include "Components/SkinnedMeshComponent.h"

ACustomizingCharacter::ACustomizingCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	UpperBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBodyMesh"));
	RootComponent = UpperBodyMesh;
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

	HeadMesh->SetLeaderPoseComponent(UpperBodyMesh);
	LowerBodyMesh->SetLeaderPoseComponent(UpperBodyMesh);
	BeardMesh->SetLeaderPoseComponent(UpperBodyMesh);
	HairMesh->SetLeaderPoseComponent(UpperBodyMesh);
	GoggleMesh->SetLeaderPoseComponent(UpperBodyMesh);
	HandMesh->SetLeaderPoseComponent(UpperBodyMesh);
}

UCustomizingWidget* ACustomizingCharacter::CreateCustomizingWidget()
{
	if (CustomizingWidgetClass)
	{
		CustomizingWidget = Cast<UCustomizingWidget>(CreateWidget(GetWorld(), CustomizingWidgetClass));
		if (CustomizingWidget)
		{
			CustomizingWidget->AddToViewport();
			CustomizingWidget->SetMainMesh(this);
		}
	}
	return CustomizingWidget;
}

void ACustomizingCharacter::RemoveCustomizingWidget()
{
	if (CustomizingWidget)
	{
		CustomizingWidget->RemoveFromParent();
		CustomizingWidget = nullptr;
	}
}
