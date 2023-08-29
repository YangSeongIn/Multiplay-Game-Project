// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "CharacterMeshCapture.generated.h"

UCLASS()
class MULTIPLAYGAME_API ACharacterMeshCapture : public AActor
{
	GENERATED_BODY()

public:
	ACharacterMeshCapture();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshOnHand;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshOnBack;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* UpperBodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HeadMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LowerBodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BeardMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GoggleMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HandMesh;


	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ApplyCustomizingInfo(const TArray<USkeletalMesh*>& Meshes);

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

public:
	void SetCaptureTexture(int32 n);
	void SetCaptureInventoryImage(class UInventory* Inventory, int32 n);
	FORCEINLINE void SetSkeletalMeshOnHand(USkeletalMesh* MeshToSet) { SkeletalMeshOnHand->SetSkeletalMesh(MeshToSet); };
	FORCEINLINE void SetSkeletalMeshOnBack(USkeletalMesh* MeshToSet) { SkeletalMeshOnBack->SetSkeletalMesh(MeshToSet); };
};
