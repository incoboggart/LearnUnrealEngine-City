#pragma once

class FGridMath
{
public:
	static FORCEINLINE FIntVector Rotate(const FIntVector Vector, bool IsClockwise = true)
	{
		if(IsClockwise)
		{
			return FIntVector(Vector.Y, -Vector.X, Vector.Z);
		}

		return FIntVector(-Vector.Y, Vector.X, Vector.Z);
	}

	static FORCEINLINE FVector Rotate(const FVector Vector, bool IsClockwise = true)
	{
		if(IsClockwise)
		{
			return FVector(Vector.Y, -Vector.X, Vector.Z);
		}

		return FVector(-Vector.Y, Vector.X, Vector.Z);
	}

	static FORCEINLINE FIntVector Negate(const FIntVector Vector)
	{
		return FIntVector(-Vector.X, -Vector.Y, -Vector.Z);
	}

	static FORCEINLINE FIntVector RoundToInt(const FVector Vector)
	{
		return FIntVector(
			FMath::RoundToInt(Vector.X),
			FMath::RoundToInt(Vector.Y),
			FMath::RoundToInt(Vector.Z));
	}

	static FORCEINLINE FIntVector ToTileId(const FVector ActorCenter, const FIntVector GridSize)
	{
#define CALC(component) ((FMath::RoundToInt(ActorCenter.component) - GridSize.component / 2) / GridSize.component)
				
		return FIntVector(CALC(X), CALC(Y), FMath::RoundToInt(ActorCenter.Z) / GridSize.Z);

#undef  CALC
	}	

	static FORCEINLINE FIntVector ToTileId(const FIntVector TileCenter, const FIntVector GridSize)
	{
#define CALC(component) ((TileCenter.component - GridSize.component / 2) / GridSize.component)
				
		return FIntVector(CALC(X), CALC(Y), TileCenter.Z / GridSize.Z);

#undef  CALC
	}

	static FORCEINLINE FIntVector ToTileCenter(const FIntVector TileId, const FIntVector GridSize)
	{
#define CALC(component) ((TileId.component * GridSize.component) + GridSize.component / 2)
		
		return  FIntVector(CALC(X), CALC(Y), TileId.Z * GridSize.Z);

#undef CALC
	}

	static FORCEINLINE FVector ToTileCenterLocation(const FIntVector TileId, const FIntVector GridSize, const float MulZ = 1.f)
	{
#define CALC(component) ((TileId.component * GridSize.component) + GridSize.component / 2)
		
		return  FVector(CALC(X), CALC(Y), TileId.Z * GridSize.Z * MulZ);

#undef CALC
	}

	static FORCEINLINE FIntVector GetMin(const FIntVector P1, const FIntVector P2)
	{
#define MIN(v) FMath::Min(P1.v, P2.v)

		return FIntVector(MIN(X), MIN(Y), MIN(Z));

#undef MIN
	}
	
	static FORCEINLINE FIntVector GetMax(const FIntVector P1, const FIntVector P2)
	{
#define MAX(v) FMath::Max(P1.v, P2.v)

		return FIntVector(MAX(X), MAX(Y), MAX(Z));

#undef MAX
	}
};
