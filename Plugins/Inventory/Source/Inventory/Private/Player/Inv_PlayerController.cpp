// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inv_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (PrimaryInteractAction == nullptr)
		{
			UE_LOG(LogInventory, Error, TEXT("PrimaryInteractAction has not set"));
		}
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ThisClass::PrimaryInteract);

	}
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(DefaultIMC, 0);
	}

	CreateHUDWidget();
}

void AInv_PlayerController::PrimaryInteract()
{
	UE_LOG(LogInventory, Log, TEXT("PrimaryInteract Key Pressed"));
}

void AInv_PlayerController::CreateHUDWidget()
{
	//Only Show HUD in Local Player
	if (!IsLocalController())
		return;

	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}
