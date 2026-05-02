// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

FInv_SlotAvailabilityResult UInv_InventoryBase::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
	FInv_SlotAvailabilityResult Result;
	Result.TotalAmountToFill = 1;
	return Result;
}
