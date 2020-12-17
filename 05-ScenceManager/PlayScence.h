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
#include "Leaf.h"
#include "Mushroom.h"
#include "Grid.h"
#include "BoardGame.h"
#include "Camera.h"
#include "Timer.h"

class CPlayScene: public CScene
{
public: 
	CMario *player;					// A play scene has to have player, right? 
	Grid* grid;
	Camera* cam;
	
	vector<LPGAMEOBJECT> objects;
	// Waiting for Grid
	vector<LPGAMEOBJECT> ObjectsInScreen;
	vector<LPGAMEOBJECT> items;
	vector<LPGAMEOBJECT> dynamicItems;
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> staticObjects;
	vector<LPGAMEOBJECT> listCBrick;


	vector<LPGAMEOBJECT> effects;
	vector<LPGAMEOBJECT> flowerBullet;
	vector<LPGAMEOBJECT> marioBullet;
	vector<LPGAMEOBJECT> listPipe;

	BoardGame* boardGame;

	Timer* transitionBgTime = new Timer(0);
	bool isCompletedTransition = false;
	bool isCompletedTransitionLightScreen = false;



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

	void playerHittingSpecialItem(LPGAMEOBJECT& item);
	
	bool checkObjOutOfCamera(LPGAMEOBJECT obj);
	void removeObjOutOfCamera(LPGAMEOBJECT& obj);

	// Opacity for Dark Screen
	int alphaTransition_opacity = 0;
	
	bool isTransition_For_LightScreen = false;
	// Render Dark Screen
	void DarkenTheScreen();
	void LightenTheScreen();
	
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

