// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Player/Inv_PlayerController.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (!bInventoryMenuOpen)
		OpenInventoryMenu();
	else
		CloseInventoryMenu();
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
		CloseInventoryMenu();
	}
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu))	return;

	if (!OwningController.IsValid() || !OwningController->IsLocalController()) return;
	
	bInventoryMenuOpen = true;
	InventoryMenu->SetVisibility(ESlateVisibility::Visible);

	//Set Mouse Input Mode
	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu))	return;

	if (!OwningController.IsValid() || !OwningController->IsLocalController()) return;
	
	bInventoryMenuOpen = false;
	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);

	//Set Mouse Input Mode
	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}