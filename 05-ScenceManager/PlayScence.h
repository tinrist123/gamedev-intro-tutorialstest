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
	Timer* expiredTimeOfCoin;
	

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> mapObjects;
	vector<LPGAMEOBJECT> listPortalStop;
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

	CPortal* Portal;


	BoardGame* boardGame;
	bool isCreatedCoinFromPSwitch_Active = false;
	bool isReversedCoinToWeakBrick = false;

	Timer* transitionGetIntoWorldMap = new Timer(0);
	int indexScaleBackground = 0;

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
	CPlayScene(int id, LPCWSTR filePath, bool isWorldSeletion);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void GetObjectGrid();

	void CheckTimeExpired_Transformed();
	Item* CreateItemOfMario(QuestionBrick* object);
	Item* CreateItemForWeakBrick(WeakBrick* object);
	Enemy *CreateFlowerBullet(CFlower *flower);


	void playerHittingSpecialItem(LPGAMEOBJECT& item);
	
	bool checkObjOutOfCamera(LPGAMEOBJECT obj);
	void removeObjOutOfCamera(LPGAMEOBJECT& obj);

	bool checkOrdinateOutOfCamera(float x, float y);

	// Opacity for Dark Screen
	int alphaTransition_opacity = 255;
	
	bool isTransition_For_LightScreen = false;
	// Render Dark Screen
	void DarkenTheScreen();
	void LightenTheScreen();
	

	bool isTransitionScaleBg = false;
	void TransitionDarkScreen(int quantityW,int quantityH,int posX, int posY);
	
	// Detect Scale Background to have beatiful Transition
	void DetectScaleDarkScreenTransition(int timeCountDown);

	void Alter_Opacity_AlphaForBackground_Screen_By_Time(bool);

	TileMap* map;
	CMario * GetPlayer() { return player; } 


	float camY;
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

