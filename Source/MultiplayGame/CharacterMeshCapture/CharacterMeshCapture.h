// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterMeshCapture.generated.h"

UCLASS()
class MULTIPLAYGAME_API ACharacterMeshCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	ACharacterMeshCapture();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* MeshPlaceHolder;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
		class USceneCaptureComponent2D* SceneCaptureComponent2D;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMeshComponent;

public:
	FORCEINLINE void SetSkeletaMesh(USkeletalMesh* NewMesh) { SkeletalMeshComponent->SetSkeletalMesh(NewMesh); };

};
