// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/UInv_InfoMessage.h"

#include "Components/TextBlock.h"

void UInv_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(TextMessage))	return;

	TextMessage->SetText(FText::GetEmpty());
	MessageHide();
}

void UInv_InfoMessage::SetMessage(const FText& Message)
{
	if (!IsValid(TextMessage))	return;

	TextMessage->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(
		MessageTimer,
		[this]()
		{
			MessageHide();
			bIsMessageActive = false;
		},
		MessageLifeTime,
		false
	);
}
