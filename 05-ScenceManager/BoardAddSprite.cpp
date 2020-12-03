#include "BoardAddSprite.h"

BoardAddSprite::BoardAddSprite()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(12);
	SetAnimationSet(ani_set);
}
