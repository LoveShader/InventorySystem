// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


class UInv_HoverItem;
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
						TSet<int32>& OutTentativelyClaimed,
						const FGameplayTag& ItemType,
						const int32 MaxStackSize);
	bool CheckSlotConstrains(const UInv_GridSlot* GridSlot,
							 const UInv_GridSlot* SubGridSlot,
							 const TSet<int32>& CheckedIndices,
							 const FGameplayTag& ItemType,
							 const int32 MaxStackSize);
	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const;
	bool HasValidItem(const UInv_GridSlot* SubGridSlot) const;
	bool IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const;
	bool DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const;
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlot* GridSlot) const;
	int32 GetStackCount(const UInv_GridSlot* GridSlot) const;

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	bool IsLeftClick(const FPointerEvent& MouseEvent);
	bool IsRightClick(const FPointerEvent& MouseEvent);

	void Pickup(UInv_InventoryItem* InventoryItem, const int32 GridIndex);
	void AssignHoverItem(UInv_InventoryItem* InventoryItem);
	void AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);
	void RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex);
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

	/** Hover Item */
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInv_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem;
public:
	FORCEINLINE	EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
};

