// Fill out your copyright notice in the Description page of Project Settings.


#include "Inv_HighlightableStaticMesh.h"

void UInv_HighlightableStaticMesh::Highlight_Implementation()
{
	IInv_Highlightable::Highlight_Implementation();

	if (IsValid(HighlightMaterial))
	{
		SetOverlayMaterial(HighlightMaterial);
	}
}

void UInv_HighlightableStaticMesh::UnHighlight_Implementation()
{
	IInv_Highlightable::UnHighlight_Implementation();

	SetOverlayMaterial(nullptr);
}
