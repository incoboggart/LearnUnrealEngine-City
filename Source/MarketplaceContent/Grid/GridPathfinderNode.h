// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridPathfinderCost.h"

#include "GridPathfinderNode.generated.h" 

/**
 * 
 */
USTRUCT()
struct MARKETPLACECONTENT_API FGridPathfinderNode
{
    GENERATED_BODY()
    
    FIntVector TileId;
    FIntVector ParentTileId;
    FGridPathfinderCost Cost;
    int32 PathLength;

    FGridPathfinderNode()
        : TileId(FIntVector()), ParentTileId(FIntVector()), Cost(FGridPathfinderCost()), PathLength(0)        
    {
        
    }

    FGridPathfinderNode(FIntVector TileId, FIntVector ParentId, FGridPathfinderCost Cost, int32 PathLength = 0)
        : TileId(TileId), ParentTileId(ParentId), Cost(Cost), PathLength(PathLength)        
    {
        
    }
};
