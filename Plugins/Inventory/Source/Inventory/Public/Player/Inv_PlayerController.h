// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

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
private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> PrimaryInteractAction;
};
