#include "Item.h"


Item::Item()
{
	this->category = Category::ITEM;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(5);
	SetAnimationSet(ani_set);
}





void Item::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state) {
	case ITEM_STATE_SPECIAL_ITEM:
		vy = -MUSHROOM_SPEED_Y;
		break;
	case ITEM_STATE_HITTING_MARIO:
		setObjDisappear();
		break;
	case ITEM_STATE_COIN_JUMP:
		vy = -0.3f;
		break;
	case ITEM_STATE_LEAF_FALL:
		vy = -LEAF_SPEED_Y;
		vx = LEAF_SPEED_X;
		break;
	default:
		break;
	}
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	
}

void Item::addPointToItem()
{
	pointEff = new EffectPoint();
	pointEff->SetPosition(this->x, this->y);
}


