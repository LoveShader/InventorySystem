// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


class UInv_GridSlot;
class UCanvasPanel;

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
private:
	void ConstructGrids();
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
public:
	FORCEINLINE	EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
};
