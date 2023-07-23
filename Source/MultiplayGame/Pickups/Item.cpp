// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/MainCharacter.h"
#include "../MainCharacterComponent/InventoryComponent.h"
#include "../MainCharacterComponent/ItemDataComponent.h"
#include "Components/WidgetComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		// AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapBegin);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);
	}

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ItemDataComponent)
	{
		ItemDataComponent->OwningActor = Cast<AActor>(this);
	}
}

void AItem::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character && Character->GetInventoryComponent())
	{
		UInventoryComponent* InvComp = Character->GetInventoryComponent();
		UItemDataComponent* DataComp = GetItemDataComponent();
		InvComp->AddOverlappedItem(DataComp->GetItemID().RowName.ToString(), DataComp->GetQuantity(), DataComp->GetItemType(), GetName(), this);
		if (InvComp->OnOverlappedItemUpdate.IsBound())
		{
			InvComp->OnOverlappedItemUpdate.Broadcast();
		}
		Character->SetOverlappingItem(this);
	}
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character && Character->GetInventoryComponent())
	{
		Character->GetInventoryComponent()->RemoveOverlappedItem(GetName());
		UInventoryComponent* InvComp = Character->GetInventoryComponent();
		if (InvComp->OnOverlappedItemUpdate.IsBound())
		{
			InvComp->OnOverlappedItemUpdate.Broadcast();
		}
		Character->SetOverlappingItem(nullptr);
	}
}

void AItem::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}
