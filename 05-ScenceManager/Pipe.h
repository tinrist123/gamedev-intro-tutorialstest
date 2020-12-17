#pragma once
#define BBOX_WIDTH  16
#define BBOX_HEIGHT 16
#include "Ground.h"

class Pipe : public Ground
{
public:
	int height;
	int width;
	bool isHaveHiddenMap;

	bool isPushMarioOut;
	bool isInHiddenMap;
	int kindTreatmentPlayer;
public:
	Pipe();

	void PipeHaveHiddenMap() {
		this->isHaveHiddenMap = true;
	}
	Pipe(int width, int height);
	Pipe(int width, int height,bool isHaveHiddenMap  ,bool isInHiddenMap,bool isPushMarioOut );
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


