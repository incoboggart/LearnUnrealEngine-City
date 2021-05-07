// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridTilesSet.h"

#include "GridLine.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FGridLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector Min;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector Max;

	FGridLine() :
	Min(FIntVector()),
	Max(FIntVector())
	{  }

	FGridLine(const FIntVector P1, const FIntVector P2)
	{
		Min = FGridMath::GetMin(P1, P2);
		Max = FGridMath::GetMax(P1, P2);
	}

	FORCEINLINE bool IsEmpty() const
	{
		return Min == Max;
	};

	FORCEINLINE bool Contains(const FIntVector& TileId) const
	{
		return Min.X >= TileId.X
			&& Min.Y >= TileId.Y
			&& Min.Z >= TileId.Z 
			&& Max.X <= TileId.X
			&& Max.Y <= TileId.Y
			&& Max.Z <= TileId.Z;
	}

	FORCEINLINE int32 Num() const
	{
		return (Min.X == Max.X
			? Max.Y - Min.Y
			: Max.X - Min.X) + 1;
	}
};
