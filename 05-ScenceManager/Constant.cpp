#include "Constant.h"

Constant::Constant(int kindMario)
{
	this->kindMario = kindMario;
	switch (this->kindMario)
	{
	case 2:
		listAni_Mario_Big = getAnimationMARIO_BIG();
		listBBox_Mario_Big = getBBox_MARIO_BIG();
		break;
	case 3:
		listAni_Mario_Big = getAnimationMARIO_BIG_HAVE_TAIL();
		listBBox_Mario_Big = getBBox_MARIO_BIG_HAVE_TAIL();
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

	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT);
	listAni_Mario_Big.push_back(MARIO_ANI_BIG_TAIL_SLOW_FALLING_LEFT);
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
	return listAni_Mario_Big;
}