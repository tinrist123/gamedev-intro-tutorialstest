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

#define ITEM_ANI_MUSHROOM         1
#define ITEM_ANI_LEAF_RIGHT       2
#define ITEM_ANI_LEAF_LEFT        3
#define ITEM_ANI_COIN             4

#define ITEM_STATE_SPECIAL_ITEM     1
#define ITEM_STATE_HITTING_MARIO    2
#define ITEM_STATE_LEAF_FALL        3
#define ITEM_STATE_COIN_JUMP        4


#define GRAVITY                          0.0004f

#define HEIGHT_MUSHROOM_UP               20
#define MUSHROOM_SPEED_Y                 0.01f
#define LEAF_SPEED_Y                     0.2f
#define LEAF_SPEED_X                     0.05f

#define START_SPEED_Y                    0


class Item;
typedef Item* LPITEM;

class Item :
    public CGameObject
{
public:
    bool tested = false;
    bool isGrowthUp = false;
    bool isTouchable = false;

    int start_x;
    int start_y;

    int typeItem;
    int kindAni;
    EffectPoint* pointEff;

    Item();
    void SetState(int state);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    void addPointToItem();
};

