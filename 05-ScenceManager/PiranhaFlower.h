#pragma once
#include "Flower.h"
class PiranhaFlower : public Enemy
{
	bool priority = false;
	bool isWaitingShooting = false;
	bool isShooting = false;

	int typeFlower = 1;
	int ny = 0;

	int	  HeightPipe;

	PiranhaFlower(float x, float y, float);

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);


};

