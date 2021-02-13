// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridTilesSet.h"
#include "GridJunction.generated.h"

/**
 * 
 */
USTRUCT()
struct MARKETPLACECONTENT_API FGridJunction : public FGridTilesSet
{
	GENERATED_BODY()

	FORCEINLINE bool IsSingleItem() const
	{
		return Items.Num() == 1;
	}

	FORCEINLINE bool IsLine() const
	{
		if(IsEmpty()
			|| IsSingleItem())
		{
			return false;
		}
		
		FIntVector One = Items[0];
		FIntVector Two = Items[1];
		FIntVector Dir = Two - One;
		Dir.Z = 0;
				
		for(int i = 1, m = Items.Num() - 1; i < m; i++)
		{
			One = Items[i];
			Two = Items[i + 1];
			FIntVector Dir2 = Two - One;
			Dir2.Z = 0;

			if(Dir != Dir2)
			{
				return false;
			}
		}

		return true;
	}
};
