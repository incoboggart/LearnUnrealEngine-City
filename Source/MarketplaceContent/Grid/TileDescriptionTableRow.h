// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "TileDescriptionTableRow.generated.h"

UENUM(BlueprintType, meta = (BitFlags))
enum ETileLayerType
{
	None			= 0x00,
	
    PedestrainsRoad = 0x01,
    CarsRoad		= 0x02
};

USTRUCT(BlueprintType)
struct MARKETPLACECONTENT_API FTileDescriptionTableRow 
    : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName TechnicalName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName VisualName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<ETileLayerType> Layer;
};
