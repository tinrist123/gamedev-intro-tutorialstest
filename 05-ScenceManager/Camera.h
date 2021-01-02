#pragma once
#include "Mario.h"
#include "Game.h"
#include "Map.h"


class Camera
{
public:
	float cam_x = 0;
	float cam_y = 0;
	bool isInHideMap = false;
	CMario* player;
	TileMap* map;
public:
	Camera(CMario* m, TileMap *map);
	Camera::~Camera()
	{
	}

	void getTileMap(TileMap* map) { this->map = map; }
	void Update();
	void SetCamPos(float x, float y) { this->cam_x = x; this->cam_y = y; }
	float GetCamPosX() { return cam_x; }
	float GetCamPosY() { return cam_y; }
	void SetCamPosY(float cam_y) { this->cam_y = cam_y; }
};

