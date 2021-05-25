// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chaos/AABBTree.h"
#include "UObject/NoExportTypes.h"

#include "GridMath.h"
#include "GridDirections.h"

#include "GridTile.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FGridTile 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntVector Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Rotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FIntVector> LocalDirections;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector ActorRotation;

	UPROPERTY(VisibleAnywhere)
	AActor* Actor;

	FORCEINLINE FRotator GetActorRotation() const
	{
		return FGridMath::ActorRotation(Rotation);
	}

	FIntVector GetNeighbourId(const FIntVector Direction) const
	{
		return FIntVector(Id.X + Direction.X, Id.Y + Direction.Y, Id.Z);
	}

	FORCEINLINE bool IsStraightLine() const
	{
		return FGridDirections::IsStraight(LocalDirections);
	}

	FORCEINLINE bool IsOrthoTurn() const
	{
		return FGridDirections::IsOrthoTurn(LocalDirections);
	}
	
	FORCEINLINE bool IsOrthoCross() const
	{
		return FGridDirections::IsOrthoCross(LocalDirections);
	}

	FORCEINLINE bool ContainsLocalDirection(const FIntVector LocalDirection) const
	{
		return LocalDirections.Contains(LocalDirection);
	}

	FORCEINLINE bool ContainsAnyLocalDirection(const TArray<FIntVector>& WorldDirections) const
	{
		for (FIntVector Direction : WorldDirections)
		{
			if(ContainsLocalDirection(Direction))
			{
				return true;
			}
		}

		return false;
	}

	FORCEINLINE bool ContainsWorldDirection(const FIntVector WorldDirection) const
	{
		return ContainsLocalDirection(GetLocalDirection(WorldDirection));
	}

	FORCEINLINE bool ContainsAnyWorldDirection(const TArray<FIntVector>& WorldDirections) const
	{
		for (const FIntVector WorldDirection : WorldDirections)
		{
			if(ContainsWorldDirection(WorldDirection))
			{
				return true;
			}
		}

		return false;
	}

	FORCEINLINE void GetWorldDirections(TArray<FIntVector>& Directions, const bool ResetArray = false) const
	{
		if(ResetArray)
		{
			Directions.Reset();
		}
		
		for (const FIntVector LocalDirection : LocalDirections)
		{
			Directions.Add(GetWorldDirection(LocalDirection));
		}
	}

	FORCEINLINE FIntVector GetWorldDirection(const FIntVector LocalDirection) const
	{
		return FGridMath::Rotate(LocalDirection, Rotation);
	}

	FORCEINLINE FIntVector GetLocalDirection(const FIntVector WorldDirection) const
	{
		return FGridMath::Rotate(WorldDirection, -Rotation);
	}
	
	

	FORCEINLINE void SetWorldDirections(...)
	{
		
	}
};


