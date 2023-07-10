// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	
	
	

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	FString ItemID;
	UPROPERTY(EditAnywhere)
		class UDataTable* InventoryDataTable;
	UPROPERTY(BlueprintReadOnly, Category = "Drag Item Visual", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UImage* ItemIcon;

public:
	FORCEINLINE void SetItemIcon(class UImage* NewIcon) { ItemIcon = NewIcon; };
	FORCEINLINE void SetItemID(FString NewID) { ItemID = NewID; };
};
