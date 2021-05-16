#pragma once
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"

#include "TilesGrid.generated.h"

USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FTilesGrid
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector GridSize;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FIntVector, FGridTile> TileById;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGridJunction> Junctions;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGridLine> Lines;

	FTilesGrid();

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

private:
	void CreateTiles(UWorld* World);
	void CreateJunctions(const TArray<FGridTile>& Tiles);
	void SimplifyTileDirections(const TArray<FGridTile>& Tiles);
	void CreateLines(const TArray<FGridTile>& Tiles);
	void SimplifyJunctions(const TArray<FGridTile>& Tiles);
	void CreateDirections(const TArray<FGridTile>& Tiles);
};
