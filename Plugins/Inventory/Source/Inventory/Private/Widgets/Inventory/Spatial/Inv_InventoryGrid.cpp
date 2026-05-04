// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrids();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnItemAdd.AddDynamic(this, &ThisClass::AddItem);
	}
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetInventoryManifest());
}

void UInv_InventoryGrid::ConstructGrids()
{
	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			//Create Grid by GridClass
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);
			const FIntPoint TilePosition = FIntPoint(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(i, j));

			//Set Grid slot's Position and Size
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetPosition(TilePosition * TileSize);
			GridCPS->SetSize(FVector2D(TileSize));
			//Add Grid Slot to GridSlots Array
			GridSlots.Add(GridSlot);
		}
	}
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item))
		return;

	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	AddItemToIndices(Result, Item);
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem)
{
	//1. Get Grid Fragment so we konw how many grid spaces the item takes
	//2. Get Image Fragment so we have the icon to display
	
	//3. Create a widget to add to the grid
	//4. Store the new widget in a container
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return ItemCategory == Item->GetItemManifest().GetItemCategory();
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
	FInv_SlotAvailabilityResult Result;
	Result.TotalAmountToFill = 1;

	//Test Purpose, Create SlotAvailablity
	FInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1;
	SlotAvailability.Index = 0;
	
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));
	
	return Result;
}

