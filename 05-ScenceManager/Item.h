#pragma once
#include "GameObject.h"
//#include "QuestionBrick.h"
//#include "Ground.h"
//#include "ColorBox.h"
//#include "Pipe.h"

#define ITEM_BBOX_WIDHT     16  
#define ITEM_BBOX_HEIGHT    16  

#define ITEM_COIN           1   
#define SPECIAL_ITEM        2

#define ITEM_ANI_COIN       0
#define ITEM_ANI_MUSHROOM   1
#define ITEM_ANI_LEAF       2

#define GRAVITY                          0.0006f

#define HEIGHT_MUSHROOM_UP               20
#define MUSHROOM_SPEED_X                 0.1f
#define MUSHROOM_SPEED_Y                 0.3f

#define START_SPEED_Y                    0

class Item :
    public CGameObject
{
public:
    bool tested = false;
    bool isGrowthUp = false;

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

