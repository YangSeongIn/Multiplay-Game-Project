// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Customizing/CustomizingInfoStruct.h"

#include "CostomizingDataAsset.generated.h"

UCLASS()
class MULTIPLAYGAME_API UCostomizingDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hair", meta=(TitleProperty="MeshName"))
	TArray<FCustomizingInfoStruct> HairMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Goggle", meta = (TitleProperty = "MeshName"))
	TArray<FCustomizingInfoStruct> GoggleMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beards", meta = (TitleProperty = "MeshName"))
	TArray<FCustomizingInfoStruct> BeardsMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UpperBody", meta = (TitleProperty = "MeshName"))
	TArray<FCustomizingInfoStruct> UpperBodyMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LowerBody", meta = (TitleProperty = "MeshName"))
	TArray<FCustomizingInfoStruct> LowerBodyMeshes;
};
