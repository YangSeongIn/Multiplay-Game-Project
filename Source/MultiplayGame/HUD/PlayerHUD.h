// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UTexture2D* CrosshairCenter;
	UTexture2D* CrosshairLeft;
	UTexture2D* CrosshairRight;
	UTexture2D* CrosshairBottom;
	UTexture2D* CrosshairTop;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};

UCLASS()
class MULTIPLAYGAME_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY()
		class UCharacterOverlay* CharacterOverlay;

	void AddCharacterOvelay();

	UPROPERTY(EditAnywhere, Category = "Announcements")
		TSubclassOf<class UUserWidget> AnnouncementClass;

	UPROPERTY()
		class UAnnouncement* Announcement;

	void AddAnnouncement();

protected:
	virtual void BeginPlay() override;
	

private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
		float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; };

};
