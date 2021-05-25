// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GridTile.h"
#include "TilesGridBuilder.h"

#include "TilesGrid.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MARKETPLACECONTENT_API UTilesGrid final : public UObject
{
    GENERATED_BODY()

    FIntVector TileSize;
    TMap<FIntVector, FGridTile> Tiles;
    
    public:

    void SetupWith(FTilesGridBuilder& GridBuilder)
    {
        TileSize = GridBuilder.TileSize;
        Tiles = GridBuilder.GetTilesMap();
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    const FGridTile& GetTile(const FIntVector TileId) { return Tiles[TileId]; }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FVector GetTileLocation(const FIntVector TileId) const { return FGridMath::ToTileCenterLocation(TileId, TileSize); }
};
