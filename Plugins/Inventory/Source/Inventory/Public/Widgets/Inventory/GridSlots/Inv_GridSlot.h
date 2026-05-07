// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;

UENUM(BlueprintType)
enum class EInv_GridSlotState  : uint8
{
	UnOccupied,
	Occupied,
	Selected,
	GrayedOut
};

UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTileIndex(int32 Index) {TileIndex = Index;}
	EInv_GridSlotState GetGridSlotState() const { return GridSlotState; }

	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Grid;

	int32 TileIndex;


	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_UnOccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayOut;

	EInv_GridSlotState GridSlotState;
};
