// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomizingCharacter.generated.h"

UCLASS()
class MULTIPLAYGAME_API ACustomizingCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomizingCharacter();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CustomizingWidgetClass;

	UPROPERTY()
	class UCustomizingWidget* CustomizingWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* UpperBodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* HeadMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* LowerBodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* BeardMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* GoggleMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh")
	USkeletalMeshComponent* HandMesh;

	UFUNCTION(BlueprintCallable)
	void RemoveCustomizingWidget();
};
