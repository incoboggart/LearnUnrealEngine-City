// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum class EDirections
{
	None = 0,
    XPos = 1 << 0,
    XNeg = 1 << 1,
    YPos = 1 << 2,
    YNeg = 1 << 3,
    All = XPos|XNeg|YNeg|YPos,
	X = XNeg|XPos,
	Y = YNeg|YPos,
	PP = XPos|YPos,
	PN = XPos|YNeg,
	NN = XNeg|YNeg,
	NP = XNeg|YPos
};

FORCEINLINE bool IncludesDirection(EDirections Value, EDirections Mask)
{
	return (static_cast<int32>(Value) & static_cast<int32>(Mask)) != 0;
}

FORCEINLINE bool ExcludesDirection(EDirections Value, EDirections Mask)
{
	return (static_cast<int32>(Value) & static_cast<int32>(Mask)) == 0;
}

FORCEINLINE void SplitDirections(const EDirections Value, TArray<EDirections>& Array)
{
	Array.Reset();
	const int32 source = static_cast<int32>(Value);

#define TEST(symbol) ((source&static_cast<int32>(symbol)) != 0)

	if(TEST(EDirections::XNeg))
	{
		Array.Add(EDirections::XNeg);
	}

	if(TEST(EDirections::XPos))
	{
		Array.Add(EDirections::XPos);
	}

	if(TEST(EDirections::YNeg))
	{
		Array.Add(EDirections::YNeg);
	}

	if(TEST(EDirections::YPos))
	{
		Array.Add(EDirections::YPos);
	}

#undef TEST
}

