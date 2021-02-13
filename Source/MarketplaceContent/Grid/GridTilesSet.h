// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridTile.h"


#include "GridTilesSet.generated.h"

/**
 * 
 */
USTRUCT()
struct FGridTilesSet
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> Items;

	FORCEINLINE bool IsEmpty() const
	{
		return Items.Num() == 0;
	}

	FORCEINLINE bool Contains(const FIntVector& TileId) const
	{
		return Items.Contains(TileId);
	}

	FORCEINLINE bool Contains(const FGridTile& Tile) const
	{
		return Items.Contains(Tile.Id);
	}

	FORCEINLINE void Add(const FIntVector& TileId)
	{
		Items.Add(TileId);
	}

	FORCEINLINE void Add(const FGridTile& Tile)
	{
		Items.Add(Tile.Id);
	}
};
