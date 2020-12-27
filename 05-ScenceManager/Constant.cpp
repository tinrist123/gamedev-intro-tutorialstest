#include "Constant.h"



Constant::Constant(int level)
{
	this->kindMario = level;
	switch (this->kindMario)
	{
	case MARIO_LEVEL_BIG:
		listAni_Mario_Big = getAnimationMARIO_BIG();
		listBBox_Mario_Big = getBBox_MARIO_BIG();
		break;
	case MARIO_LEVEL_BIG_TAIL:
		listAni_Mario_Big = getAnimationMARIO_BIG_HAVE_TAIL();
		listBBox_Mario_Big = getBBox_MARIO_BIG_HAVE_TAIL();
		break;
	case MARIO_LEVEL_BIG_FIRE:
		listAni_Mario_Big = getAnimationMARIO_BIG_HAVE_FIRE();
		listBBox_Mario_Big = getBBox_MARIO_BIG();
		break;
	default:
		break;
	}
}

void Constant::changeLevelMario(int level)
{
	this->kindMario = level;
	switch (this->kindMario)
	{
	case MARIO_LEVEL_BIG:
		listAni_Mario_Big = getAnimationMARIO_BIG();
		listBBox_Mario_Big = getBBox_MARIO_BIG();
		break;
	case MARIO_LEVEL_BIG_TAIL:
		listAni_Mario_Big = getAnimationMARIO_BIG_HAVE_TAIL();
		listBBox_Mario_Big = getBBox_MARIO_BIG_HAVE_TAIL();
		break;
	case MARIO_LEVEL_BIG_FIRE:
		listAni_Mario_Big = getAnimationMARIO_BIG_HAVE_FIRE();
		listBBox_Mario_Big = getBBox_MARIO_BIG();
		break;
	default:
		break;
	}
}


vector<int> Constant::getBBox_MARIO_BIG_HAVE_TAIL()
{
	listBBox_Mario_Big.clear();
	listBBox_Mario_Big.push_back(MARIO_BIG_TAIL_BBOX_WIDTH);
	listBBox_Mario_Big.push_back(MARIO_BIG_TAIL_BBOX_HEIGHT);
	
	listBBox_Mario_Big.push_back(MARIO_BIG_TAIL_BBOX_SITTING_HEIGHT);

	return listBBox_Mario_Big;
}

vector<int> Constant::getBBox_MARIO_BIG()
{
	listBBox_Mario_Big.clear();

	listBBox_Mario_Big.push_back(MARIO_BIG_BBOX_WIDTH);
	listBBox_Mario_Big.push_back(MARIO_BIG_BBOX_HEIGHT);

	listBBox_Mario_Big.push_back(MARIO_BIG_BBOX_SITTING_HEIGHT);

	return listBBox_Mario_Big;
}

vector<int> Constant::getAnimationMARIO_BIG_HAVE_TAIL()
{
	listAni_Mario_Big.clear();
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_SITTING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_IDLE_RIGHT);
								  
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_SITTING_LEFT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_IDLE_LEFT);
								  
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_WALKING_STOP_LEFT); 
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_WALKING_RIGHT);
								  
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_WALKING_STOP_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_WALKING_LEFT);
								  
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_FLYING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_FALLING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_FALLING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_RUNNING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_RUNNING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_RUNNING_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_RUNNING_FLYING_LEFT);
	
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_KICK_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_KICK_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_IDLE_HOLD_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_IDLE_HOLD_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_WALKING_HOLD_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_WALKING_HOLD_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_HOLD_JUMPING_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_HOLD_JUMPING_KOOPAS_LEFT);


	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_GET_INTO_HIDDEN_MAP);


	return listAni_Mario_Big;
}

vector<int> Constant::getAnimationMARIO_BIG_HAVE_FIRE()
{
	listAni_Mario_Big.clear();

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_SITTING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_IDLE_RIGHT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_SITTING_LEFT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_IDLE_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_WALKING_STOP_LEFT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_WALKING_RIGHT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_WALKING_STOP_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_WALKING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_FLYING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_FALLING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_FALLING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_RUNNING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_RUNNING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_RUNNING_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_RUNNING_FLYING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_KICK_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_KICK_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_IDLE_HOLD_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_IDLE_HOLD_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_WALKING_HOLD_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_WALKING_HOLD_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_HOLD_JUMPING_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_HOLD_JUMPING_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FIRE_GET_INTO_HIDDEN_MAP);



	return listAni_Mario_Big;
}

vector<int> Constant::getAnimationMARIO_BIG()
{
	listAni_Mario_Big.clear();

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_SITTING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_IDLE_RIGHT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_SITTING_LEFT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_IDLE_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_WALKING_STOP_LEFT); 
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_WALKING_RIGHT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_WALKING_STOP_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_WALKING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FLYING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FALLING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_FALLING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_RUNNING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_RUNNING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_RUNNING_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_RUNNING_FLYING_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_KICK_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_KICK_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_IDLE_HOLD_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_IDLE_HOLD_KOOPAS_LEFT);


	listAni_Mario_Big.push_back(MARIO_ANI_BIG_WALKING_HOLD_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_WALKING_HOLD_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_HOLD_JUMPING_KOOPAS_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_HOLD_JUMPING_KOOPAS_LEFT);

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_GET_INTO_HIDDEN_MAP);

	return listAni_Mario_Big;
}