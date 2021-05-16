// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

UCLASS()
class MARKETPLACECONTENT_API ASplineActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASplineActor();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USplineComponent* SplineComponent;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE void PopulatePoints(TArray<FVector>& Points)
    {
        SplineComponent->SetSplineWorldPoints(Points);
        SplineComponent->UpdateSpline();
    }

    UFUNCTION(BlueprintImplementableEvent)
    void SetActor(AActor* Actor);
};
