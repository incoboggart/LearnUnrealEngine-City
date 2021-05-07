// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadsGameModeBase.h"


#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "FGridMath.h"
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"

ARoadsGameModeBase::ARoadsGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	
	Grid = FTilesGrid();
	GridSize = FIntVector(500, 500, 1);
	
	DrawTileDirectionsEnabled = true;
	DrawTileDirectionsColor = FColor::Green;
	DrawJunctionsEnabled = true;
	DrawJunctionsColor = FColor::Green;
	DrawLinesEnabled = true;
	DrawLinesColor = FColor::Green;
}

void ARoadsGameModeBase::BeginPlay()
{
	Grid.GridSize = GridSize;
	Grid.BuildGrid(GetWorld());
}

void ARoadsGameModeBase::Tick(float DeltaSeconds)
{
	if(DrawTileDirectionsEnabled)
	{
		DrawTileDirections();
	}

	if(DrawJunctionsEnabled)
	{
		DrawJunctions();
	}

	if(DrawLinesEnabled)
	{
		DrawLines();
	}
}

bool ARoadsGameModeBase::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ARoadsGameModeBase::DrawTileDirections() const
{
	const float MarkerSize = 50;
	const FColor MarkerColor = DrawTileDirectionsColor;
	const float AngleSize = FMath::DegreesToRadians(30);
	TArray<EDirections> Directions = TArray<EDirections>();
	
	for (auto Pair : Grid.GetTilesMap())
	{
		const FGridTile Tile = Pair.Value;
		const FIntVector TileCenter = FGridMath::ToTileCenter(Tile.Id, Grid.GridSize);
		FVector MarkerPosition = FVector(TileCenter.X, TileCenter.Y, 50);
		if(Tile.LocalDirections == EDirections::None)
		{
			DrawDebugBox(GetWorld(), MarkerPosition, FVector(MarkerSize/2), FColor::Red);
		}		
		if(Tile.LocalDirections == EDirections::All)
		{
			DrawDebugSphere(GetWorld(), MarkerPosition, MarkerSize, 4, MarkerColor);
		}
		else
		{
			SplitDirections(Tile.LocalDirections, Directions);
			for (EDirections Direction : Directions)
			{
				FVector ConeDirection = Tile.GetWorldDirection(Direction);
				DrawDebugCone(GetWorld(), MarkerPosition - ConeDirection * MarkerSize, ConeDirection, MarkerSize, AngleSize, AngleSize, 2, MarkerColor);
			}
		}
	}
}

void ARoadsGameModeBase::DrawJunctions() const
{
	for (FGridJunction Junction : Grid.GetJunctions())
	{
		FIntVector Min = FIntVector(MAX_int32,MAX_int32,MAX_int32);
		FIntVector Max = FIntVector(MIN_int32,MIN_int32,MIN_int32);
		
		for (const FIntVector TileId : Junction.Items)
		{
#define MIN(sym) Min.sym = FMath::Min(Min.sym, TileId.sym)
#define MAX(sym) Max.sym = FMath::Max(Max.sym, TileId.sym)
			
			MIN(X);
			MIN(Y);
			MIN(Z);
			MAX(X);
			MAX(Y);
			MAX(Z);

#undef MIN
#undef MAX
		}

		Min = FGridMath::ToTileCenter(Min, Grid.GridSize);
		Max = FGridMath::ToTileCenter(Max, Grid.GridSize);

#define AVG(sym) ((Min.sym + Max.sym) / 2.0)
		FVector Position = FVector(AVG(X), AVG(Y), AVG(Z) + 50);
#undef AVG

		DrawDebugSphere(GetWorld(), Position, 60, 8, DrawJunctionsColor);
	}
}

void ARoadsGameModeBase::DrawLines() const
{
	for (FGridLine Line : Grid.GetLines())
	{
		FVector Start = FGridMath::ToTileCenterLocation(Line.Min, Grid.GridSize);
		FVector End = FGridMath::ToTileCenterLocation(Line.Max, Grid.GridSize);

		Start.Z = 50;
		End.Z = 50;
		
		DrawDebugLine(GetWorld(),
			Start,
			End,
			DrawLinesColor);
	}
}


