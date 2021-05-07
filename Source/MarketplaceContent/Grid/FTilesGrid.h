#pragma once
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"

#include "FTilesGrid.generated.h"

USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FTilesGrid
{
	GENERATED_BODY()

	FTilesGrid();
	
	FORCEINLINE const TMap<FIntVector, FGridTile>& GetTilesMap() const { return TilesMap; }
	FORCEINLINE const TArray<FGridJunction>& GetJunctions() const { return Junctions; }
	FORCEINLINE const TArray<FGridLine>& GetLines() const { return Lines; }	

	void BuildGrid(UWorld* World);

	FORCEINLINE bool IsJunctionTile(const FGridTile& Tile) const
	{
		return Junctions.FindByPredicate([&](const FGridJunction j){return j.Contains(Tile);});
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector GridSize;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FIntVector, FGridTile> TilesMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGridJunction> Junctions;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FGridLine> Lines;

private:
	void CreateTiles(UWorld* World);
	void CreateJunctions(const TArray<FGridTile>& Tiles);
	void CreateLines(const TArray<FGridTile>& Tiles);
	void CreateDirections(const TArray<FGridTile>& Tiles);
};
