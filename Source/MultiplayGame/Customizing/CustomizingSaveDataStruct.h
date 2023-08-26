// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomizingSaveDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FCustomizingSaveDataStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HairIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoggleIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BeardIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpperBodyIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LowerBodyIndex;
};