// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "../Customizing/CustomizingInfoStruct.h"
#include "CustomizingWidget.generated.h"

class UButton;

UCLASS()
class MULTIPLAYGAME_API UCustomizingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPartMesh(FString SlotID, class USkeletalMesh* MeshToSet);
	void SetPartIndex(FString SlotID, int32 Index);
	void HighlightingSlot(int32 TargetIdx);
	/*int32 CurrentHair;
	int32 CurrentGoggle;
	int32 CurrentBeard;
	int32 CurrentUpperBody;
	int32 CurrentLowerBody;*/

	FCustomizingSaveDataStruct CustomizingSaveDataStruct;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ACustomizingCharacter* MainMesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SlotClass;
	UPROPERTY()
	class UCustomizingSlot* CustomizingSlot;

	UPROPERTY()
	TArray<class USkeletalMesh*> HeadMeshes;

	UPROPERTY(EditAnywhere)
	UDataTable* DataTable;

	/*UPROPERTY(EditAnywhere)
	TArray<FCustomizingInfotruct> Hairs;
	UPROPERTY(EditAnywhere)
	TArray<FCustomizingInfotruct> Goggles;
	UPROPERTY(EditAnywhere)
	TArray<FCustomizingInfotruct> Beards;
	UPROPERTY(EditAnywhere)
	TArray<FCustomizingInfotruct> UpperBodies;
	UPROPERTY(EditAnywhere)
	TArray<FCustomizingInfotruct> LowerBodies;*/
	UPROPERTY()
	class UMainGameInstance* GameInstance;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Hair;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Goggle;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Beard;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_UpperBody;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_LowerBody;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Back;

	UPROPERTY(meta=(BindWidget))
	class UWrapBox* Grid;

	UFUNCTION()
	void OnClicked_Btn_Hair();
	UFUNCTION()
	void OnClickedBtn_Goggle();
	UFUNCTION()
	void OnClicked_Btn_Beard();
	UFUNCTION()
	void OnClicked_Btn_UpperBody();
	UFUNCTION()
	void OnClicked_Btn_LowerBody();

	TArray<class UCustomizingSlot*> Slots;

	void SetMeshVisibility(USkeletalMesh* MeshToSet, USkeletalMeshComponent* TargetMesh);

public:
	FORCEINLINE void SetMainMesh(class ACustomizingCharacter* MeshToSet) { MainMesh = MeshToSet; };
	FORCEINLINE TArray<class UCustomizingSlot*> GetSlots() { return Slots; };
};
