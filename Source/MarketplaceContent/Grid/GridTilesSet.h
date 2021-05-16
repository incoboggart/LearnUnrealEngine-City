// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridTile.h"

#include "GridTilesSet.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGridTilesSet
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> Items;

	FORCEINLINE bool IsEmpty() const
	{
		return Items.Num() == 0;
	}

	FORCEINLINE bool Contains(const FIntVector TileId) const 
	{
		return Items.Contains(TileId);
	}

	FORCEINLINE bool Contains(const FGridTile& Tile) const 
	{
		return Items.Contains(Tile.Id);
	}

	FORCEINLINE void Add(const FIntVector TileId)
	{
		Items.Add(TileId);
	}

	FORCEINLINE void Add(const FGridTile& Tile)
	{
		Items.Add(Tile.Id);
	}

	FORCEINLINE bool Select(TMap<FIntVector, FGridTile>& TileById, TArray<FGridTile>& Tiles)
	{
		for (FIntVector TileId : Items)
		{
			if(!TileById.Contains(TileId))
			{
				return false;
			}

			Tiles.Add(TileById[TileId]);
		}

		return true;
	}

	bool HasDiff(TSet<FIntVector>& Set)
	{
		if(Set.Num() != Items.Num())
		{
			return true;
		}

		for (FIntVector TileId : Items)
		{
			if(!Set.Contains(TileId))
			{
				return true;
			}
		}

		return false;
	}
};
