#include "Items/Manifest/Inv_ItemManifest.h"

#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter)
{
	UInv_InventoryItem* InventoryItem = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass());
	//for the Item actor will be destroyed when it be picked up,it's manifest have to save in InventoryItem
	InventoryItem->SetItemManifest(*this);

	return InventoryItem;
}
