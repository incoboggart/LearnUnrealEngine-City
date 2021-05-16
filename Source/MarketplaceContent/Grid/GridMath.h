#pragma once

class FGridMath
{
public:
	/*
	 * Negate all components of FIntVector
	 */
	static FORCEINLINE FIntVector N(const FIntVector Vector)
	{
		return FIntVector(-Vector.X, -Vector.Y, -Vector.Z);
	}

	/*
	 * Convert to FVector
	 */
	static FORCEINLINE FVector F(const FIntVector I)
	{
		return FVector(I.X, I.Y, I.Z);
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

	static FORCEINLINE FIntVector Min(const FIntVector P1, const FIntVector P2)
	{
#define MIN(v) FMath::Min(P1.v, P2.v)

		return FIntVector(MIN(X), MIN(Y), MIN(Z));

#undef MIN
	}
	
	static FORCEINLINE FIntVector Max(const FIntVector P1, const FIntVector P2)
	{
#define MAX(v) FMath::Max(P1.v, P2.v)

		return FIntVector(MAX(X), MAX(Y), MAX(Z));

#undef MAX
	}

	static FORCEINLINE FIntVector Abs(const FIntVector V)
	{
#define ABS(v) FMath::Abs(V.v)

		return FIntVector(ABS(X), ABS(Y), ABS(Z));

#undef ABS
	}

	static FORCEINLINE FIntVector Rotate90(const FIntVector Vector, bool IsClockwise = true)
	{
		if(IsClockwise)
		{
			return FIntVector(Vector.Y, -Vector.X, Vector.Z);
		}

		return FIntVector(-Vector.Y, Vector.X, Vector.Z);
	}

	static FORCEINLINE FVector Rotate90(const FVector Vector, bool IsClockwise = true)
	{
		if(IsClockwise)
		{
			return FVector(Vector.Y, -Vector.X, Vector.Z);
		}

		return FVector(-Vector.Y, Vector.X, Vector.Z);
	}

	static FORCEINLINE FIntVector Rotate(const FIntVector Vec, const int32 TileRotation)
	{
		int32 Sign = FMath::Sign(TileRotation);
		FIntVector Result = Vec;
		for(int i = 0, count = FMath::Abs(TileRotation); i < count; i++)
		{
			Result = Rotate90(Result, Sign < 0);
		}

		return Result;
	}

	static FORCEINLINE int32 TileRotation(const FRotator ActorRotation)
	{
		return FMath::RoundToInt(ActorRotation.Yaw)/90;
	}

	static FORCEINLINE FRotator ActorRotation(const int32 TileRotation)
	{
		return FRotator(0,TileRotation*90,0);
	}
};

union FIntVectorPacker
{
	struct FComponents
	{
		int16 X:16;
		int16 Y:16;
		int16 Z:16;
		int16 V:16;
	};

	uint64 Packed;
	FComponents Unpacked;

	static FORCEINLINE uint64 Pack(const FIntVector Vector)
	{
		FComponents Unpacked;
		Unpacked.X = Vector.X;
		Unpacked.Y = Vector.Y;
		Unpacked.Z = Vector.Z;
		Unpacked.V = 0;
		
		FIntVectorPacker P;
		P.Unpacked = Unpacked;
		return P.Packed;	
	}
    
	static FORCEINLINE FIntVector Unpack(const uint64 Packed)
	{
		FIntVectorPacker P;
		P.Packed = Packed;
		
		FComponents Unpacked = P.Unpacked;
		return FIntVector(Unpacked.X, Unpacked.Y, Unpacked.Z);
	}
};
