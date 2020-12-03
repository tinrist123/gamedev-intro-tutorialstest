#pragma once
#include "Item.h"

class Leaf : public Item
{
public:
    Leaf(float posX,float posY);
    ~Leaf();
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

    float LeftLimit;
    float RightLimit;
};

