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
#include "LetterEndGame.h"
#include "BoxItem.h"
#include "RandomItem.h"
#include "P_Switch.h"
#include "ShakeTree.h"
#include "PortalStop.h"
#include "P_Switch.h"
#include "MovingWood.h"
#include "BoomerangBrother.h"
#include "BomerangWeapon.h"
#include "SelectionCarret.h"

class CPlayScene: public CScene
{
public: 
	CMario *player;					// A play scene has to have player, right? 
	

	// intro scence
	Timer* timeMovingGreetingDoor = new Timer(200);
	Timer* timeToRemoveAdvertisement = new Timer(1000);
	bool isEndGreetingDoor = false;
	bool isAdvertisementStart = false;
	bool isBlackBackground = true;
	bool isRender = false;
	//END INTRO SCENCE
	
	
	Grid* gridGame;

	vector<int> listId_grid;

	// ID--ROW--COL---OBJ
	unordered_map<int, vector<unordered_map<int, int>>> objects_cell;

	// Default List to get Static Obj, this is Constant
	vector<LPGAMEOBJECT> listStaticObj;

	// Default List to get dyniamic/moving Obj, this is Constant
	vector<LPGAMEOBJECT> listMovingObject;

	Camera* cam;
	Timer* expiredTimeOfCoin;
	Timer* EndGameTime = new Timer(900);

	// All objects include: Moving and static Obj
	vector<LPGAMEOBJECT> objects;
	
	// Use for Grid, Hashmap
	unordered_map<int, LPGAMEOBJECT> objectsGrid;

	// All obj Indepen on Grid, Just push once
	vector<LPGAMEOBJECT> StaticObjects;

	// World Selection obj
	vector<LPGAMEOBJECT> listPortalStop;
	
	// Use grid to Get all objects in Screen
	vector<LPGAMEOBJECT> ObjectsInScreen;

	// All small lists
	vector<LPGAMEOBJECT> items;
	vector<LPGAMEOBJECT> dynamicItems;
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> listCBrick;



	// This List is special, It Trigger Event P_switch active 
	// Transform Coin to Weak or turn back
	vector<LPGAMEOBJECT> listWeakBrick;
	vector<LPGAMEOBJECT> listCoinTransform;
	// Trigger for P_switch

	// Special list, Independence on Grid
	vector<LPGAMEOBJECT> effects;
	vector<LPGAMEOBJECT> listFireBall;
	vector<LPGAMEOBJECT> marioBullet;

	// Trigger for Slipe into Pipe
	vector<LPGAMEOBJECT> listPipe;

	LPGAMEOBJECT selectionCarret;

	float posOfTextEndGame_x;
	float posOfTextEndGame_y;

	void storePosTextEndGame(float x, float y)
	{
		this->posOfTextEndGame_x = x;
		this->posOfTextEndGame_y = y;
	}

	CPortal* Portal;

	bool secondStringCreated = false;
	LettersEndGame* firstRowString;
	LettersEndGame* secondRowString = NULL;
	RandomItem* itemCollected = NULL;
	BoxItem* boxCollectedItem = NULL;

	BoardGame* boardGame;
	bool isCreatedCoinFromPSwitch_Active = false;
	bool isReversedCoinToWeakBrick = false;

	Timer* transitionGetIntoWorldMap = new Timer(0);
	int indexScaleBackground = 0;

	Timer* transitionBgTime = new Timer(0);
	bool isCompletedTransition = false;
	bool isCompletedTransitionLightScreen = false;
	bool isSpecialPipeContext = false;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_Map(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath, bool isWorldSeletion, int typeCamera);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void GetObjectGrid();


	bool checkElementExist(int idGrid);
	void CheckTimeExpired_Transformed();
	Item* CreateItemOfMario(QuestionBrick* object);
	Item* CreateItemForWeakBrick(WeakBrick* object);
	Enemy *CreateFlowerBullet(CFlower *flower);

	//void CreateBoomerang(bool &isFired, BoomerangBrother *boomerangBrother);

	void playerHittingSpecialItem(LPGAMEOBJECT& item);
	
	bool checkObjOutOfCamera(LPGAMEOBJECT obj);
	void removeObjOutOfCamera(LPGAMEOBJECT& obj);

	bool checkOrdinateOutOfCamera(float x, float y);

	// Opacity for Dark Screen
	int alphaTransition_opacity = 0;
	
	bool isTransition_For_LightScreen = false;
	// Render Dark Screen
	void DarkenTheScreen();
	void LightenTheScreen();
	
	void SwitchToSelectionWorld();

	bool isTransitionScaleBg = false;
	void TransitionDarkScreen(int quantityW,int quantityH,int posX, int posY);
	
	// Detect Scale Background to have beatiful Transition
	void DetectScaleDarkScreenTransition(int timeCountDown);

	void Alter_Opacity_AlphaForBackground_Screen_By_Time(bool);

	TileMap* map;
	CMario * GetPlayer() { return player; } 

	void SplitObjectsToDetect(vector<LPGAMEOBJECT>& coObjects);

	void DetectSpecialAttributeOfMap();

	float camY;
	//friend class CPlayScenceKeyHandler;

	void DrawBackGround()
	{
		LPDIRECT3DTEXTURE9 darken = CTextures::GetInstance()->Get(1);

		CGame::GetInstance()->Draw(0, 0, darken, 0, 0, 300,187, 255);
	}
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

