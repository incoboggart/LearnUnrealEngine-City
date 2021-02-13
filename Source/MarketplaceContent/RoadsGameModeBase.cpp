// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadsGameModeBase.h"


#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"
#include "Engine/StaticMeshActor.h"

ARoadsGameModeBase::ARoadsGameModeBase()
{
	TilesMap = TMap<FIntVector, FGridTile>();
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	GridSize = FIntVector(500, 500, 1);
	DrawTileDirectionsEnabled = true;
	DrawTileDirectionsColor = FColor::Green;
	DrawJunctionsEnabled = true;
	DrawJunctionsColor = FColor::Green;	
}

void ARoadsGameModeBase::BeginPlay()
{
	CreateTiles();
	CreateJunctions();
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
}

bool ARoadsGameModeBase::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ARoadsGameModeBase::CreateTiles()
{
	TArray<FIntVector> Crossings = TArray<FIntVector>();
	
	FActorIterator AllActorsIterator = FActorIterator(GetWorld());
	while (AllActorsIterator)
	{	
		AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(*AllActorsIterator);
		if(StaticMeshActor != nullptr)
		{
			FString ActorName = StaticMeshActor->GetName().ToLower();
			if (ActorName.Contains("road")
                && !ActorName.Contains("parking"))
			{
				FVector ActorCenter, ActorExtent;
				StaticMeshActor->GetActorBounds(false, ActorCenter, ActorExtent, true);

				UE_LOG(LogTemp, Display, TEXT("Road static mesh at %s"), *ActorCenter.ToString())

				FIntVector TileId = FGridTile::ToTileId(ActorCenter, GridSize); 
				if(TilesMap.Contains(TileId))
				{
					UE_LOG(LogTemp, Warning, TEXT("DUPLICATE %s From %s -> %s"), *TileId.ToString(), *StaticMeshActor->GetName(), *TilesMap[TileId].Actor->GetName())
				}

				FGridTile Tile = FGridTile();
				Tile.Id = TileId;
				Tile.Actor = StaticMeshActor;
				if(ActorName.Contains("bare"))
				{
					Tile.Directions = EDirections::All;
					//Tile.DirectionsArray = FGridDirections::Ortho;					
				}
				else if(ActorName.Contains("crossing"))
				{
					Tile.Directions = EDirections::X;
					//Tile.DirectionsArray = FGridDirections::X;

					Crossings.Add(TileId);
				}
				else
				{
					Tile.Directions = EDirections::YNeg;
					//Tile.DirectionsArray.Empty(1);
					//Tile.DirectionsArray.Add(FIntVector(0, -1, 0));
				}
				               
				TilesMap.Add(Tile.Id, Tile);
			}
		}

		++AllActorsIterator;
	}

	const TArray<EDirections> Directions = {EDirections::XNeg, EDirections::XPos, EDirections::YNeg, EDirections::YPos};
	for (FIntVector CrossingTileId : Crossings)
	{
		auto CrossingTile = TilesMap[CrossingTileId];
		
		for (const EDirections Direction : Directions)
		{
			auto NeighbourId = CrossingTile.GetNeighbourId(Direction);
			if(TilesMap.Contains(NeighbourId))
			{
				const auto NeighbourTile = TilesMap[NeighbourId];
				if(NeighbourTile.Directions == EDirections::All)
				{
					continue;
				}

				
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Road tiles count: %d"), TilesMap.Num())
}

void ARoadsGameModeBase::CreateJunctions()
{
	Junctions = TArray<FGridJunction>();
	
	TSet<FIntVector> Visited = TSet<FIntVector>();
	TArray<FIntVector> Queue = TArray<FIntVector>();

	const TArray<EDirections> Directions = {EDirections::XNeg, EDirections::XPos, EDirections::YNeg, EDirections::YPos};

	TArray<FGridTile> Tiles = TArray<FGridTile>();
	TilesMap.GenerateValueArray(Tiles);

	for (FGridTile Tile : Tiles)
	{
		if(!FGridTile::IsAllDirections(Tile))
		{
			continue;
		}

		if(Junctions.FindByPredicate([&](const FGridJunction j)
		{
			return j.Contains(Tile);
		}) != nullptr)
		{
			continue;
		}

		Visited.Add(Tile.Id);

		FGridJunction Junction = FGridJunction();
		Junction.Add(Tile);		
				
		for (const EDirections Direction : Directions)
		{
			FIntVector NeighbourId = Tile.GetNeighbourId(Direction);
			if(TilesMap.Contains(NeighbourId))
			{
				Queue.Add(NeighbourId);
			}
		}

		while (Queue.Num() != 0)
		{
			FIntVector QueuedId = Queue.Pop();
			if(Visited.Contains(QueuedId))
			{
				continue;;
			}

			Visited.Add(QueuedId);

			const FGridTile QueuedTile = TilesMap[QueuedId];
			if(!FGridTile::IsAllDirections(QueuedTile))
			{
				continue;
			}

			Junction.Add(QueuedTile);
			
			for (const EDirections Direction : Directions)
			{
				FIntVector NeighbourId = QueuedTile.GetNeighbourId(Direction);
				if(!TilesMap.Contains(NeighbourId))
				{
					continue;
				}

				if(Visited.Contains(NeighbourId))
				{
					continue;
				}	

				Queue.Add(NeighbourId);
			}
		}

		if(!Junction.IsSingleItem())
		{
			Junctions.Add(Junction);
		}

		Visited.Reset();
	}

	UE_LOG(LogTemp, Display, TEXT("Junctions count: %d"), Junctions.Num())
}

void ARoadsGameModeBase::CreateLines()
{
	// const TArray<EDirections> Directions = {EDirections::XNeg, EDirections::XPos, EDirections::YNeg, EDirections::YPos};
	//     	
	// for (FGridTile JunctionTile : Junctions)
	// {
	// 	for (auto Direction : Directions)
	// 	{
	// 		FIntVector NeighbourId = JunctionTile.GetNeighbourId(Direction);
	// 		if(!TilesMap.Contains(NeighbourId))
	// 		{
	// 			continue;
	// 		}
 //    
	// 		FGridTile Tile = TilesMap[NeighbourId];
	// 		if(Tile.Directions == EDirections::All)
	// 		{
	// 			continue;
	// 		}
 //    			
	// 		if(Lines.ContainsByPredicate([&](FGridLine Street)
 //                        {
 //                            return Street.BeginsWith(NeighbourId);
 //                        }))
	// 		{
	// 			continue;
	// 		}
	// 	}
	// }
}

void ARoadsGameModeBase::DrawTileDirections()
{
	const float MarkerSize = 50;
	const FColor MarkerColor = DrawTileDirectionsColor;
	const float AngleSize = FMath::DegreesToRadians(30);
	TArray<EDirections> Directions = TArray<EDirections>();
	
	for (auto Pair : TilesMap)
	{
		const FGridTile Tile = Pair.Value;
		const FIntVector TileCenter = FGridTile::ToTileCenter(Tile.Id, GridSize);
		FVector MarkerPosition = FVector(TileCenter.X, TileCenter.Y, 50);
		if(Tile.Directions == EDirections::None)
		{
			DrawDebugBox(GetWorld(), MarkerPosition, FVector(MarkerSize/2), FColor::Red);
		}		
		if(Tile.Directions == EDirections::All)
		{
			DrawDebugSphere(GetWorld(), MarkerPosition, MarkerSize, 4, MarkerColor);
		}
		else
		{
			SplitDirections(Tile.Directions, Directions);
			for (EDirections Direction : Directions)
			{
				FVector ActorRotation = Tile.Actor->GetActorRotation().Vector();
				FVector ConeDirection;
				switch (Direction)
				{
				case EDirections::XPos:
					ConeDirection = -ActorRotation;
					break;
				case EDirections::XNeg:
					ConeDirection = ActorRotation;
					break;
				// case EDirections::YPos:
				// 	{
				// 		ConeDirection = ActorRotation.RotateAngleAxis(90, FVector(0,0,1));
				// 		//ConeDirection = FVector(-ActorRotation.Y, ActorRotation.X, ActorRotation.Z);
				// 		// FVector TestVectorPos = FVector(ActorRotation.Y, ActorRotation.X, ActorRotation.Z);
				// 		// UE_LOG(LogTemp, Display, TEXT("POS Actor: %s Normal: %s, Hacky: %s"), *ActorRotation.ToString(), *ConeDirection.ToString(), *TestVectorPos.ToString())
				// 	}					
				// 	break;
				case EDirections::YNeg:
					{
						ConeDirection = ActorRotation.RotateAngleAxis(-90, FVector(0,0,1));
						//ConeDirection = FVector(ActorRotation.Y, ActorRotation.X, ActorRotation.Z);
						// FVector TestVectorNeg = FVector(-ActorRotation.Y, ActorRotation.X, ActorRotation.Z);
						// UE_LOG(LogTemp, Display, TEXT("NEG Actor: %s Normal: %s, Hacky: %s"), *ActorRotation.ToString(), *ConeDirection.ToString(), *TestVectorNeg.ToString())
					}					
					break;
				}
			
				DrawDebugCone(GetWorld(), MarkerPosition - ConeDirection * (MarkerSize), ConeDirection, MarkerSize, AngleSize, AngleSize, 2, MarkerColor);
			}
		}
	}
}

void ARoadsGameModeBase::DrawJunctions()
{
	for (FGridJunction Junction : Junctions)
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

		Min = FGridTile::ToTileCenter(Min, GridSize);
		Max = FGridTile::ToTileCenter(Max, GridSize);

#define AVG(sym) ((Min.sym + Max.sym) / 2.0)
		FVector Position = FVector(AVG(X), AVG(Y), AVG(Z) + 50);
#undef AVG

		DrawDebugSphere(GetWorld(), Position, 60, 8, DrawJunctionsColor);
	}

	
}

