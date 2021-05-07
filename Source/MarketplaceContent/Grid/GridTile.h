// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chaos/AABBTree.h"
#include "UObject/NoExportTypes.h"
#include "EDirections.h"
#include "FGridMath.h"

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

	EDirections LocalDirections;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntVector ActorRotation;

	UPROPERTY(VisibleAnywhere)
	AActor* Actor;

	FIntVector GetNeighbourId(const EDirections Directions) const;

	FIntVector GetNeighbourId(const FIntVector Direction) const
	{
		return FIntVector(Id.X + Direction.X, Id.Y + Direction.Y, Id.Z);
	}

	FORCEINLINE void GetLocalDirectionsSplit(TArray<EDirections>& Array) const
	{
		SplitDirections(LocalDirections, Array);
	}

	FORCEINLINE FVector GetWorldDirection(const EDirections Direction) const
	{
		const FIntVector Dir = GetWorldDirectionInt(Direction);
		return FVector(Dir.X, Dir.Y, Dir.Z);
	}

	FORCEINLINE bool IsStraight() const
	{
		return LocalDirections == EDirections::X
			|| LocalDirections == EDirections::Y;
	}

	FORCEINLINE bool ContainsDirection(const FIntVector Direction) const
	{
#define CHECK(sym) (IncludesDirection(LocalDirections, sym) && Direction == GetWorldDirectionInt(sym)) 
		
		if(CHECK(EDirections::XNeg)
			|| CHECK(EDirections::XPos)
			|| CHECK(EDirections::YNeg)
			|| CHECK(EDirections::YPos))
		{
			return true;
		}

		return false;

#undef CHECK
	}

	FORCEINLINE bool ContainsAnyDirection(const TArray<FIntVector> Directions) const
	{
		for (FIntVector Direction : Directions)
		{
			if(ContainsDirection(Direction))
			{
				return true;
			}
		}

		return false;
	}

	FORCEINLINE FIntVector GetWorldDirectionInt(const EDirections Direction) const
	{
		switch (Direction)
		{
		case EDirections::XPos:
			return FGridMath::Negate(ActorRotation);
		case EDirections::XNeg:
			return ActorRotation;
		case EDirections::YPos:
			return FGridMath::Rotate(ActorRotation, false);
		case EDirections::YNeg:
			return FGridMath::Rotate(ActorRotation);
		default:
			return FIntVector(0,0,0);
		}
	}

	static bool IsAllDirections(const FGridTile& Tile)
	{
		return Tile.LocalDirections == EDirections::All; 
	}
};


