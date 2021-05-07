// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "FTilesGrid.h"
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"
#include "Chaos/AABBTree.h"
#include "GameFramework/GameModeBase.h"
#include "RoadsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MARKETPLACECONTENT_API ARoadsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FIntVector GridSize;

	UPROPERTY(EditAnywhere)
	bool DrawTileDirectionsEnabled;

	UPROPERTY(EditAnywhere)
	FColor DrawTileDirectionsColor;

	UPROPERTY(EditAnywhere)
	bool DrawJunctionsEnabled;

	UPROPERTY(EditAnywhere)
	FColor DrawJunctionsColor;

	UPROPERTY(EditAnywhere)
	bool DrawLinesEnabled;

	UPROPERTY(EditAnywhere)
	FColor DrawLinesColor;

	FTilesGrid Grid;

	ARoadsGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

private:
	void DrawTileDirections() const;
	void DrawJunctions() const;
	void DrawLines() const;
};


