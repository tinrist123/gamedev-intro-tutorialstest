#pragma once
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Flower.h"
#include "Ground.h"
#include "Pipe.h"
#include "QuestionBrick.h"
#include "ColorBox.h"
#include "WeakBrick.h"
#include "CloudBrick.h"
#include "Item.h"
#include "Map.h"
#include "FireBullet.h"
#include "MarioBullet.h"
#include "EffectsFire.h"
#include "Coin.h"
#include "ListEffects.h"
#include "EffectPoint.h"
#include "Leaf.h"
#include "Mushroom.h"
#include "Grid.h"
#include "BoardGame.h"
class CPlayScene: public CScene
{
public: 
	CMario *player;					// A play scene has to have player, right? 
	Grid* grid;
	

	vector<LPGAMEOBJECT> objects;
	// Waiting for Grid
	vector<LPGAMEOBJECT> ObjectsInScreen;
	vector<LPITEM> items;
	vector<LPENEMY> enemies;
	vector<LPGAMEOBJECT> staticObjects;
	vector<LPEFFECTS> effects;

	BoardGame* boardGame;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_Map(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void GetObjectGrid();

	Enemy *CreateFlowerBullet(CFlower *flower);
	void playerHittingSpecialItem(Item*& item);

	TileMap* map;
	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

