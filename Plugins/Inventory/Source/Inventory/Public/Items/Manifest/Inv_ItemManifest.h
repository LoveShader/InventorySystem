#pragma once

#include "Widgets/Types/Inv_GridTypes.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemManifest.generated.h"

struct FInv_ItemFragment;
class UInv_InventoryItem;

USTRUCT()
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
	UInv_InventoryItem* Manifest(UObject* NewOuter);
	
private:
	//Declare the item's type
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
};