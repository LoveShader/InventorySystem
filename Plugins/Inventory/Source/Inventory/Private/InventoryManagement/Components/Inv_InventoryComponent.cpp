// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Player/Inv_PlayerController.h"
#include "Widgets/Types/Inv_GridTypes.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (!bInventoryMenuOpen)
		OpenInventoryMenu();
	else
		CloseInventoryMenu();
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	if (!ItemComponent)
		return;

	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);
	if (Result.TotalAmountToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}

	if (Result.Item.IsValid() && Result.bStackable)
	{
		//Add stacks to an item that already exists in the inventory. We only  want to update the stackCount,
		//not create a new item of this type
		Server_AddStacksToItem(ItemComponent, Result.TotalAmountToFill, Result.Reminder);
	} else if (Result.TotalAmountToFill > 0)
	{
		// This item type doesn't exist in the inventory. Create a new one and update all pertinent slots
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalAmountToFill : 0);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
	InventoryList.AddEntry(ItemComponent);

	//TODO: Tell the Item Component to Destroy its owning Actor
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent,
	int32 StackCount, int32 Remainder)
{
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
		InventoryMenu->SetInventoryComponent(this);
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

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}
