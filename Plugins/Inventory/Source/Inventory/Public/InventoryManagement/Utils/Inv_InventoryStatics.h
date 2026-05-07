// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"
enum class EInv_ItemCategory : uint8;
class UInv_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComp);

	template<typename T, typename Func>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 Columns, const Func& Function);	
};

template <typename T, typename Func>
void UInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 Columns,
	const Func& Function)
{
	//1. 先遍历行，再遍历列
	for (int32 j = 0; j < Range2D.Y; ++j)
	{
		for (int32 i = 0; i < Range2D.X; ++i)
		{
			//2. 将当前索引cast为二维方式
			const FIntPoint Coordinates = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) + FIntPoint(i, j);
			//3. 将当前坐标再转换为索引形式
			const int32 TileIndex = UInv_WidgetUtils::GetIndexFromPosition(Coordinates, Columns);
			//4. 针对每一个格子使用传入的Function进行处理
			Function(Array[TileIndex]);
		}
	}
}
