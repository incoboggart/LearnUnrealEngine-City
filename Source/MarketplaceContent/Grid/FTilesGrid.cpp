#include "FTilesGrid.h"

#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"

FTilesGrid::FTilesGrid()
{
	TilesMap = TMap<FIntVector, FGridTile>();

	Junctions = TArray<FGridJunction>();
	Lines = TArray<FGridLine>();
}

void FTilesGrid::BuildGrid(UWorld* World)
{	
	CreateTiles(World);
	
	TArray<FGridTile> Tiles = TArray<FGridTile>();
	TilesMap.GenerateValueArray(Tiles);
	
	CreateJunctions(Tiles);
	CreateLines(Tiles);
}

void FTilesGrid::CreateTiles(UWorld* World)
{
	TArray<FIntVector> Crossings = TArray<FIntVector>();
	TArray<FIntVector> Bare = TArray<FIntVector>();
	
	FActorIterator AllActorsIterator = FActorIterator(World);
	while (AllActorsIterator)
	{	
		AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(*AllActorsIterator);
		++AllActorsIterator;
		
		if(StaticMeshActor == nullptr)
		{
			continue;
		}

		FString ActorName = StaticMeshActor->GetName().ToLower();
		if (!ActorName.Contains("road")
			|| ActorName.Contains("parking"))
		{
			continue;
		}
		
		FVector ActorCenter, ActorExtent;
		StaticMeshActor->GetActorBounds(false, ActorCenter, ActorExtent, true);

		UE_LOG(LogTemp, Display, TEXT("Road static mesh at %s"), *ActorCenter.ToString())

		FIntVector TileId = FGridMath::ToTileId(ActorCenter, GridSize); 
		if(TilesMap.Contains(TileId))
		{
			UE_LOG(LogTemp, Warning, TEXT("DUPLICATE %s From %s -> %s"), *TileId.ToString(), *StaticMeshActor->GetName(), *TilesMap[TileId].Actor->GetName())
		}

		FGridTile Tile = FGridTile();
		Tile.Id = TileId;
		Tile.Actor = StaticMeshActor;
		Tile.ActorRotation = FGridMath::RoundToInt(StaticMeshActor->GetActorRotation().Vector());
				
		if(ActorName.Contains("bare"))
		{
			Bare.Add(TileId);

			Tile.LocalDirections = EDirections::All;
		}
		else if(ActorName.Contains("crossing"))
		{
			Crossings.Add(TileId);

			Tile.LocalDirections = EDirections::X;
		}
		else
		{
			Tile.LocalDirections = EDirections::Y;
		}
				               
		TilesMap.Add(Tile.Id, Tile);
	}

	UE_LOG(LogTemp, Display, TEXT("Road tiles count: %d"), TilesMap.Num())
}

void FTilesGrid::CreateJunctions(const TArray<FGridTile>& Tiles)
{	
	TSet<FIntVector> Visited = TSet<FIntVector>();
	TArray<FIntVector> Queue = TArray<FIntVector>();

	const TArray<EDirections> Directions = {EDirections::XNeg, EDirections::XPos, EDirections::YNeg, EDirections::YPos};

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

void FTilesGrid::CreateLines(const TArray<FGridTile>& Tiles)
{
	Lines = TArray<FGridLine>();
	
	TSet<FIntVector> Visited = TSet<FIntVector>();
	TArray<FIntVector> Queue = TArray<FIntVector>();
	TArray<EDirections> TileDirections = TArray<EDirections>();
	TArray<FIntVector> WorldDirections = TArray<FIntVector>();
	
	for (FGridTile Tile : Tiles)
	{
		if(!Tile.IsStraight())
		{
			continue;
		}

		if(Visited.Contains(Tile.Id))
		{
			continue;
		}

		Visited.Add(Tile.Id);
		
		Tile.GetLocalDirectionsSplit(TileDirections);
		for (EDirections Direction : TileDirections)
		{
			WorldDirections.Add(Tile.GetWorldDirectionInt(Direction));
		}

		FIntVector Min = Tile.Id;
		FIntVector Max = Tile.Id;

		for (const FIntVector Direction : WorldDirections)
		{
			FGridTile Next = Tile;
			
			while(true)
			{
				FIntVector NextId = Next.GetNeighbourId(Direction);
				if(!TilesMap.Contains(NextId))
				{
					break;
				}
				
				Next = TilesMap[NextId];
				if(!Next.IsStraight())
				{
					break;
				}

				if(!Next.ContainsAnyDirection(WorldDirections))
				{
					break;
				}

				Min = FGridMath::GetMin(Min, NextId);
				Max = FGridMath::GetMax(Max, NextId);
			}
		}

		FGridLine Line = FGridLine(Min, Max);
		if(Line.Num() > 1)
		{
			Lines.Add(Line);
		}
		
		TileDirections.Reset();
		WorldDirections.Reset();
	}

	UE_LOG(LogTemp, Display, TEXT("Road lines count: %d"), Lines.Num())
}
