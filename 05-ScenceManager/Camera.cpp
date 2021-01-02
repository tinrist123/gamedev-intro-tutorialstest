#include "Camera.h"

Camera::Camera(CMario* player, TileMap *map)
{
	this->player = player;
	this->map = map;
}


void Camera::Update()
{
	//Update camera to follow player
	if (player->x > (CGame::GetInstance()->screen_width / 2) && player->x < map->GetWidthTileMap() - (CGame::GetInstance()->screen_width / 2))
	{
		cam_x = player->x - (CGame::GetInstance()->screen_width / 2);
	}

	if (!player->isInHiddenMap)
	{
		//CAMY TOT NHAT 
		if (player->y > CGame::GetInstance()->GetScreenHeight() / 4 && player->y < map->GetHeightTileMap() -  350)
		{
			cam_y = player->y - CGame::GetInstance()->GetScreenHeight() / 4 - 25.0f;
		}
	}
	else
	{
		cam_y = 436.0f;
		isInHideMap = true;
	}
	//DebugOut(L"map->GetHeightMap(): %d ", map->GetHeightMap());
	if (isInHideMap && !player->isInHiddenMap)
	{
		cam_y = 225.296234;
		isInHideMap = false;
	}

	CGame::GetInstance()->SetCamPos(cam_x, cam_y);
}


