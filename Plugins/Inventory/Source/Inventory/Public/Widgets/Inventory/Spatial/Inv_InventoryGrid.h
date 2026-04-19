// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EInv_ItemCategory ItemCategory;
public:
	FORCEINLINE	EInv_ItemCategory GetItemCategory() const {return ItemCategory;}
};
