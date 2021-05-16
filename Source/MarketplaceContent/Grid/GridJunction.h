// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridDirections.h"
#include "GridLine.h"
#include "GridTilesSet.h"
#include "GridJunction.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FGridJunction : public FGridTilesSet
{
	GENERATED_BODY()

	FORCEINLINE bool IsSingleItem() const
	{
		return Items.Num() == 1;
	}

	FORCEINLINE bool IsLine() const
	{
		if(IsEmpty()
			|| IsSingleItem())
		{
			return false;
		}
		
		FIntVector One = Items[0];
		FIntVector Two = Items[1];
		FIntVector Dir = Two - One;
		Dir.Z = 0;
				
		for(int i = 1, m = Items.Num() - 1; i < m; i++)
		{
			One = Items[i];
			Two = Items[i + 1];
			FIntVector Dir2 = Two - One;
			Dir2.Z = 0;

			if(Dir != Dir2)
			{
				return false;
			}
		}

		return true;
	}

	static bool TrySimplifyJunction(FGridJunction& Junction, TMap<FIntVector, FGridTile>& TileById)
	{
		return false;
		
		if(TrySimplifyToLines(Junction, TileById))
		{
			return true;
		}

		return false;
	}

private:
	static bool TrySimplifyToLines(FGridJunction& Junction, TMap<FIntVector, FGridTile>& TileById)
	{
		TArray<FGridLine> Lines = TArray<FGridLine>();
				
		TArray<FGridTile> Tiles = TArray<FGridTile>();
        TSet<FIntVector> Visited = TSet<FIntVector>();
        TArray<FIntVector> WorldDirections = TArray<FIntVector>();

		if(!Junction.Select(TileById, Tiles))
		{
			return false;
		}
    
		for (FGridTile Tile : Tiles)
		{
			if(Visited.Contains(Tile.Id))
			{
				continue;
			}
    
			Tile.GetWorldDirections(WorldDirections);

			FIntVector Min = Tile.Id;
			FIntVector Max = Tile.Id;
    
			for (const FIntVector Direction : WorldDirections)
            {
            	FGridTile Next = Tile;
				Visited.Add(Tile.Id);
            	
            	while(true)
            	{
            		FIntVector NextId = Next.GetNeighbourId(Direction);
            		if(!TileById.Contains(NextId)
            			|| !Junction.Contains(NextId))
            		{
            			break;
            		}
            		
            		Next = TileById[NextId];
            		if(!Next.IsStraightLine()
            			&& !Next.IsOrthoCross())
            		{
            			break;
            		}
    
            		Min = FGridMath::Min(Min, NextId);
            		Max = FGridMath::Max(Max, NextId);

            		Visited.Add(NextId);
    
            		if(!Next.ContainsAnyLocalDirection(WorldDirections))
            		{
            			break;
            		}
            	}
            }
    
            FGridLine Line = FGridLine(Min, Max);
            if(Line.Num() > 1)
            {
            	Lines.Add(Line);
            }
		}

		if(Junction.HasDiff(Visited))
		{
			return false;
		}

		// TODO straighten tile directions according to lines
		// for (FGridLine Line : Lines)
		// {
		// 	FIntVector Dir1 = Line.Min - Line.Max;
		// 	FIntVector Dir2 = Line.Max - Line.Min;
		// 				
		// 	for(int x = Line.Min.X; x <= Line.Max.X; x++)
		// 	for(int y = Line.Min.Y; y <= Line.Max.Y; y++)
		// 	for(int z = Line.Min.Z; z <= Line.Max.Z; z++)
		// 	{
		// 		FIntVector TileId = FIntVector(x,y,z);
		// 		TileById[TileId].Set
		// 	}
		// }

		return true;
	}
	
};
