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

	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<class UTextureRenderTarget2D>> ActTextures;

	UPROPERTY(EditAnywhere)
		TArray<class UMaterialInterface*> InvTextures;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMeshOnHand;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMeshOnBack;

public:
	FORCEINLINE void SetSkeletaMesh(USkeletalMesh* NewMesh) { SkeletalMeshComponent->SetSkeletalMesh(NewMesh); };
	FORCEINLINE USkeletalMeshComponent* GetSkeletaMesh() { return SkeletalMeshComponent; };
	void SetCaptureTexture(int32 n);
	void SetCaptureInventoryImage(class UInventory* Inventory, int32 n);
	FORCEINLINE void SetSkeletalMeshOnHand(USkeletalMesh* MeshToSet) { SkeletalMeshOnHand->SetSkeletalMesh(MeshToSet); };
	FORCEINLINE void SetSkeletalMeshOnBack(USkeletalMesh* MeshToSet) { SkeletalMeshOnBack->SetSkeletalMesh(MeshToSet); };
};
