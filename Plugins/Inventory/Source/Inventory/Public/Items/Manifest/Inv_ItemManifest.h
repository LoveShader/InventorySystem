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

	template<typename T>
	requires(std::derived_from<T, FInv_ItemFragment>)
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const;

	template<typename T>
	requires(std::derived_from<T, FInv_ItemFragment>)
	const T* GetFragmentOfType() const;
private:
	//Declare the item's type
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
};

template<typename T>
	requires(std::derived_from<T, FInv_ItemFragment>)
	const T* FInv_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& Tag) const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(Tag))
				continue;
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires (std::derived_from<T, FInv_ItemFragment>)
const T* FInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}
