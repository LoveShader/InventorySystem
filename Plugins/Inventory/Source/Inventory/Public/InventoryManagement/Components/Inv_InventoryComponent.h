// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"

struct FInv_InventoryFastArray;
class UInv_ItemComponent;
class UInv_InventoryBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	void ToggleInventoryMenu();
	void TryAddItem(UInv_ItemComponent* ItemComp);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventoryMenu();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void AddRepSubObj(UObject* SubObj);
	

	FInventoryItemChange OnItemAdd;
	FInventoryItemChange OnItemRemove;
	FNoRoomInInventory NoRoomInInventory;
protected:
	virtual void BeginPlay() override;
private:
	void ConstructInventory();
	void OpenInventoryMenu();
private:
	TWeakObjectPtr<APlayerController> OwningController;
	
	UPROPERTY()
	TObjectPtr<UInv_InventoryBase> InventoryMenu;
	
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;
	
	bool bInventoryMenuOpen = false;

	/** FastArray for Inventory Item */
	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryList;
};
