// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridTilesSet.h"

#include "GridLine.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FGridLine : public FGridTilesSet
{
	GENERATED_BODY()

	FORCEINLINE bool BeginsWith(const FIntVector& TileId)
	{
		if(IsEmpty())
		{
			return false;
		}

		if(TileId == Items[0])
		{
			return true;
		}

		if(TileId == Items.Last())
		{
			return true;
		}

		return false;
	}
};
