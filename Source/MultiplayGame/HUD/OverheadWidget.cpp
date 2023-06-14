// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	}
	FString LocalRoleString = FString::Printf(TEXT("Local Role: %s"), *Role);
	SetDisplayText(LocalRoleString);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	const APlayerState* PlayerState = InPawn->GetPlayerState();
	if (PlayerState == nullptr || !*PlayerState->GetPlayerName() && TotalTime < TimeLimit)
	{
		FTimerHandle GetPlayerStateTimer;
		FTimerDelegate RecursiveDelegate;
		RecursiveDelegate.BindUFunction(this, FName("ShowPlayerName"), InPawn);
		GetWorld()->GetTimerManager().SetTimer(GetPlayerStateTimer, RecursiveDelegate, TimeInterval, false, 0.1f);
		TotalTime = TimeInterval;
		return;
	}
	const FString PlayerName = PlayerState->GetPlayerName();
	SetDisplayText(PlayerName);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
