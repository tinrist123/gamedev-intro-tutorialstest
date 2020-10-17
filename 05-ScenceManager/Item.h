#pragma once
#include "GameObject.h"
//#include "QuestionBrick.h"
//#include "Ground.h"
//#include "ColorBox.h"
//#include "Pipe.h"

#define ITEM_COIN           1   
#define SPECIAL_ITEM        2   

class Item :
    public CGameObject
{
public:
    int width;
    int height;

    int start_x;
    int start_y;

    int typeItem;

    Item(int width, int height, int typeitem, int x, int y);
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void SetState(int state);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

