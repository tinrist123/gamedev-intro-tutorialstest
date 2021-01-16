#pragma once
#include "Ground.h"
//#include "QuestionBrick.h"
//#include "Ground.h"
//#include "ColorBox.h"
//#include "Pipe.h"
#include "EffectPoint.h"

#define ITEM_BBOX_WIDHT                 16  
#define ITEM_BBOX_HEIGHT                16  

#define ITEM_COIN                       1   
#define SPECIAL_ITEM                    2

#define ITEM_ANI_MUSHROOM               1
#define ITEM_ANI_LEAF_RIGHT             2
#define ITEM_ANI_LEAF_LEFT              3
#define ITEM_ANI_COIN                   4
#define ITEM_ANI_COIN_IDLE              7
#define ITEM_ANI_MUSHROOM_END_GAME      8
#define ITEM_ANI_FLOWER_END_GAME        9
#define ITEM_ANI_STAR_END_GAME          10
#define ITEM_ANI_RANDOMIZE_END_GAME     11
#define ITEM_ANI_GREEN_MUSHROOM         12
#define ITEM_ANI_IDLE_COIN              13

#define ITEM_STATE_SPECIAL_ITEM         1
#define ITEM_STATE_HITTING_MARIO        2
#define ITEM_STATE_LEAF_FALL            3
#define ITEM_STATE_COIN_JUMP            4
#define ITEM_STATE_MOVE_UP              5
#define ITEM_STATE_IDLE_COIN            6


#define GRAVITY                          0.0004f

#define HEIGHT_MUSHROOM_UP               20
#define MUSHROOM_SPEED_Y                 0.01f
#define LEAF_SPEED_Y                     0.2f
#define LEAF_SPEED_X                     0.05f
#define MOVE_UP_SPEED                    0.1f

#define START_SPEED_Y                    0


class Item;
typedef Item* LPITEM;

class Item :
    public CGameObject
{
public:
    EffectPoint* pointEff;

    bool tested = false;
    bool isGrowthUp = false;

    int start_x;
    int start_y;

    int typeItem;
    int kindAni;

    Item();
    void SetState(int state);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    void addPointToItem();
    void setAnimation(int ani) { this->ani = ani; }
};

