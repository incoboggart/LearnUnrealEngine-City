// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadsGameModeBase.h"


#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "GridDirections.h"
#include "GridJunction.h"
#include "GridLine.h"
#include "GridMath.h"
#include "GridPathfinder.h"
#include "GridTile.h"
#include "SplineActor.h"

ARoadsGameModeBase::ARoadsGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	
	Grid = FTilesGridBuilder();
	TileSize = FIntVector(500, 500, 1);

	DrawTileDirectionsEnabled = true;
	DrawTileDirectionsColor = FColor::Green;
	DrawJunctionsEnabled = true;
	DrawJunctionsColor = FColor::Green;
	DrawLinesEnabled = true;
	DrawLinesColor = FColor::Green;

	SplineActorClass = ASplineActor::StaticClass();
}

void ARoadsGameModeBase::BeginPlay()
{
	Grid.TileSize = TileSize;
	Grid.BuildGrid(GetWorld());

	BuildTestRoute();

	TestSplineInstance = GetWorld()->SpawnActor<ASplineActor>(SplineActorClass);
	if(!IsValid(TestSplineInstance))
	{
		return;
	}

	TArray<FVector> Points = TArray<FVector>();
	FVector Offset = FVector(0,0,50);
	for (const FIntVector TileId : TestRoute)
	{
		if(Grid.GetTile(TileId).IsOrthoTurn())
		{
			continue;
		}
			
		Points.Add(FGridMath::ToTileCenterLocation(TileId, Grid.TileSize) + Offset);
	}

	 TestSplineInstance->PopulatePoints(Points);

	APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(!IsValid(Pawn))
	{
		return;
	}
	
	Pawn->SetActorLocation(FGridMath::ToTileCenterLocation(TestRouteStart, Grid.TileSize), false, nullptr, ETeleportType::ResetPhysics);

	TestSplineInstance->SetActor(Pawn);
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

	FIntVector TileCenter = FGridMath::ToTileCenter(TestRouteStart, Grid.TileSize);
	FVector MarkerPosition = FVector(TileCenter.X, TileCenter.Y, 50);
	DrawDebugSphere(GetWorld(), MarkerPosition, 60, 8, FColor::Red);

	TileCenter = FGridMath::ToTileCenter(TestRouteFinish, Grid.TileSize);
	MarkerPosition = FVector(TileCenter.X, TileCenter.Y, 50);
	DrawDebugSphere(GetWorld(), MarkerPosition, 60, 8, FColor::Red);
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
	TArray<FIntVector> Directions = TArray<FIntVector>();
	
	for (auto Pair : Grid.GetTilesMap())
	{
		const FGridTile Tile = Pair.Value;
		const FIntVector TileCenter = FGridMath::ToTileCenter(Tile.Id, Grid.TileSize);
		FVector MarkerPosition = FVector(TileCenter.X, TileCenter.Y, 50);
		if(FGridDirections::IsEmpty(Tile.LocalDirections))
		{
			DrawDebugBox(GetWorld(), MarkerPosition, FVector(MarkerSize/2), FColor::Red);
		}
		else if(FGridDirections::IsOrthoCross(Tile.LocalDirections))
		{
			DrawDebugSphere(GetWorld(), MarkerPosition, MarkerSize, 4, MarkerColor);
		}
		else
		{
			Tile.GetWorldDirections(Directions, true);
			for (const FIntVector Direction : Directions)
			{
				FVector ConeDirection = -FGridMath::F(Direction);
				DrawDebugCone(GetWorld(), MarkerPosition - ConeDirection * MarkerSize, ConeDirection, MarkerSize, AngleSize, AngleSize, 2, MarkerColor);
			}
			
			// for (FIntVector Direction : Tile.LocalDirections)
			// {
			// 	FVector ConeDirection =  FGridMath::F(FGridMath::Rotate(Direction, Tile.Rotation));
			// 	DrawDebugCone(GetWorld(), MarkerPosition - ConeDirection * MarkerSize, ConeDirection, MarkerSize, AngleSize, AngleSize, 2, MarkerColor);
			// }
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

		Min = FGridMath::ToTileCenter(Min, Grid.TileSize);
		Max = FGridMath::ToTileCenter(Max, Grid.TileSize);

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
		FVector Start = FGridMath::ToTileCenterLocation(Line.Min, Grid.TileSize);
		FVector End = FGridMath::ToTileCenterLocation(Line.Max, Grid.TileSize);

		Start.Z = 50;
		End.Z = 50;
		
		DrawDebugLine(GetWorld(),
			Start,
			End,
			DrawLinesColor);
	}
}

void ARoadsGameModeBase::BuildTestRoute()
{
	TArray<FIntVector> Keys;
	Grid.TileById.GenerateKeyArray(Keys);

	if(TestRouteStart == FIntVector::ZeroValue)
	{
		TestRouteStart = Keys[FMath::RandRange(0, Keys.Num() - 1)];
	}
	if(TestRouteFinish == FIntVector::ZeroValue)
	{
		TestRouteFinish = Keys[FMath::RandRange(0, Keys.Num() - 1)];
	}

	//Error: Failed to find path from X=3 Y=-29 Z=0 to X=5 Y=-23 Z=0
	FGridPathfinder Pathfinder = FGridPathfinder();
	Pathfinder.Start = TestRouteStart;
	Pathfinder.Finish = TestRouteFinish;
	Pathfinder.FindPath(Grid, TestRoute);
}


