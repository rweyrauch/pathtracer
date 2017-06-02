//
// Created by rick on 5/21/17.
//

#include "World.h"

HitableInfo* BuildWorld(Hitable* world)
{
    if (world == nullptr) return nullptr;

    // Count the total number of Hitables in world.
    int numHitables = world->numChildren();

    HitableInfo* worldTable = new HitableInfo[numHitables];

    return worldTable;
}
