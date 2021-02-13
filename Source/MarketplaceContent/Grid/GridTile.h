// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chaos/AABBTree.h"
#include "UObject/NoExportTypes.h"
#include "EDirections.h"
#include "Chaos/AABBTree.h"

#include "GridTile.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FGridTile 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntVector Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIntVector> DirectionsArray;

	EDirections Directions;

	UPROPERTY(VisibleAnywhere)
	AActor* Actor;

	FIntVector GetNeighbourId(const EDirections Directions) const;

	static bool IsAllDirections(const FGridTile& Tile)
	{
		return Tile.Directions == EDirections::All; 
	}

	static FIntVector ToTileId(const FVector ActorCenter, const FIntVector GridSize)
	{
#define CALC(component) ((FMath::RoundToInt(ActorCenter.component) - GridSize.component / 2) / GridSize.component)
				
		return FIntVector(CALC(X), CALC(Y), FMath::RoundToInt(ActorCenter.Z) / GridSize.Z);

#undef  CALC
	}	

	static FIntVector ToTileId(const FIntVector TileCenter, const FIntVector GridSize)
	{
#define CALC(component) ((TileCenter.component - GridSize.component / 2) / GridSize.component)
				
		return FIntVector(CALC(X), CALC(Y), TileCenter.Z / GridSize.Z);

#undef  CALC
	}

	static FIntVector ToTileCenter(const FIntVector TileId, const FIntVector GridSize)
	{
#define CALC(component) ((TileId.component * GridSize.component) + GridSize.component / 2)
		
		return  FIntVector(CALC(X), CALC(Y), TileId.Z * GridSize.Z);

#undef CALC
	}
};


