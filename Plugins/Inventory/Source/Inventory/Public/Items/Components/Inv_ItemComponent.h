// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();
protected:

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FString PickupMessage;

	UPROPERTY(EditAnywhere, Category = Inventory)
	FInv_ItemManifest InventoryManifest;
public:
 	FORCEINLINE FString GetPickupMessage() const { return PickupMessage; }
	FORCEINLINE FInv_ItemManifest GetInventoryManifest() const { return InventoryManifest; }
};
