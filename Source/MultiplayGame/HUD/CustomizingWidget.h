// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "../Customizing/CustomizingSaveDataStruct.h"
#include "../Customizing/CustomizingInfoStruct.h"
#include "CustomizingWidget.generated.h"

class UButton;
class UCostomizingDataAsset;

UCLASS()
class MULTIPLAYGAME_API UCustomizingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPartMesh(ECustomizingCategory Category, class USkeletalMesh* MeshToSet);
	void SetPartIndex(ECustomizingCategory Category, int32 Index);
	void HighlightingSlot(int32 TargetIdx);

	FCustomizingSaveDataStruct CustomizingSaveDataStruct;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SlotClass;
	UPROPERTY()
	class UCustomizingSlot* CustomizingSlot;

	UPROPERTY()
	TArray<class USkeletalMesh*> HeadMeshes;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* Character;

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

	TArray<class UCustomizingSlot*> Slots;

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

	void SetMeshVisibility(USkeletalMesh* MeshToSet, USkeletalMeshComponent* TargetMesh);

	void SetCustomizingSlot(const TArray<FCustomizingInfoStruct>& InfoStructToCustomize, ECustomizingCategory Category, int32 BodyIndex);

public:
	FORCEINLINE TArray<class UCustomizingSlot*> GetSlots() { return Slots; };
	FORCEINLINE class AMainCharacter* GetCharacter() { return Character; };
	FORCEINLINE void SetCharacter(class AMainCharacter* CharacterToSet) { Character = CharacterToSet; };
};
