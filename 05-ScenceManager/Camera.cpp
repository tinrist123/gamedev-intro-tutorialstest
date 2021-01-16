#include "Camera.h"

Camera::Camera(CMario* player, TileMap *map,int typeCamera)
{
	this->player = player;
	this->map = map;
	this->typeCamera = typeCamera;
}

void Camera::Update(DWORD dt, float &posX_Player)
{
	//Update camera to follow player


	if (this->typeCamera == MOVING_CAMERA)
	{
		//cam_x += 0.03*dt;
		cam_x = 2090;
		if (cam_x  >= posX_Player)
		{
			posX_Player = cam_x;
		}
		DebugOut(L"player->x: %f\n", player->x);
		DebugOut(L"player->y: %f\n", player->y);
		if (cam_x > 1778 && cam_x < 1800)
		{
			cam_x = 1778.0f; //Lock Cam On End Pipe
		}
		else
			if (cam_x > 1800)
			{
				cam_x = player->x - (CGame::GetInstance()->screen_width / 4);
				if (player->x > (CGame::GetInstance()->screen_width / 4) && player->x + (CGame::GetInstance()->screen_width / 4) < 2528)
				{
					cam_x = player->x - (CGame::GetInstance()->screen_width / 4);
					if (cam_x < 2048) //Look cam First EndScene
						cam_x = 2048;
				}
				if (cam_x > 2290) //Look Cam Last EndScene
					cam_x = 2290;
			}
	}
	else if (this->typeCamera == NORMAL_CAMERA)
	{
		if (player->isInWorldSelectionMap)
		{
			cam_x = 0;
			cam_y = 0;
		}
		else
		{
			if (player->x > (CGame::GetInstance()->screen_width / 2) && player->x < map->GetWidthTileMap() - (CGame::GetInstance()->screen_width / 2))
			{
				cam_x = player->x - (CGame::GetInstance()->screen_width / 2);
			}
		}
	}
	

	if (!player->isInHiddenMap)
	{
		//CAMY TOT NHAT 
		if (player->y > CGame::GetInstance()->GetScreenHeight() / 4 && player->y < map->GetHeightTileMap() - 350)
		{
			cam_y = player->y - CGame::GetInstance()->GetScreenHeight() / 4;
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
		cam_y = 225.0f;
		isInHideMap = false;
	}
	CGame::GetInstance()->SetCamPos(cam_x, cam_y);
}


