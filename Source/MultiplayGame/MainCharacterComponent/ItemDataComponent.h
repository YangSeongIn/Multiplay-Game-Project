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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact(class AMainCharacter* MainCharacter);

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
		int Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FDataTableRowHandle ItemID;

public:
	FORCEINLINE FDataTableRowHandle GetItemID() { return ItemID; };

};
