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
		
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
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
	SlottedItem->SetIsStackable(bStackable);

	const int32 StackCount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackCount);
	
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
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment);

	const FIntPoint ItemRange = GridFragment != nullptr ? GridFragment->GetGridSize() : FIntPoint(1, 1);
	//it the object is Stackable, Set it's StackAmount
	if (bStackableItem)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}
	
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, ItemRange, Columns, [&NewItem, &Index](UInv_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetAvailable(false);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
	});
	
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
}

bool UInv_InventoryGrid::HasRoomAtIndex(const UInv_GridSlot* GridSlot,
										const FIntPoint& Dimensions,
										const TSet<int32>& CheckedIndices,
										TSet<int32>& OutTentativelyClaimed,
										const FGameplayTag& ItemType,
										const int32 MaxStackSize)
{
	// Is there room at this index? (i.e. are there other items in the way?)
	bool bHasRoom = true;
	UInv_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetTileIndex(), Dimensions, Columns, [&](const UInv_GridSlot* SubGridSlot)
	{
		// Check any other important conditions - ForEach2D over a 2D range
		if (CheckSlotConstrains(GridSlot, SubGridSlot, CheckedIndices, ItemType, MaxStackSize))
		{
			//when find the grid slot is not used, set it's Claimed.
			// if other grid slot has another item or has been claimed, set bHasRoom false
			// so it will not add to CheckedIndices
			OutTentativelyClaimed.Add(SubGridSlot->GetTileIndex());
		}
		else
		{
			bHasRoom = false;
		}
	});

	return bHasRoom;
}

bool UInv_InventoryGrid::CheckSlotConstrains(const UInv_GridSlot* GridSlot ,
										     const UInv_GridSlot* SubGridSlot,
										     const TSet<int32>& CheckedIndices,
										     const FGameplayTag& ItemType,
										     const int32 MaxStackSize)
{
	// Index Claimed?
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetTileIndex()))
		return false;

	// Has valid item?
	if (!HasValidItem(SubGridSlot))
		return true;
	// if item upper left index equal the index that need to check
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot))
		return false;
	// If so, is this a stackable item?
	UInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable())
		return false;

	// If this item the same type as the item we're trying to add?
	if (!DoesItemTypeMatch(SubItem, ItemType))
		return false;
	// If stackable, is this slot at the max stack size already?
	if (GridSlot->GetStackCount() >= MaxStackSize)
		return false;
	
	return true;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
	const FInv_GridFragment* GridFragment = Manifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UInv_InventoryGrid::HasValidItem(const UInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGrid::IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const
{
	return GridSlot->GetTileIndex() == SubGridSlot->GetUpperLeftIndex();
}

bool UInv_InventoryGrid::DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
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
	// Determine if the item is stackable
	const FInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	// Determine how many stacks to add.
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;

	TSet<int32> CheckedIndices;
	// For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		// If we don't have anymore to fill, break out of the loop early.
		if (AmountToFill == 0)
			break;
		// Is the index claimed yet?
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetTileIndex()))
			continue;
		// Can the item fit here? (i.e. is it out of grid bounds?)
		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativelyClaimed, Manifest.GetItemType(), MaxStackSize))
		{
			continue;
		}
		CheckedIndices.Append(TentativelyClaimed);
		
		// How much to fill?
		// Update the amount left to fill
	}
	
	// How much is the Remainder?
	
	return Result;
}

