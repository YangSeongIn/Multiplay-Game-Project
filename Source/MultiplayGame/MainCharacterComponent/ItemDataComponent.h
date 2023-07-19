// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Types/InventorySlotStruct.h"
#include "ItemDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYGAME_API UItemDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemDataComponent();
	friend class AItem;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact(class AMainCharacter* MainCharacter);

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
		int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FDataTableRowHandle ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		EItemType ItemType;

	UPROPERTY()
		AActor* OwningActor;

public:
	FORCEINLINE FDataTableRowHandle GetItemID() { return ItemID; };
	FORCEINLINE int32 GetQuantity() { return Quantity; };
	FORCEINLINE EItemType GetItemType() { return ItemType; };
};
