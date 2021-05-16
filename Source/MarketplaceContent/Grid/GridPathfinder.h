// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridPathfinderCost.h"
#include "GridPathfinderNode.h"
#include "MarketplaceContent.h"
#include "TilesGrid.h"
#include "GridPathfinder.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FGridPathfinder
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
    FIntVector Start;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
    FIntVector Finish;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
    int32 OrthCost = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
    int32 DiagCost = 14;

    bool FindPath(FTilesGrid& Grid, TArray<FIntVector>& OutPath) const
    {
        if(!Grid.ContainsTile(Start)
            || !Grid.ContainsTile(Finish))
        {
            return false;
        }

        auto WorldDirections = TArray<FIntVector>();
        
        TArray<FIntVector> Open = TArray<FIntVector>();
        TSet<FIntVector> Closed = TSet<FIntVector>();
        TMap<FIntVector, FGridPathfinderNode> Nodes = TMap<FIntVector, FGridPathfinderNode>();

        int32 MaxIterations = 10000;
        int32 Iterations = 0;
        
        Open.Add(Start);
        Nodes.Add(Start, FGridPathfinderNode(Start, Start, EvaluateCost(Start)));

        UE_LOG(LogTilesGrid, Log, TEXT("Search path from %s to %s"), *Start.ToString(), *Finish.ToString());
        
        while (Open.Num() != 0
            && Iterations < MaxIterations)
        {
            int32 MinCostTileIndex = FindLowestCostTileIndex(Open, Nodes);
            FIntVector CurrentTileId = Open[MinCostTileIndex];

            UE_LOG(LogTilesGrid, Log, TEXT("Evaluate tile %s"), *CurrentTileId.ToString());

            if(CurrentTileId == Finish)
            {
                UE_LOG(LogTilesGrid, Log, TEXT("Found path from %s to %s"), *Start.ToString(), *Finish.ToString());

                EmplacePath(Nodes, OutPath);
                    
                return true;
            }

            Open.RemoveAt(MinCostTileIndex);
            Closed.Emplace(CurrentTileId);

            if(!Grid.ContainsTile(CurrentTileId))
            {
                UE_LOG(LogTilesGrid, Error, TEXT("Grid have no tile %s"), *CurrentTileId.ToString());

                break;
            }

            auto Tile = Grid.GetTile(CurrentTileId);
            Tile.GetWorldDirections(WorldDirections, true);            
            for (auto WorldDirection : WorldDirections)
            {                
                FIntVector NeighbourId = CurrentTileId + WorldDirection;

                UE_LOG(LogTilesGrid, Log, TEXT("Evaluate Neighbour %s in direction %s"), *NeighbourId.ToString(), *WorldDirection.ToString());

                if(!Grid.ContainsTile(NeighbourId))
                {
                    UE_LOG(LogTilesGrid, Warning, TEXT("Grid have no tile %s"), *NeighbourId.ToString());

                    if(Tile.IsOrthoTurn())
                    {
                        UE_LOG(LogTilesGrid, Warning, TEXT("Grid have no tile %s wd %s"), *NeighbourId.ToString(), *WorldDirection.ToString());
                    }
                    
                    continue;
                }

                bool IsClosed = Closed.Contains(NeighbourId);
                if(!IsClosed)
                {
                    bool IsOpen = Open.Contains(NeighbourId);

                    FGridPathfinderCost Cost = EvaluateCost(NeighbourId);
                    if(!IsOpen)
                    {
                        FGridPathfinderNode Node = FGridPathfinderNode(NeighbourId, CurrentTileId, Cost);
                        Nodes.Emplace(NeighbourId, Node);

                        Open.Add(NeighbourId);
                    }
                    else
                    {
                        FGridPathfinderNode Node = Nodes[NeighbourId];
                        if(Node.Cost.FCost() > Cost.FCost())
                        {
                            Node = FGridPathfinderNode(NeighbourId, CurrentTileId, Cost);
                            
                            Nodes.Emplace(NeighbourId, Node);
                        }                        
                    }
                }
            }

            Iterations++;
        }

        UE_LOG(LogTilesGrid, Error, TEXT("Failed to find path from %s to %s"), *Start.ToString(), *Finish.ToString());
        
        return false;
    }

private:
    int32 EvaluateDistance(const FIntVector From, const FIntVector To) const
    {
        const FIntVector Delta = FGridMath::Abs(To - From);
        const int32 DiagSteps = FMath::Min(Delta.X, Delta.Y);
        const int32 OrthoSteps = Delta.X + Delta.Y - DiagSteps*2;
        return DiagSteps*DiagCost + OrthoSteps*OrthCost;
    }

    FGridPathfinderCost EvaluateCost(const FIntVector TileId) const
    {
        const int32 GCost = EvaluateDistance(Start, TileId);
        const int32 HCost = EvaluateDistance(Finish, TileId);
        return FGridPathfinderCost(GCost, HCost);
    }

    void EmplacePath(TMap<FIntVector, FGridPathfinderNode>& Nodes, TArray<FIntVector>& OutPath) const
    {
        OutPath.Reset();

        FGridPathfinderNode Node = Nodes[Finish];
        while (Node.TileId != Start)
        {
            OutPath.Add(Node.TileId);

            Node = Nodes[Node.ParentTileId];
        }
    }

    static int32 FindLowestCostTileIndex(TArray<FIntVector>& Open, TMap<FIntVector, FGridPathfinderNode>& Nodes)
    {
        int32 MinTileIndex = 0;
        FIntVector MinTileId = Open[MinTileIndex];
        FGridPathfinderNode MinNode = Nodes[MinTileId];
        
        for(int32 i = 1; i < Open.Num(); i++)
        {
            FIntVector TileId = Open[i];
            FGridPathfinderNode Node = Nodes[TileId];

            if(Node.Cost.FCost() < MinNode.Cost.FCost())
            {
                MinTileIndex = i;
                MinTileId = TileId;
                MinNode = Node;
            }
        }

        return MinTileIndex;
    }
};
