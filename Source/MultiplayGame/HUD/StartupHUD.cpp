// Fill out your copyright notice in the Description page of Project Settings.


#include "StartupHUD.h"
#include "MultiplayerSessions/public/Menu.h"

void AStartupHUD::AddMenuOvelay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && MenuOverlayClass)
	{
		MenuOverlay = CreateWidget<UMenu>(PlayerController, MenuOverlayClass);
		if (MenuOverlay)
		{
			MenuOverlay->AddToViewport();
			MenuOverlay->MenuSetup();
		}
	}
}