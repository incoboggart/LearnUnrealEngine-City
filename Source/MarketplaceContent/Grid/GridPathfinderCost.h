#pragma once

#include "GridPathfinderCost.generated.h"

USTRUCT()
struct MARKETPLACECONTENT_API FGridPathfinderCost
{
    GENERATED_BODY()

    FGridPathfinderCost()
        : G(0), F(0)
    {
    }

    FGridPathfinderCost(const int32 GCost, const int32 HCost)
        : G(GCost)
    {
        F = GCost + HCost;
    }
    
    FORCEINLINE int32 GCost() const
    {
        return G;
    }

    FORCEINLINE int32 HCost() const
    {
        return F - G;
    }

    FORCEINLINE int32 FCost() const
    {
        return F;
    }

private:
    int32 G, F;   

public:
    bool operator<(FGridPathfinderCost const& Other) const
    {
        return F < Other.F;
    }
    
    bool operator<=(FGridPathfinderCost const& Other) const
    {
        return F <= Other.F;
    }
    
    bool operator>(FGridPathfinderCost const& Other) const
    {
        return F > Other.F;
    }
    
    bool operator>=(FGridPathfinderCost const& Other) const
    {
        return F >= Other.F;
    }
};
