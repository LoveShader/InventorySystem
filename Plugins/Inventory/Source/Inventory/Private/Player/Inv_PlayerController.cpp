// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inv_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Inv_HighlightableStaticMesh.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	TraceLenght = 500.0f;
}

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

void AInv_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceWithItems();
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

void AInv_PlayerController::TraceWithItems()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport))
		return;

	// Get ScreenSpace Center Coordinates
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// Get WorldSpace Trace Start and Trace Direction
	// use Start and Direction to calculate Trace End
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter,  TraceStart, Forward))
		return;

	const FVector TraceEnd = TraceStart + Forward * TraceLenght;

	//Do Line With, Trace Channel will be set in Blueprint
	//use our custom Trace Channel to avoid uneffective trace
	FHitResult TraceResult;
	GetWorld()->LineTraceSingleByChannel(TraceResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = TraceResult.GetActor();
	
	if (LastActor == ThisActor)
		return;

	// if LastActor is Valid and ThisActor is not Valid, there is not a valid hit result
	if (LastActor.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}

		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}

	// if ThisActor is Valid and LastActor is not Valid, there is a valid hit result
	if (ThisActor.IsValid())
	{
		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (IsValid(ItemComponent))
		{
			if (IsValid(HUDWidget))
			{
				HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
			}
		}

		if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
		{
			IInv_Highlightable::Execute_Highlight(Highlightable);
		}
	}
}
