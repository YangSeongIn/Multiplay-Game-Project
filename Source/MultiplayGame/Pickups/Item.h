// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class MULTIPLAYGAME_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();
	UFUNCTION()
	virtual void OnSphereOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ShowPickupWidget(bool bShowWidget);
	UFUNCTION(Client, Reliable)
	void ServerBroadcast(class UInventoryComponent* InvComp);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaSphere;

private:
	UPROPERTY(EditAnywhere)
	class UItemDataComponent* ItemDataComponent;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* PickupWidget;

public:
	FORCEINLINE UItemDataComponent* GetItemDataComponent() { return ItemDataComponent; };
};
