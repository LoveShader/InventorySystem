// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


struct FInv_ImageFragment;
struct FInv_GridFragment;
class UInv_SlottedItem;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UInv_GridSlot;
class UCanvasPanel;

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent);
private:
	void ConstructGrids();

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	bool MatchesCategory(const UInv_InventoryItem* Item) const;

	FInv_SlotAvailabilityResult HasRoomForItem(UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);
	void AddItemAtIndex(UInv_InventoryItem* NewItem, int32 AmountToFill, int32 Index, bool bStackable);
	UInv_SlottedItem* CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index) const;
	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem) const;
	
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const;
	void UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount);
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;
	bool HasRoomAtIndex(const UInv_GridSlot* GridSlot,
		const FIntPoint& Dimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed);
	bool CheckSlotConstrains(const UInv_GridSlot* GridSlot, const TSet<int32>& CheckedIndices);
	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const;
	bool HasValidItem(const UInv_GridSlot* SubGridSlot) const;
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EInv_ItemCategory ItemCategory;
	/*
	 * Grid Widget
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditAnywhere, Category = GridSlots)
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = GridSlots)
	int Rows;

	UPROPERTY(EditAnywhere, Category = GridSlots)
	int Columns;

	UPROPERTY(EditAnywhere, Category = GridSlots)
	int TileSize;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	/** Slotted Item */
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInv_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItem>> SlottedItems;
public:
	FORCEINLINE	EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
};

