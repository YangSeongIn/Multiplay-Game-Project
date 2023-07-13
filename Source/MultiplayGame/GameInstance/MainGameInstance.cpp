// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../CharacterMeshCapture/CharacterMeshCapture.h"
#include "Net/UnrealNetwork.h"

void UMainGameInstance::OnStart()
{
	Super::OnStart();
	/*UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterMeshCapture::StaticClass(), MeshCaptures);
	UE_LOG(LogTemp, Warning, TEXT("Instance MeshCaptures Num : %d"), MeshCaptures.Num())*/
	MulticastOnStart();
}

void UMainGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UMainGameInstance, MeshCaptures, COND_OwnerOnly);
}

void UMainGameInstance::MulticastOnStart_Implementation()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterMeshCapture::StaticClass(), MeshCaptures);
	UE_LOG(LogTemp, Warning, TEXT("Instance MeshCaptures Num : %d"), MeshCaptures.Num())
}

AActor* UMainGameInstance::GetMeshCapture(int32 n)
{
	UE_LOG(LogTemp, Warning, TEXT("Instance MeshCaptures Num 22 : %d"), MeshCaptures.Num());
	if (MeshCaptures.Num() <= n) return nullptr;
	return MeshCaptures[n];
}

bool UMainGameInstance::CanAddPlayerNum()
{
	if (PlayerNum + 1 < MeshCaptures.Num()) return true;
	return false;
}