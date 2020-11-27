#pragma once
#include "Ground.h"
#include "EffectPoint.h"
//#include "QuestionBrick.h"
//#include "Ground.h"
//#include "ColorBox.h"
//#include "Pipe.h"

#define ITEM_BBOX_WIDHT     16  
#define ITEM_BBOX_HEIGHT    16  

#define ITEM_COIN           1   
#define SPECIAL_ITEM        2

#define ITEM_ANI_MUSHROOM   1
#define ITEM_ANI_LEAF       2
#define ITEM_ANI_COIN       3

#define ITEM_STATE_SPECIAL_ITEM     1
#define ITEM_STATE_HITTING_MARIO    2
#define ITEM_STATE_LEAF_FALL        3
#define ITEM_STATE_COIN_JUMP        4
#define ITEM_IDLE                   5


#define GRAVITY                          0.0004f

#define HEIGHT_MUSHROOM_UP               20
#define MUSHROOM_SPEED_X                 0.1f
#define MUSHROOM_SPEED_Y                 0.1f

#define START_SPEED_Y                    0


class Item;
typedef Item* LPITEM;

class Item :
    public CGameObject
{
public:
    bool tested = false;
    bool isGrowthUp = false;

    int start_x;
    int start_y;

    int typeItem;
    int kindAni;
    EffectPoint* pointEff;

    Item(int typeitem,int kindAni,int ,int );
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void SetState(int state);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    void addPointToItem();
};

