// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "TilesGridBuilder.h"
#include "GridJunction.h"
#include "GridLine.h"
#include "GridTile.h"
#include "SplineActor.h"
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
	
public:	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawTileDirectionsEnabled;

	UPROPERTY(EditAnywhere, Category="Debug")
	FColor DrawTileDirectionsColor;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawJunctionsEnabled;

	UPROPERTY(EditAnywhere, Category="Debug")
	FColor DrawJunctionsColor;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawLinesEnabled;

	

	UPROPERTY(EditAnywhere, Category="Debug")
	FColor DrawLinesColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid")
	FIntVector TileSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
	FTilesGridBuilder Grid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug")
	FIntVector TestRouteStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug")
	FIntVector TestRouteFinish;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	TArray<FIntVector> TestRoute;

	UPROPERTY(EditAnywhere, Category="Debug")
	UClass* SplineActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug")
	ASplineActor* TestSplineInstance;

	ARoadsGameModeBase();

	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:	
	void DrawTileDirections() const;
	void DrawJunctions() const;
	void DrawLines() const;
	void BuildTestRoute();
};


