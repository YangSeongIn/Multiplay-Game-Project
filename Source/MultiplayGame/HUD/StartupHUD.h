// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartupHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYGAME_API AStartupHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MenuOverlayClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMenu* MenuOverlay;

public:
	void AddMenuOvelay();

};
