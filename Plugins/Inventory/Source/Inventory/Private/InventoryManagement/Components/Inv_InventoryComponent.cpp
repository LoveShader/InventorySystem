// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Player/Inv_PlayerController.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<AInv_PlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Owning Player Controller is not Valid"));
	//Important!!!
	// if is not locally Controlled, just return
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(),InventoryMenuClass);
	if (IsValid(InventoryMenu))
	{
		InventoryMenu->AddToViewport();
	}
}


