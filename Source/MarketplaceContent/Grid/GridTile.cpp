// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"

#include "EDirections.h"

FIntVector FGridTile::GetNeighbourId(const EDirections Direction) const
{
	if(Direction == EDirections::None)
	{
		return Id;
	}		
	
	FIntVector Result = FIntVector(0,0,0);
	if(IncludesDirection(Direction, EDirections::XNeg))
	{
		Result.X = -1;
	}
	else if(IncludesDirection(Direction, EDirections::XPos))
	{
		Result.X = 1;
	}

	if(IncludesDirection(Direction, EDirections::YNeg))
	{
		Result.Y = -1;
	}
	else if(IncludesDirection(Direction, EDirections::YPos))
	{
		Result.Y = 1;
	}

	return Id + Result;
}
