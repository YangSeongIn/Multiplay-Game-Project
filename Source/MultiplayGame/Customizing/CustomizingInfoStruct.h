// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomizingInfoStruct.generated.h"

UENUM(BlueprintType)
enum class ECustomizingCategory : uint8
{
	ECC_Hair UMETA(Displayname = "Hair"),
	ECC_Goggle UMETA(Displayname = "Goggle"),
	ECC_Beard UMETA(Displayname = "Beard"),
	ECC_UpperBody UMETA(Displayname = "UpperBody"),
	ECC_LowerBody UMETA(Displayname = "LowerBody"),
};

USTRUCT(BlueprintType)
struct FCustomizingInfoStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECustomizingCategory Category;
};

//USTRUCT(BlueprintType)
//struct FCustomizingCategoryArrays
//{
//public:
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	class TArray<class USkeletalMesh*> Hairs;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	class TArray<class USkeletalMesh*> Goggles;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	class TArray<class USkeletalMesh*> Beards;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	class TArray<class USkeletalMesh*> UpperBodies;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	class TArray<class USkeletalMesh*> LowerBodies;
//};