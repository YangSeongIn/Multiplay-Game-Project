// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizingSlot.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UCustomizingSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlotClicked(bool bIsClicked);

protected:
	virtual void NativeConstruct() override;

private:
	FString SlotID;
	int32 Index;

	UPROPERTY()
	class UCustomizingWidget* CustomizingWidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(meta = (BindWidget))
	class UImage* Thumbnail;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NoneText;

	UPROPERTY(meta = (BindWidget))
	class UBorder* OutBorder;

	UPROPERTY()
	class USkeletalMesh* Mesh;

	UFUNCTION()
	void OnClickedButton();

public:
	void SetImage(class UTexture2D* ImageToSet);
	FORCEINLINE void SetCustomMesh(class USkeletalMesh* MeshToSet) { Mesh = MeshToSet; };
	FORCEINLINE void SetCustomizingWidget(class UCustomizingWidget* CustomizingWidgetToSet) { CustomizingWidget = CustomizingWidgetToSet; };
	FORCEINLINE void SetSlotID(FString IDToSet) { SlotID = IDToSet; };
	FORCEINLINE void SetIndex(int32 IndexToSet) { Index = IndexToSet; };
};
