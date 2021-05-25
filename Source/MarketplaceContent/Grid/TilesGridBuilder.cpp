#include "TilesGridBuilder.h"

#include "EngineUtils.h"
#include "GridDirections.h"
#include "Engine/StaticMeshActor.h"

#include "MarketplaceContent.h"

FTilesGridBuilder::FTilesGridBuilder()
{
	TileById = TMap<FIntVector, FGridTile>();

	Junctions = TArray<FGridJunction>();
	Lines = TArray<FGridLine>();
}

void FTilesGridBuilder::BuildGrid(UWorld* World)
{
	CreateTiles(World);
		
	TArray<FGridTile> Tiles = TArray<FGridTile>();
	TileById.GenerateValueArray(Tiles);
	
	CreateJunctions(Tiles);
	SimplifyTileDirections(Tiles);
	CreateLines(Tiles);
	// SimplifyJunctions(Tiles);
}

void FTilesGridBuilder::CreateTiles(UWorld* World)
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

		UE_LOG(LogTilesGrid, Display, TEXT("Road static mesh at %s"), *ActorCenter.ToString())

		FIntVector TileId = FGridMath::ToTileId(ActorCenter, TileSize); 
		if(TileById.Contains(TileId))
		{
			UE_LOG(LogTilesGrid, Warning, TEXT("DUPLICATE %s From %s -> %s"), *TileId.ToString(), *StaticMeshActor->GetName(), *TileById[TileId].Actor->GetName())
		}

		FGridTile Tile = FGridTile();
		Tile.Id = TileId;
		Tile.Actor = StaticMeshActor;
		Tile.ActorRotation = FGridMath::RoundToInt(StaticMeshActor->GetActorRotation().Vector());
		Tile.Rotation = FGridMath::TileRotation(StaticMeshActor->GetActorRotation());
				
		if(ActorName.Contains("bare"))
		{
			Bare.Add(TileId);

			Tile.LocalDirections = FGridDirections::CrossNormal();
		}
		else if(ActorName.Contains("crossing"))
		{
			Crossings.Add(TileId);

			Tile.LocalDirections = FGridDirections::X();
		}
		else
		{
			Tile.LocalDirections = FGridDirections::Y();
		}
				               
		TileById.Add(Tile.Id, Tile);
	}

	UE_LOG(LogTilesGrid, Display, TEXT("Road tiles count: %d"), TileById.Num())

	TArray<FIntVector> StraightCrossDirections = FGridDirections::CrossNormal();
	for (auto It = TileById.CreateIterator(); It; ++It)
	{
		FGridTile& Tile = It.Value();
		if(!Tile.IsStraightLine())
		{
			continue;
		}
		
		for (FIntVector LocalDirection : StraightCrossDirections)
		{
			if(Tile.ContainsLocalDirection(LocalDirection))
			{
				continue;
			}

			FIntVector WorldDirection = Tile.GetWorldDirection(LocalDirection);
			FIntVector NeighbourId = Tile.Id + WorldDirection;
			if(!ContainsTile(NeighbourId))
			{
				continue;
			}

			FGridTile& Neighbour = GetTile(NeighbourId);
			if(Neighbour.ContainsWorldDirection((WorldDirection)))
			{
				Tile.LocalDirections.Add(LocalDirection);
			}
		}
	}
}

void FTilesGridBuilder::CreateJunctions(const TArray<FGridTile>& Tiles)
{	
	TSet<FIntVector> Visited = TSet<FIntVector>();
	TArray<FIntVector> Queue = TArray<FIntVector>();

	const TArray<FIntVector> Directions = FGridDirections::CrossNormal();

	for (FGridTile Tile : Tiles)
	{
		if(!Tile.IsOrthoCross())
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
				
		for (const FIntVector Direction : Directions)
		{
			FIntVector NeighbourId = Tile.GetNeighbourId(Direction);
			if(TileById.Contains(NeighbourId))
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
		
			const FGridTile QueuedTile = TileById[QueuedId];
			if(!QueuedTile.IsOrthoCross())
			{
				continue;
			}
		
			Junction.Add(QueuedTile);
			
			for (const FIntVector Direction : Directions)
			{
				FIntVector NeighbourId = QueuedTile.GetNeighbourId(Direction);
				if(!TileById.Contains(NeighbourId))
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

	UE_LOG(LogTilesGrid, Display, TEXT("Junctions count: %d"), Junctions.Num())
}

void FTilesGridBuilder::SimplifyTileDirections(const TArray<FGridTile>& Tiles)
{
	TArray<FIntVector> StraightCrossDirections = FGridDirections::CrossNormal();
	TArray<FIntVector> FinalDirections = TArray<FIntVector>();
	
	for (auto It = TileById.CreateIterator(); It; ++It)
	{
		FGridTile& Tile = It.Value();
		if(!Tile.IsOrthoCross())
		{
			continue;
		}
		
		for (FIntVector LocalDirection : StraightCrossDirections)
		{
			const FIntVector NextTileId = Tile.Id + Tile.GetWorldDirection(LocalDirection);
			if(!ContainsTile(NextTileId))
			{
				continue;
			}

			FinalDirections.Add((LocalDirection));
		}

		Tile.LocalDirections.Reset();
		Tile.LocalDirections.Append(FinalDirections);

		FinalDirections.Reset();
	}
}

void FTilesGridBuilder::CreateLines(const TArray<FGridTile>& Tiles)
{
	Lines = TArray<FGridLine>();
	
	TSet<FIntVector> Visited = TSet<FIntVector>();
	TArray<FIntVector> Queue = TArray<FIntVector>();
	TArray<FIntVector> WorldDirections = TArray<FIntVector>();

	// Create straight lines from tiles with restricted directions
	for (FGridTile Tile : Tiles)
	{
		if(!Tile.IsStraightLine())
		{
			continue;
		}

		if(Visited.Contains(Tile.Id))
		{
			continue;
		}

		Visited.Add(Tile.Id);

		Tile.GetWorldDirections(WorldDirections);

		FIntVector Min = Tile.Id;
		FIntVector Max = Tile.Id;		

		for (const FIntVector Direction : WorldDirections)
		{
			FGridTile Next = Tile;
			
			while(true)
			{
				FIntVector NextId = Next.GetNeighbourId(Direction);
				if(!TileById.Contains(NextId))
				{
					break;
				}
				
				Next = TileById[NextId];
				if(Next.IsOrthoCross())
				{
					break;
				}
				
				if(!Next.IsStraightLine())
				{
					break;
				}

				if(!Next.ContainsAnyWorldDirection(WorldDirections))
				{
					break;
				}

				Min = FGridMath::Min(Min, NextId);
				Max = FGridMath::Max(Max, NextId);

				Visited.Add(NextId);
			}
		}

		FGridLine Line = FGridLine(Min, Max);
		if(Line.Num() > 1)
		{
			Lines.Add(Line);
		}
		
		WorldDirections.Reset();
	}

	UE_LOG(LogTilesGrid, Display, TEXT("Road lines count: %d"), Lines.Num())
}

void FTilesGridBuilder::SimplifyJunctions(const TArray<FGridTile>& AllTiles)
{
	TArray<FGridTile> Tiles = TArray<FGridTile>();
	TSet<FIntVector> Visited = TSet<FIntVector>();
	TArray<FIntVector> WorldDirections = TArray<FIntVector>();
	
	for (FGridJunction Junction : Junctions)
	{
		if(!Junction.Select(TileById, Tiles))
		{
			continue;
		}

		for (FGridTile Tile : Tiles)
		{
			if(!Tile.IsStraightLine()
				|| Visited.Contains(Tile.Id))
			{
				continue;
			}

			Tile.GetWorldDirections(WorldDirections);

			// check all tiles are in desired directions and no turns. 
		}
	}
}
