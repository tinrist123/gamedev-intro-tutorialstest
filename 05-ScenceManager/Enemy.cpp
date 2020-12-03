#include "Enemy.h"
#include "Enemy.h"
#include "Game.h"


void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Enemy::Render()
{

}

bool Enemy::CheckOutCamera()
{
	CGame* game = CGame::GetInstance();
	return (x >= game->GetCamPosX() + 10 && x < game->GetCamPosX() + (SCREEN_WIDTH - 10) && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT));
}

Enemy::Enemy()
{
	this->category = Category::ENEMY;
}


Enemy::~Enemy()
{
}
