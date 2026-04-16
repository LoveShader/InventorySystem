// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
class UInputMappingContext;
class UInputAction;
class UInv_InventoryComponent;

UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AInv_PlayerController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceWithItems();

	UFUNCTION(Blueprintable)
	void ToggleInventory();
private:
	UPROPERTY(EditAnywhere, Category = Inventory)
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TObjectPtr<UInputAction> PrimaryInteractAction;
	
	UPROPERTY(EditAnywhere, Category = Inventory)
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	/*
	 * Trace Items Parameters 
	 */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	double TraceLenght = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> LastActor;
	TWeakObjectPtr<AActor> ThisActor;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
};
