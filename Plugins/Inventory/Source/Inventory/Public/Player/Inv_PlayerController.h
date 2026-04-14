// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
class UInputMappingContext;
class UInputAction;

UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
protected:
	virtual void BeginPlay() override;
private:
	void PrimaryInteract();
	void CreateHUDWidget();
private:
	UPROPERTY(EditAnywhere, Category = Inventory)
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;
};
