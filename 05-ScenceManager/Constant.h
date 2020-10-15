#include "Mario.h"

#pragma once
class Constant
{
	int kindMario;
public:
	Constant(int kindMario);
	vector<int> listAni_Mario_Big;
	vector<int> listBBox_Mario_Big;
	vector<int> getBBox_MARIO_BIG_HAVE_TAIL();
	vector<int> getBBox_MARIO_BIG();
	vector<int> getAnimationMARIO_BIG_HAVE_TAIL();
	vector<int> getAnimationMARIO_BIG();
};

