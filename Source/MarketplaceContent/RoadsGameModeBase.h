// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"
#include "Chaos/AABBTree.h"
#include "Grid/EDirections.h"
#include "GameFramework/GameModeBase.h"
#include "RoadsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MARKETPLACECONTENT_API ARoadsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FIntVector GridSize;

	UPROPERTY(EditAnywhere)
	bool DrawTileDirectionsEnabled;

	UPROPERTY(EditAnywhere)
	FColor DrawTileDirectionsColor;

	UPROPERTY(EditAnywhere)
	bool DrawJunctionsEnabled;

	UPROPERTY(EditAnywhere)
	FColor DrawJunctionsColor;

	UPROPERTY(VisibleAnywhere)
	TMap<FIntVector, FGridTile> TilesMap;

	UPROPERTY(VisibleAnywhere)
	TArray<FGridJunction> Junctions;

	UPROPERTY(VisibleAnywhere)
	TArray<FGridLine> Lines;

	ARoadsGameModeBase();

	virtual void BeginPlay() override;

	
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE bool IsJunctionTile(const FGridTile& Tile)
	{
		return Junctions.FindByPredicate([&](FGridJunction j){return j.Contains(Tile);});
	}

	virtual bool ShouldTickIfViewportsOnly() const override;

	void CreateTiles();

	void CreateJunctions(const TArray<FGridTile>& Tiles);

	void CreateLines(const TArray<FGridTile>& Tiles);

	void CreateDirections(const TArray<FGridTile>& Tiles);

	void DrawTileDirections();

	void DrawJunctions();
};


