// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragment/Inv_FragmentTag.h"
#include "Items/Fragment/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
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
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Availability.AmountToFill, Availability.Index, Result.bStackable);

		UpdateGridSlots(NewItem, Availability.Index);
	}
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* NewItem, int32 AmountToFill, int32 Index, bool bStackable)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(NewItem, FragmentTags::ImageFragment);

	if (!GridFragment || !ImageFragment) return;
	UInv_SlottedItem* SlottedItem = CreateSlottedItem(NewItem, bStackable , AmountToFill, GridFragment, ImageFragment, Index);

	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);
	//4. Store the new widget in a container
	SlottedItems.Add(Index, SlottedItem);
}

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index) const
{
	UInv_SlottedItem* SlottedItem = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
		
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	// Set SlottedItem's Inventory Item
	SlottedItem->SetInventoryItem(Item);
	SlottedItem->SetGridIndex(Index);
	return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
	UInv_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	// Set Item Position
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment,
	const FInv_ImageFragment* ImageFragment) const
{
	// Set SlottedItem's Image
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index)
{
	check(GridSlots.IsValidIndex(Index));

	UInv_GridSlot* GridSlot = GridSlots[Index];
	if (GridSlot)
	{
		GridSlot->SetOccupiedTexture();
	}
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

