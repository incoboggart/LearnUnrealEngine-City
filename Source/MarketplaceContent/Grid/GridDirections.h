// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GridMath.h"

/**
 * 
 */
class MARKETPLACECONTENT_API FGridDirections
{
public:
	static FORCEINLINE TArray<FIntVector> X()
	{
		return {FIntVector(1,0,0), FIntVector(-1,0,0)};
	}

	static FORCEINLINE TArray<FIntVector> Y()
	{
		return {FIntVector(0,1,0), FIntVector(0,-1,0)};
	}

	static FORCEINLINE TArray<FIntVector> CrossNormal()
	{
		return {FIntVector(1,0,0), FIntVector(-1,0,0), FIntVector(0,1,0), FIntVector(0,-1,0)};
	}

	static FORCEINLINE TArray<FIntVector> All()
	{
		return
		{
			FIntVector( 1, 0,0),
			FIntVector(-1, 0,0),
			FIntVector( 0, 1,0),
			FIntVector( 0,-1,0),
			FIntVector( 1, 1,0),
			FIntVector( 1,-1,0),
			FIntVector(-1,-1,0),
			FIntVector(-1, 1,0),
		}; 
	}

	static FORCEINLINE bool IsEmpty(const TArray<FIntVector>& Directions)
	{
		return Directions.Num() == 0;
	} 

	static FORCEINLINE bool IsDeadEnd(const TArray<FIntVector>& Directions)
	{
		return Directions.Num() == 1;
	}

	static FORCEINLINE bool IsStraight(const TArray<FIntVector>& Directions)
	{		
		return Directions.Num() == 2
			&& Directions[0] == FGridMath::N(Directions[1]);
	}

	static FORCEINLINE bool IsOrthoTurn(const TArray<FIntVector>& Directions)
	{
		const int32 Num = Directions.Num();		
		for(int32 i = 0; i < Num; i++)
		{
			const FIntVector One = FGridMath::Abs(Directions[i]);
			for(int32 j = 0; j < Num; j++)
			{
				if(i == j)
				{
					continue;
				}

				const FIntVector Two = FGridMath::Abs(Directions[j]);
				if(One.X != Two.X
					|| One.Y != Two.Y)
				{
					return true;
				}
			}
		}

		return false;
	}

	static FORCEINLINE bool IsOrthoCross(const TArray<FIntVector>& Directions)
	{
		TArray<FIntVector> Origin = CrossNormal();
		if(Directions.Num() != Origin.Num())
		{
			return false;
		}

		for (FIntVector Dir : Origin)
		{
			if(!Directions.Contains(Dir))
			{
				return false;
			}
		}

		return true;
	}

	static FORCEINLINE bool IsOrthoDirection(const FIntVector& Direction)
	{
		return (Direction.X + Direction.Y) == 1;
	}
};
