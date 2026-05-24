// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "IDetailTreeNode.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HoverItem.generated.h"

class UInv_InventoryItem;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(const int32 Count) const;

	FGameplayTag GetItemType() const;
	void SetPreviousGridIndex(int32 Index) { PreviousGridIndex = Index;}
	int32 GetPreviousGridIndex() const { return PreviousGridIndex;}
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions;}
	FIntPoint GetGridDimensions() const { return GridDimensions;}
	void SetStackCount(const int32 Count) { StackCount = Count;}
	int32 GetStackCount() const { return StackCount;}
	bool IsStackable() const { return bIsStackable;}
	void SetIsStackable(bool bStacks);
	UInv_InventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UInv_InventoryItem* Item);
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 PreviousGridIndex;
	FIntPoint GridDimensions;
	int32 StackCount;
	bool bIsStackable{false};
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
};
