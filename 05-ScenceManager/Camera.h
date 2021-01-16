#pragma once
#include "Mario.h"
#include "Game.h"
#include "Map.h"

#define NORMAL_CAMERA		0
#define MOVING_CAMERA		1

class Camera
{
public:
	bool specialCam = false;
	int typeCamera;

	float cam_x = 0;
	float cam_y = 0;
	bool isInHideMap = false;
	CMario* player;
	TileMap* map;
public:
	Camera(CMario* m, TileMap *map,int typeCamera = 0);
	Camera::~Camera()
	{
	}

	void getTileMap(TileMap* map) { this->map = map; }
	void Update(DWORD dt, float &posX_Player);
	void SetCamPos(float x, float y) { this->cam_x = x; this->cam_y = y; }
	float GetCamPosX() { return cam_x; }
	float GetCamPosY() { return cam_y; }
	void SetCamPosY(float cam_y) { this->cam_y = cam_y; }
	void SetCamPosX(float x) { this->cam_x = x;}
};

