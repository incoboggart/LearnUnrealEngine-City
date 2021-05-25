#pragma once
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"

#include "TilesGridBuilder.generated.h"

USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FTilesGridBuilder
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector TileSize;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FIntVector, FGridTile> TileById;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGridJunction> Junctions;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGridLine> Lines;

	FTilesGridBuilder();

	FORCEINLINE const TMap<FIntVector, FGridTile>& GetTilesMap() const
	{
		return TileById;
	}
	
	FORCEINLINE const TArray<FGridJunction>& GetJunctions() const
	{
		return Junctions;
	}
	
	FORCEINLINE const TArray<FGridLine>& GetLines() const
	{
		return Lines;
	}		

	FORCEINLINE bool IsJunctionTile(const FGridTile& Tile) const
	{
		return Junctions.FindByPredicate([&](const FGridJunction j){return j.Contains(Tile);});
	}

	FORCEINLINE bool ContainsTile(const FIntVector& TileId) const
	{
		return TileById.Contains(TileId);
	}	

	FGridTile& GetTile(const FIntVector& TileId)
	{
		return TileById[TileId];
	}

	void BuildGrid(UWorld* World);

	static FTilesGridBuilder BuildGrid(const FIntVector TileSize, UWorld* World)
	{
		FTilesGridBuilder GridBuilder;
		GridBuilder.TileSize = TileSize;
		GridBuilder.BuildGrid(World);
		return GridBuilder;
	}

private:
	void CreateTiles(UWorld* World);
	void CreateJunctions(const TArray<FGridTile>& Tiles);
	void SimplifyTileDirections(const TArray<FGridTile>& Tiles);
	void CreateLines(const TArray<FGridTile>& Tiles);
	void SimplifyJunctions(const TArray<FGridTile>& Tiles);
	void CreateDirections(const TArray<FGridTile>& Tiles);
};
