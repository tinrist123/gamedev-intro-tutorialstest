#pragma once
#include <iostream>
#include <fstream>
#include "PlayScence.h"
#include "P_Switch.h"
#include "ShakeTree.h"
#include "PortalStop.h"
#include "P_Switch.h"
#include "MovingWood.h"
#include "BoomerangBrother.h"
#include "BomerangWeapon.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath,bool isWorldSeletion, int typeCamera) : CScene(id, filePath, isWorldSeletion, typeCamera)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/
#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TILEMAP			7

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_Flower				4
#define OBJECT_TYPE_Ground				5
#define OBJECT_TYPE_ColorBox			6
#define OBJECT_TYPE_Pipe				7
#define OBJECT_TYPE_QUESTION_BRICK		8
#define OBJECT_TYPE_WeakBrick			9
#define OBJECT_TYPE_CloudBrick			10
#define OBJECT_TYPE_Item				11
#define OBJECT_TYPE_Bullet				12
#define OBJECT_TYPE_COIN				16
#define OBJECT_TYPE_ITEM_END_GAME		17
#define OBJECT_TYPE_MOVING_WOOD			18
#define OBJECT_TYPE_BOMERANG_BROTHER	19

// =============WORLD SELECTION ELEMENT
#define OBJECT_TYPE_SHAKE_TREE			20
#define OBJECT_TYPE_PORT_STOP			21


#define OBJECT_TYPE_PORTAL				50

#define MAX_SCENE_LINE					1024

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time


	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_Map(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 9) return;
	int ID = atoi(tokens[0].c_str());
	wstring filePath_texture = ToWSTR(tokens[1]);
	wstring filePath_data = ToWSTR(tokens[2]);
	int num_row_on_texture = atoi(tokens[3].c_str());
	int num_col_on_textture = atoi(tokens[4].c_str());
	int num_row_on_tilemap = atoi(tokens[5].c_str());
	int num_col_on_tilemap = atoi(tokens[6].c_str());
	int tileset_width = atoi(tokens[7].c_str());
	int tileset_height = atoi(tokens[8].c_str());
	float widthGrid = atoi(tokens[9].c_str());
	float heightGrid = atoi(tokens[10].c_str());
	grid = new Grid();
	grid->Resize(widthGrid, heightGrid);
	grid->PushGrid(objects);
	map = new TileMap(ID, filePath_texture.c_str(), filePath_data.c_str(), num_row_on_texture, num_col_on_textture, num_row_on_tilemap, num_col_on_tilemap, tileset_width, tileset_height);
	boardGame = new BoardGame(player);
	cam = new Camera(player,map,this->typeCamera);
}

/*		
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		//if (player!=NULL) 
		//{
		//	//DebugOut(L"[ERROR] MARIO object was created before!\n");
		//	return;
		//}
		//else
		{
			player = CMario::GetInstance();
			player->SetAnimationSet(ani_set);
			player->SetPosition(x, y);
			return;
		}
		//DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		int typeGoomba = atof(tokens[4].c_str());
		obj = new CGoomba(x,y,typeGoomba); 
		break;
	}
	case OBJECT_TYPE_MOVING_WOOD:
	{
		obj = new MovingWood();
		break;
	}
	case OBJECT_TYPE_ITEM_END_GAME:
	{
		obj = new RandomItem(x,y);
		storePosTextEndGame(x, y);
		break;
	}
	case OBJECT_TYPE_PORT_STOP:
	{
		float w = atof(tokens[4].c_str());
		float h = atof(tokens[5].c_str());

		float l = atof(tokens[6].c_str());
		float t = atof(tokens[7].c_str());
		float r = atof(tokens[8].c_str());
		float b = atof(tokens[9].c_str());

		obj = new PortalStop(w,h,l,t,r,b);
		break;
	}
	case OBJECT_TYPE_SHAKE_TREE:
	{
		obj = new ShakeTree();
		break;
	}
	case OBJECT_TYPE_CloudBrick:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new CloudBrick(width, height);
		break;
	}
	case OBJECT_TYPE_BRICK:
		{
			float width = atof(tokens[4].c_str());
			float height = atof(tokens[5].c_str());
			obj = new CBrick(width, height);
			break;
		}
	case OBJECT_TYPE_WeakBrick:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		int isHaveItem = 0;
		if (tokens.size() == 7)
			isHaveItem = atof(tokens[6].c_str());
		obj = new WeakBrick(width, height, isHaveItem);
		obj->start_x = x;
		obj->start_y = y;
		break;
	}
	case OBJECT_TYPE_BOMERANG_BROTHER: 
		obj = new BoomerangBrother();  
		break;
	case OBJECT_TYPE_Bullet: obj = new FireBullet(); break;
	case OBJECT_TYPE_COIN: 
	{
		obj = new Coin();
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		int typeKoopas = atof(tokens[4].c_str());
		int typeColorKoopas = atof(tokens[5].c_str());
		obj = new CKoopas(x,y,typeKoopas, typeColorKoopas);
		break;
	}
	case OBJECT_TYPE_Flower:
	{
		int HeightPipe = atof(tokens[4].c_str());
		int typeFlower = atof(tokens[5].c_str());
		obj = new CFlower(x,y, HeightPipe,typeFlower);
		break;
	}
	case OBJECT_TYPE_Ground: 
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new Ground(width, height);
		break;
	}
	case OBJECT_TYPE_ColorBox:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new ColorBox(width, height);
		break;
	}
	case OBJECT_TYPE_Pipe:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		if (tokens.size() < 7)
		{
			obj = new Pipe(width, height);
		}
		else {
			int isMapInside = atof(tokens[6].c_str());
			int isInHiddenMap = atof(tokens[7].c_str());
			int isPushMarioOut = atof(tokens[8].c_str());	
			if (tokens.size() > 9)
			{
				int directionOfMario = atof(tokens[9].c_str());
				obj = new Pipe(width, height, isMapInside == 1, isInHiddenMap == 1, isPushMarioOut == 1,directionOfMario);
			}
			else
			{
				obj = new Pipe(width, height, isMapInside == 1, isInHiddenMap == 1, isPushMarioOut == 1);
				this->isSpecialPipeContext = true;
			}
			
		}
		break;
	}
	case OBJECT_TYPE_QUESTION_BRICK:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		int itemInside = atof(tokens[6].c_str());
		obj = new QuestionBrick(x,y,width, height,itemInside);
		break;
	}
	case OBJECT_TYPE_PORTAL:
		{	
			int quantityW = atof(tokens[3].c_str());
			int quantityH = atof(tokens[4].c_str());
			int scene_id = atoi(tokens[5].c_str());
			obj = new CPortal(quantityW,quantityH, scene_id);
			if (player->isInWorldSelectionMap)
			{
				obj->SetAnimationSet(ani_set);
				obj->SetPosition(x, y);
				listPortalStop.push_back(obj);
			}
		}
		break;
	default:
		//DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetAnimationSet(ani_set);
	obj->SetPosition(x, y);

	if (obj->getTypeObject() == Type::PIPE)
	{
		listPipe.push_back(obj);
	}
	else if (obj->getTypeObject() == Type::PORTAL)
	{
		Portal = (CPortal *)obj;
	}
	if (obj->getCategoryObject() == Category::MAPOBJECT)
	{
		//mapObjects.push_back(obj);
		if (obj->getTypeObject() == Type::PORTALSTOP || obj->getTypeObject() == Type::SHAKETREE)
		{
			listPortalStop.push_back(obj);
		}
	}
	else if (!player->isInWorldSelectionMap)
	{
		objects.push_back(obj);
	}
}

void CPlayScene::Load()
{
	//DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	//mapScence = new TileMap();
	
	ifstream f;
	f.open(sceneFilePath);
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TILEMAP]") { 
			section = SCENE_SECTION_TILEMAP; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_TILEMAP: _ParseSection_Map(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	//DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}


void CPlayScene::CheckTimeExpired_Transformed()
{
	if (this->expiredTimeOfCoin && this->expiredTimeOfCoin != 0 && this->isCreatedCoinFromPSwitch_Active)
	{
		if (this->expiredTimeOfCoin->IsTimeUp())
		{
			this->isCreatedCoinFromPSwitch_Active = false;
			this->expiredTimeOfCoin = 0;
			listCoinTransform.clear();
		}
	}
}

Item* CPlayScene::CreateItemOfMario(QuestionBrick* object)
{
	object->isCreated = true;
	if (object->ItemInside == ITEM_COIN)
	{
		Coin *coin = new Coin(object->x, object->y);
		coin->SetPosition(object->x, object->y);
		coin->SetState(ITEM_STATE_COIN_JUMP);
		return coin;
	}
	else if (object->ItemInside == SPECIAL_ITEM)
	{
		int typeItem = (player->level < MARIO_LEVEL_BIG_TAIL) ? ITEM_ANI_MUSHROOM : 2;
		if (typeItem == 2)
		{
			Leaf *leaf = new Leaf(object->x, object->y);
			leaf->SetPosition(object->x, object->y - 4);
			if (player->x > (leaf->x)) leaf->nx = -1;
			else if (player->x < (leaf->x)) leaf->nx = 1;
			leaf->SetState(ITEM_STATE_LEAF_FALL);
			return leaf;
		}
		else if (typeItem == ITEM_ANI_MUSHROOM)
		{
			Mushroom *mushroom = new Mushroom(object->start_x, object->start_y);
			mushroom->SetPosition(object->x, object->y - 2.5);
			if (player->x > (mushroom->x )) mushroom->nx = -1;
			else if (player->x < (mushroom->x )) mushroom->nx = 1;
			mushroom->SetState(ITEM_STATE_SPECIAL_ITEM);
			mushroom->setKindOfMushroom(KIND_RED_MUSHROOM);
			return mushroom;
		}
	}
}
Item* CPlayScene::CreateItemForWeakBrick(WeakBrick* weakBrick)
{
	weakBrick->isTouchable = false;
	if (weakBrick->kindOfItemInsinde == WEAK_BRICK_HAVE_P_SWITCH)
	{
		P_Switch* p_switch= new P_Switch(weakBrick->x, weakBrick->y);
		p_switch->SetPosition(weakBrick->x, weakBrick->y - 16);
		if (player->x > (p_switch->x)) p_switch->nx = -1;
		else if (player->x < (p_switch->x)) p_switch->nx = 1;
		return p_switch;
	}
	else if (weakBrick->kindOfItemInsinde == WEAK_BRICK_HAVE_COIN)
	{
		if (weakBrick->health > 0)
		{
			weakBrick->isCreated = false;
		}
		Coin* coin = new Coin(weakBrick->x, weakBrick->y);
		coin->SetPosition(weakBrick->x + 4, weakBrick->y);
		coin->SetState(ITEM_STATE_COIN_JUMP);
		return coin;
	}
	else if (weakBrick->kindOfItemInsinde == WEAK_BRICK_HAVE_LEVEL_MUSHROOM)
	{
		Mushroom* mushroom = new Mushroom(weakBrick->start_x, weakBrick->start_y);
		mushroom->SetPosition(weakBrick->x, weakBrick->y - 2.5);
		if (player->x > (mushroom->x)) mushroom->nx = -1;
		else if (player->x < (mushroom->x)) mushroom->nx = 1;
		mushroom->SetState(ITEM_STATE_SPECIAL_ITEM);
		mushroom->setKindOfMushroom(KIND_GREEN_MUSHROOM);
		return mushroom;
	}
	else if (weakBrick->kindOfItemInsinde == WEAK_BRICK_HAVE_SPECIAL_ITEM)
	{
		int typeItem = (player->level < MARIO_LEVEL_BIG_TAIL) ? ITEM_ANI_MUSHROOM : 2;
		if (typeItem == 2)
		{
			Leaf* leaf = new Leaf(weakBrick->x, weakBrick->y);
			leaf->SetPosition(weakBrick->x, weakBrick->y - 4);
			if (player->x > (leaf->x)) leaf->nx = -1;
			else if (player->x < (leaf->x)) leaf->nx = 1;
			leaf->SetState(ITEM_STATE_LEAF_FALL);
			return leaf;
		}
		else if (typeItem == ITEM_ANI_MUSHROOM)
		{
			Mushroom* mushroom = new Mushroom(weakBrick->start_x, weakBrick->start_y);
			mushroom->SetPosition(weakBrick->x, weakBrick->y - 2.5);
			if (player->x > (mushroom->x)) mushroom->nx = -1;
			else if (player->x < (mushroom->x)) mushroom->nx = 1;
			mushroom->SetState(ITEM_STATE_SPECIAL_ITEM);
			mushroom->setKindOfMushroom(KIND_RED_MUSHROOM);
			return mushroom;
		}
	}
}
Enemy* CPlayScene::CreateFlowerBullet(CFlower* flower)
{
	FireBullet* bullet = new FireBullet();
	bullet->x = flower->x;
	bullet->y = flower->y + 4;
	bullet->nx = (player->x - flower->x < 0) ? -1 : 1;
	bullet->ny = (player->y - flower->y < 0) ? -1 : ((player->y - flower->y > 16.0f)) ? 1 : 0;
	bullet->SetState(1);
	return bullet;
}

//void CPlayScene::CreateBoomerang(bool& isFired, BoomerangBrother* boomerangBrother)
//{
//	if (!boomerangBrother->DelayFireTime->isStarted)
//	{
//		boomerangBrother->DelayFireTime->Start();
//	}
//	else
//	{
//		if (boomerangBrother->DelayFireTime->IsTimeUp())
//		{
//			boomerangBrother->DelayFireTime->Stop();
//			boomerangBrother->isThrowAnotherBoomerang = true;
//			isFired = true;
//		}
//	}
//}

// special item mean mushroom or leaf or more
void CPlayScene::playerHittingSpecialItem(LPGAMEOBJECT& item)
{
	item->SetState(ITEM_STATE_HITTING_MARIO);
	player->SetLevel((player->level >= MARIO_LEVEL_BIG_TAIL) ? MARIO_LEVEL_BIG_TAIL : ++player->level);
	
	// GET ITEM AND UPDATE MARIO LEVEL
	if (player->level == 2) player->y += -15.0f;
	else player->y += -0.4f;

	EffectPoint* effect = new EffectPoint();
	effect->SetPosition(item->x, item->y);
	effects.push_back(effect);
}

bool CPlayScene::checkObjOutOfCamera(LPGAMEOBJECT obj)
{
	float cam_x = CGame::GetInstance()->GetCamPosX();
	float cam_y = CGame::GetInstance()->GetCamPosY();
		
	{
		if (obj->x > (double)(1.0 * CGame::GetInstance()->GetScreenWidth()) + cam_x)
			return true;
	}
	{
		if (obj->x < cam_x - BBOX_BIT)
		{
			return true;
		}
	}

	/*if (obj->y > CGame::GetInstance()->GetScreenHeight()
		|| obj->y < cam_y)
	{
		return true;
	}*/

	return false;
}

void CPlayScene::removeObjOutOfCamera(LPGAMEOBJECT& obj)
{
	float cam_x = CGame::GetInstance()->GetCamPosX();
	float cam_y = CGame::GetInstance()->GetCamPosY();
	{
		if (obj->x >  (double)(1.0* CGame::GetInstance()->GetScreenWidth()) + cam_x)
			obj->setObjDisappear();
	}
	{
		if (obj->x < cam_x - BBOX_BIT)
		{
			obj->setObjDisappear();
		}
	}
	/*if (obj->y > CGame::GetInstance()->GetScreenHeight()
		|| obj->y < cam_y)
	{
		obj->setObjDisappear();
	}*/
}

bool CPlayScene::checkOrdinateOutOfCamera(float x, float y)
{
	float cam_x = CGame::GetInstance()->GetCamPosX();
	float cam_y = CGame::GetInstance()->GetCamPosY();

	
	if (x > (double)(1.0 * CGame::GetInstance()->GetScreenWidth()) + cam_x)
		return true;
	if (x < cam_x - BBOX_BIT)
	{
		return true;
	}

	/*if (y > CGame::GetInstance()->GetScreenHeight()
		|| y < cam_y)
	{
		return true;
	}*/

	return false;
}

void CPlayScene::LightenTheScreen()
{
	
	LPDIRECT3DTEXTURE9 darken = CTextures::GetInstance()->Get(1);
	RECT rect;

	float l = CGame::GetInstance()->GetCamPosX();
	float t = CGame::GetInstance()->GetCamPosY();

	rect.left = 0;
	rect.top = 0;
	rect.right = ceil(CGame::GetInstance()->GetScreenWidth());
	rect.bottom = ceil(CGame::GetInstance()->GetScreenHeight());
	
	CGame::GetInstance()->Draw(l, t, darken, rect.left, rect.top, rect.right, rect.bottom, this->alphaTransition_opacity);
}
void CPlayScene::SwitchToSelectionWorld()
{
	player->isInWorldSelectionMap = true;
	CGame::GetInstance()->SwitchScene(SCENCE_ID_WORLD_SELECTION);
	player->SetState(MARIO_STATE_IDLE);
	player->SetSpeed(0,0);
	player->SetPosition(player->portalStop->x, player->portalStop->y);
	player->isWalking = false;
	player->isCanGetIntoWorldMap = false;

	CGame::GetInstance()->ResetCamera();
	return;
}
void CPlayScene::TransitionDarkScreen(int quantityW, int quantityH, int posX, int posY)
{
	LPDIRECT3DTEXTURE9 darken = CTextures::GetInstance()->Get(1);
	RECT rect;

	float l = CGame::GetInstance()->GetCamPosX() + posX;
	float t = CGame::GetInstance()->GetCamPosY() + posY;

	rect.left = 0;
	rect.top = 0;
	rect.right = ceil(quantityW * BBOX_BIT);
	rect.bottom = ceil(quantityH * 16 );

	CGame::GetInstance()->Draw(l, t, darken, rect.left, rect.top, rect.right, rect.bottom, 255);


}
void CPlayScene::DetectScaleDarkScreenTransition(int timeCountDown)
{
	if (this->transitionGetIntoWorldMap->isStarted == false)
	{
		this->transitionGetIntoWorldMap->Start();
	}
	if (transitionGetIntoWorldMap->GetElapsedTime() > timeCountDown)
	{
		this->indexScaleBackground++;
		this->transitionGetIntoWorldMap->Start();
	}
	int middlePointScreen = CGame::GetInstance()->GetScreenHeight() / 32 + 2;
	if (this->indexScaleBackground >= middlePointScreen)
	{
		this->isTransitionScaleBg = false;
		player->isInWorldSelectionMap = false;
		//player->CompleteSwitchedScreen();
		CGame::GetInstance()->SwitchScene(player->scenceId);
		// Reset to reuse
		this->indexScaleBackground = 0;
		DetectSpecialAttributeOfMap();
		return;

	
	}
}
void CPlayScene::Alter_Opacity_AlphaForBackground_Screen_By_Time(bool isDark)
{
	if (isDark)
	{
		if (!transitionBgTime->isStarted)
		{
			transitionBgTime->Start();
		}
		if (transitionBgTime->isStarted && transitionBgTime != 0)
		{
			if (transitionBgTime->GetElapsedTime() > 600)
			{
				this->alphaTransition_opacity = 255;
				this->isCompletedTransition = true;
			}
			else if (transitionBgTime->GetElapsedTime() > 550)
				this->alphaTransition_opacity = 200;
			else if (transitionBgTime->GetElapsedTime() > 500)
				this->alphaTransition_opacity = 150;
			else if (transitionBgTime->GetElapsedTime() > 450)
				this->alphaTransition_opacity = 100;
			else if (transitionBgTime->GetElapsedTime() > 400)
				this->alphaTransition_opacity = 50;
		}
	}
	else if (!isDark)
	{
		if (!transitionBgTime->isStarted)
		{
			transitionBgTime->Start();
		}
		if (transitionBgTime->isStarted && transitionBgTime != 0)
		{
			if (transitionBgTime->GetElapsedTime() > 500)
			{
				this->alphaTransition_opacity = 0;
				this->isTransition_For_LightScreen = false;
				transitionBgTime->isStarted = false;
			}
			else if (transitionBgTime->GetElapsedTime() > 400)
				this->alphaTransition_opacity = 100;
			else if (transitionBgTime->GetElapsedTime() > 350) this->alphaTransition_opacity = 150;
			else if (transitionBgTime->GetElapsedTime() > 300) this->alphaTransition_opacity = 200;
			else if (transitionBgTime->GetElapsedTime() > 250) this->alphaTransition_opacity = 255;
		}
	}
}
void CPlayScene::DetectSpecialAttributeOfMap()
{
	if (player->scenceId == SCENCE_ID_WORLD_MOVING_CAMERA)
	{
		cam->specialCam = true;
	}
}
void CPlayScene::DarkenTheScreen()
{
	LPDIRECT3DTEXTURE9 darken = CTextures::GetInstance()->Get(1);
	RECT rect;

	float l = CGame::GetInstance()->GetCamPosX();
	float t = CGame::GetInstance()->GetCamPosY();

	rect.left = 0;
	rect.top = 0;
	rect.right = ceil(CGame::GetInstance()->GetScreenWidth());
	rect.bottom = ceil(CGame::GetInstance()->GetScreenHeight());

	

	CGame::GetInstance()->Draw(l, t, darken, rect.left, rect.top, rect.right, rect.bottom, this->alphaTransition_opacity);
}


void CPlayScene::GetObjectGrid()
{
	ObjectsInScreen.clear();
	items.clear();
	staticObjects.clear();
	listCBrick.clear();
	listWeakBrick.clear();
	enemies.clear();

	grid->GetGrid(ObjectsInScreen);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "good" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	float playerPositionX = player->x;
	float playerPositionY = player->y;

	grid->CheckCamGrid(objects);
	GetObjectGrid();


	for (size_t i = 0; i < ObjectsInScreen.size(); i++)
	{
		coObjects.push_back(ObjectsInScreen[i]);

		if (ObjectsInScreen[i]->getCategoryObject() == Category::GROUND)
		{
			staticObjects.push_back(ObjectsInScreen[i]);
		}
		else if (ObjectsInScreen[i]->getCategoryObject() == Category::ENEMY)
		{
			enemies.push_back(ObjectsInScreen[i]);
		}
		else if (ObjectsInScreen[i]->getCategoryObject() == Category::ITEM)
		{
			items.push_back(ObjectsInScreen[i]);
		}
		else if (ObjectsInScreen[i]->getCategoryObject() == Category::BRICK)
		{
			listCBrick.push_back(ObjectsInScreen[i]);
			staticObjects.push_back(ObjectsInScreen[i]);
			if (ObjectsInScreen[i]->getTypeObject() == Type::WEAKBRICK)
			{
				listWeakBrick.push_back(ObjectsInScreen[i]);
			}
		}
	}

	{
		if (player->isFireShoot)
		{
			if (marioBullet.size() < 2)
			{
				player->bullet = new MarioBullet();
				player->bullet->x = player->x + 5 * player->nx;
				player->bullet->y = player->y;
				player->bullet->nx = player->nx;
				player->bullet->SetState(BULLET_STATE_NORMAL_MOVING);
				marioBullet.push_back(player->bullet);
			}
			player->isFireShoot = false;
		}
		if (player->isInWorldSelectionMap)
		{
			player->Update(dt, &coObjects,&dynamicItems,&enemies,&listPortalStop);
			if (player->isSwitchedScreen)
			{
				this->isTransitionScaleBg = true;
			}
		}
		else
		{
			if (player->isSwitchedScreen)
			{
				this->isTransition_For_LightScreen = true;
				player->CompleteSwitchedScreen();
			}
			player->Update(dt, &staticObjects, &dynamicItems, &enemies);
		}
	}

	for (size_t i = 0; i < listWeakBrick.size(); i++)
	{
		if (this->isCreatedCoinFromPSwitch_Active)
		{
			if (listWeakBrick[i]->getTypeObject() == Type::WEAKBRICK)
			{
				WeakBrick* weakBrick = dynamic_cast<WeakBrick*>(listWeakBrick[i]);
				if (!weakBrick->isTranformed )
				{
					if (!weakBrick->isHaveP_Switch)
					{
						weakBrick->activeTranform();
						weakBrick->isRendered = false;
						weakBrick->isBoundingBox = false;
						Coin* coin = new Coin();
						coin->SetPosition(weakBrick->x, weakBrick->y);
						coin->isTranformed_Coin = true;
						coin->SetState(ITEM_STATE_IDLE_COIN);
						listCoinTransform.push_back(coin);
					}
					else if (weakBrick->isHaveP_Switch)
					{
						Coin* coin = new Coin();
						coin->SetPosition(weakBrick->x, weakBrick->y);
						coin->isTranformed_Coin = true;
						coin->SetState(ITEM_STATE_IDLE_COIN);
						coin->isTouchable = false;
						listCoinTransform.push_back(coin);
					}
				}

			}
		}
		if (!this->isCreatedCoinFromPSwitch_Active)
		{
			WeakBrick* weakBrick = dynamic_cast<WeakBrick*>(listWeakBrick[i]);
			weakBrick->isBoundingBox = true;
			weakBrick->isTranformed = false;
			weakBrick->isRendered = true;
		}
		
	}

	for (size_t i = 0; i < listCBrick.size(); i++)
	{
		if (listCBrick[i]->health == 0 && dynamic_cast<QuestionBrick*>(listCBrick[i]) && !staticObjects[i]->isCreated)
		{
			QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*>(staticObjects[i]);
			// TODO: EFFECT DISAPEAR SO COIN MUST DISAPEAR TOO, so Calculate it
			// DONE
			dynamicItems.push_back(CreateItemOfMario(questionBrick));
		}
		else if (listCBrick[i]->getTypeObject() == Type::WEAKBRICK
			&& !listCBrick[i]->isCreated && listCBrick[i]->isTouchable)
		{
			WeakBrick* weakBrick = dynamic_cast<WeakBrick*>(listCBrick[i]);
			weakBrick->isCreated = true;
			dynamicItems.push_back(CreateItemForWeakBrick(weakBrick));
		}
		// WeakBrick is hit by Mario and tranform 4 pieces
		if (listCBrick[i]->isAddedEffect)
			if (listCBrick[i]->isAddedEffect && listCBrick[i]->getTypeObject() == Type::WEAKBRICK)
		{
			WeakBrick* weakBrick = dynamic_cast<WeakBrick*>(listCBrick[i]);
			effects.push_back(weakBrick->listPiece[0]);
			effects.push_back(weakBrick->listPiece[1]);
			effects.push_back(weakBrick->listPiece[2]);
			effects.push_back(weakBrick->listPiece[3]);
			weakBrick->setObjDisappear();
		}

		/*if (listCBrick[i]->checkInActiveUpdated())
		{
			if (this->isReversedCoinToWeakBrick)
			{
				WeakBrick* weakBrick = dynamic_cast<WeakBrick*>(listCBrick[i]);
				weakBrick->activeUpdate();
				weakBrick->isRendered = true;
				weakBrick->isTranformed = false;
			}
			continue;
		}*/

		listCBrick[i]->Update(dt, NULL);

	}


	for (size_t i = 0; i < marioBullet.size(); i++)
	{
		marioBullet[i]->Update(dt, &ObjectsInScreen);
		if (!(marioBullet[i])->isAddedEffect
			&& marioBullet[i]->objectDisappear()
			)
		{
			MarioBullet* Bullet = dynamic_cast<MarioBullet*>(marioBullet[i]);
			if (Bullet->isCollision)
			{
				(marioBullet[i])->isAddedEffect = true;
				EffectsFire* effect = new EffectsFire();
				effect->SetPosition(Bullet->pointCollisionX, Bullet->pointCollisionY);
				effects.push_back(effect);

				if (Bullet->isHittingEnemy)
				{
					EffectPoint* point = new EffectPoint();
					point->SetPosition(Bullet->pointCollisionX, Bullet->pointCollisionY);
					effects.push_back(point);
				}
			}
		}
		this->removeObjOutOfCamera(marioBullet[i]);
	}


	for (size_t i = 0; i < listFireBall.size(); i++)
	{
		listFireBall[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt, &coObjects);
	}

	CheckTimeExpired_Transformed();

	for (size_t i = 0; i < dynamicItems.size(); i++)
	{
		if (dynamicItems[i]->getCategoryObject() == Category::ITEM)
		{
			{
				Item* item = dynamic_cast<Item*>(dynamicItems[i]);
				if (item->pointEff)
				{
					if (item->pointEff->objectDisappear())
					{
						dynamicItems[i]->setObjDisappear();
					}
				}
			}
		}
		if (dynamicItems[i]->getTypeObject() == Type::P_SWITCH && !this->isCreatedCoinFromPSwitch_Active)
		{
			P_Switch* p_switch = dynamic_cast<P_Switch*>(dynamicItems[i]);
			if (p_switch->effect && !p_switch->isCreated)
			{
				p_switch->isCreated = true;
				effects.push_back(p_switch->effect);
			}
			if (p_switch->isActived && p_switch->health == 1)
			{
				p_switch->subHealth();
				this->isCreatedCoinFromPSwitch_Active = true;
				expiredTimeOfCoin = new Timer(6000);
				expiredTimeOfCoin->Start();
			}

		}
		dynamicItems[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt, &staticObjects);
	}

	player->CollideWithItem(items, effects);
	player->CollideWithItem(dynamicItems, effects);
	player->CollideWithCoinTransform(listCoinTransform,listWeakBrick);
	player->CollisionWithFireball(listFireBall);


	int enemySize = enemies.size();
	for (size_t i = 0; i < enemySize; i++)
	{
		if (enemies[i]->GetHealth() != 0)
		{
			if (fabs(enemies[i]->x - playerPositionX) < 90.0f && enemies[i]->getTypeObject() == Type::FLOWER)
			{
				CFlower* flower = dynamic_cast<CFlower*>(enemies[i]);

				if (flower->health == 0)
				{
					effects.push_back(flower->pointEff);
				}

				if (flower->state == FLOWER_STATE_UP)
				{
					if (flower->isWaitingShooting && flower->y == 336.0f)
					{
						flower->isShooting = true;
						if (flower->isReadyFire() && flower->delayBullet != 0)
						{
							flower->isWaitingShooting = false;
							if (
								flower->typeFlower != FLOWER_TYPE_PIRANHA_FLOWER_FIRE
								)
							{
								listFireBall.push_back(CreateFlowerBullet(flower));
							}
							flower->delayBullet = 0;
						}
						else if (flower->delayBullet == 0)
						{
							flower->setTimeLoadingBullet();
						}

					}
					flower->nx = (player->x - flower->x < 0) ? -1 : 1;
					flower->ny = (player->y - flower->y < 0) ? -1 : ((player->y - flower->y > 16.0f)) ? 1 : 0;
				}

			}
			else if (enemies[i]->getTypeObject() == Type::KOOPAS && enemies[i]->isAliveObject())
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(enemies[i]);				
				if (player->isCanHoldingKoopas && koopas->isOutOfControl)
				{
					// detect Nx to accurately hold koopasf
					float distanceKoopasByOy = (player->level == MARIO_LEVEL_SMALL) ? 0 : 0;
					float distanceKoopasbyOx = (player->nx == 1) ? 18 : -5;
					if (player->level == MARIO_LEVEL_SMALL)
					{
						distanceKoopasbyOx = (player->nx == 1) ? 19 : 0;

					}
					// Hold koopas
					koopas->updatePositionKoopasByPositionMario(player->x + distanceKoopasbyOx, player->y - distanceKoopasByOy);
				}
				else if (player->isCanHoldingKoopas && !koopas->isOutOfControl && koopas->isPlayerHolding)
				{
					// When koopas revive, player Cant hold it
					player->isCanHoldingKoopas = false;
					koopas->isPlayerHolding = false;
				}
				if (!player->isCanHoldingKoopas && koopas->isOutOfControl)
				{
					player->ChainKickKoopas(koopas, false);
				}
			}
			else if (enemies[i]->getTypeObject() == Type::GOOMBA && enemies[i]->health == 2)
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(enemies[i]);
				float goombaPositionX = goomba->x;
				float goombaPositionY = goomba->y;
				if (goomba->level == PARAGOOMBA)
				{
					goomba->followPlayerByNx(((goombaPositionX - playerPositionX) > 0) ? -1 : 1);
					if (fabs(goombaPositionX - playerPositionX) < 250.0f
						&& !goomba->IsBlockingChangeState
						&& goomba->health == 2
						)
					{
						// Become crazy goomba and it have 3 state change circle
						goomba->blockingChangeState(true);
						goomba->startTimeChangeState();
					}
				}
			}
			else if (enemies[i]->getTypeObject() == Type::KOOPAS && enemies[i]->GetState() == KOOPAS_STATE_SHELL_MOVING)
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(enemies[i]);
				koopas->CollideWithEnemies(&enemies, &effects);
			}
			else if (enemies[i]->getTypeObject() == Type::BOOMERANGBROTHER)
			{
				BoomerangBrother* boomerangBrother = dynamic_cast<BoomerangBrother*>(enemies[i]);
				boomerangBrother->CollideWithBoomerang(&listFireBall);
				boomerangBrother->nx = (boomerangBrother->x - playerPositionX < 0) ? 1 : -1;

				// First Action when throw boomerang, aim target
				/*if (boomerangBrother->isCanHoldingBoomerang)
				{
					for (size_t i = 0; i < listFireBall.size(); i++)
					{
						if (listFireBall[i]->getTypeObject() == Type::BOOMERANG)
						{
							BoomerangWeapon* boomerang = dynamic_cast<BoomerangWeapon*>(listFireBall[i]);
							if (boomerang->isAimToStatus)
							{
								boomerang->SetPosition(boomerangBrother->x, boomerangBrother->y - BBOX_BIT / 4);
							}
						}
					}
				}
				else if (!boomerangBrother->isCanHoldingBoomerang)
				{
					for (size_t i = 0; i < listFireBall.size(); i++)
					{
						if (listFireBall[i]->getTypeObject() == Type::BOOMERANG)
						{
							BoomerangWeapon* boomerang = dynamic_cast<BoomerangWeapon*>(listFireBall[i]);
							if (boomerang->isAimToStatus)
							{
								boomerang->isAimToStatus = false;
								boomerang->SetState(BOOMERANG_STATE_MOVE_HORIZONTAL);
							}
						}
					}
				}*/

				if (fabs(enemies[i]->x - playerPositionX) < 100.0f)
				{
					bool isFired = false;
					if (boomerangBrother->countBoomerangWeapon <= 3)
					{
						if (boomerangBrother->getAllBoomerang)
						{
							// Waiting For Throw Boomerang, he has delayed
							if (!boomerangBrother->WaitingAnotherShootingBmrTime->isStarted)
							{
								boomerangBrother->WaitingAnotherShootingBmrTime->Start();
							}
							else
							{
								if (boomerangBrother->WaitingAnotherShootingBmrTime->IsTimeUp())
								{
									if (!boomerangBrother->loadingShootingTime->isStarted)
									{
										boomerangBrother->loadingShootingTime->Start();
										boomerangBrother->SetState(BOOMERANG_BROTHER_STATE_PREPARING_ATTACK);
										// KHONG CO Y TUONG
										boomerangBrother->isCanHoldingBoomerang = true;
									}
									else
									{
										if (boomerangBrother->loadingShootingTime->IsTimeUp())
										{
											boomerangBrother->SetState(BOOMERANG_BROTHER_STATE_WALKING_OR_ATTACKING);
											boomerangBrother->loadingShootingTime->Stop();
											boomerangBrother->WaitingAnotherShootingBmrTime->Stop();
											isFired = true;
										}
									}
								
								}
							}

							if (boomerangBrother->countBoomerangWeapon == 3)
							{
								boomerangBrother->getAllBoomerang = false;
								boomerangBrother->WaitingAnotherShootingBmrTime->Stop();
								boomerangBrother->loadingShootingTime->Stop();
							}
						
						}

						if (isFired)
						{
							BoomerangWeapon* boomerang = new BoomerangWeapon();
							boomerang->SetPosition(enemies[i]->x, enemies[i]->y);
							boomerang->nx = enemies[i]->x - playerPositionX < 0 ? 1 : -1;
							boomerang->SetState(BOOMERANG_STATE_MOVE_HORIZONTAL);
							boomerang->storeAmplitydeOy(boomerangBrother->y - BBOX_BIT / 2);
							boomerang->isAimToStatus = true;

							boomerangBrother->countBoomerangWeapon++;
							listFireBall.push_back(boomerang);
						}
					}
				}
			}
		}
		
		/*if (enemies[i]->isReCreated)
		{
			enemies[i]->ResetStart();
			enemies[i]->isReCreated = false;
		}
		if (checkObjOutOfCamera(enemies[i]))
		{
			enemies[i]->DisableObj_OutCamera();
		}
		if (enemies[i]->Check_DisableObj_ByOutCamera())
		{
			if (checkOrdinateOutOfCamera(enemies[i]->start_x, enemies[i]->start_y))
			{
				enemies[i]->EnableObj_InCamera();
				enemies[i]->isReCreated = true;
			}
		}*/

		enemies[i]->Update(dt, &staticObjects);
	}

	//player->CollideWithEnemy(enemies);
	// remove everything
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->objectDisappear())
		{
			objects.erase(objects.begin() + i);
		}
	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		if (effects[i]->objectDisappear())
		{
			effects.erase(effects.begin() + i);
			continue;
		}
	}

	for (size_t i = 0; i < listCoinTransform.size(); i++)
	{
		if (listCoinTransform[i]->objectDisappear())
		{
			listCoinTransform.erase(listCoinTransform.begin() + i);
			listWeakBrick.erase(listWeakBrick.begin() + i);
			continue;
		}
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->objectDisappear())
		{
			items.erase(items.begin() + i);
			continue;
		}
	}

	for (size_t i = 0; i < dynamicItems.size(); i++)
	{
		if (dynamicItems[i]->objectDisappear())
		{
			dynamicItems.erase(dynamicItems.begin() + i);
			continue;
		}
	}

	for (size_t i = 0; i < marioBullet.size(); i++)
	{
		if (marioBullet[i]->objectDisappear())
		{
			marioBullet.erase(marioBullet.begin() + i);
		}
	}

	for (size_t i = 0; i < listFireBall.size(); i++)
	{
		if (listFireBall[i]->objectDisappear())
		{
			listFireBall.erase(listFireBall.begin() + i);
		}
	}

	// isCompletedTransition: Complete Dark Screen Transition
	if (this->isCompletedTransition && player->CheckMarioSlideIntoPipe())
	{
		if (!player->isInHiddenMap && !this->isSpecialPipeContext)
		{
			// Get position of Pipe that Mario Push Out
			for (int i = 0; i < listPipe.size(); i++)
			{
				Pipe* pipe = dynamic_cast<Pipe*>(listPipe[i]);
				if (
					// Pipe is in HideMap
					pipe->isInHiddenMap
					&&
					// Pipe can push mario out (Pipe Output)
					pipe->isPushMarioOut
					)
				{

					player->SetPosition(listPipe[i]->x, listPipe[i]->y - 28);

					//// Turn on Light with transition
					this->isTransition_For_LightScreen = true;

					// Set Mario is in Hidden Map
					player->isInHiddenMap = true;

					// disable Moving into Pipe
					player->MarioSlideOutPipe();
					this->isCompletedTransition = false;

					//Set timeover for time, to use LightenScreen
					this->transitionBgTime->isStarted = false;

					player->SetState(MARIO_STATE_IDLE);

					break;
				}
			}
		}
		else if (player->isInHiddenMap || this->isSpecialPipeContext)
		{
			for (int i = 0; i < listPipe.size(); i++)
			{
				Pipe* pipe = dynamic_cast<Pipe*>(listPipe[i]);
				if (
					// Pipe is in HideMap
					!pipe->isInHiddenMap
					&&
					// Pipe can push mario out (Pipe Output)
					pipe->isPushMarioOut
					)
				{

					if (pipe->directionOy != 0) player->vy *= pipe->directionOy;

					player->SetPosition(listPipe[i]->x, listPipe[i]->y);
					player->storePosPipe_HaveHiddenMap(listPipe[i]->x, listPipe[i]->y);
					cam->SetCamPosX(2198.0f);
					// Turn on Light
					this->isTransition_For_LightScreen = true;

					// Set Mario is in Hidden Map
					player->isInHiddenMap = false;

					// This line comment because I want Mario push out slowly Pipe
					//player->MarioSlideOutPipe();

					this->isCompletedTransition = false;

					//Set timeover for time, to use LightenScreen
					this->transitionBgTime->isStarted = false;

					player->SetState(MARIO_STATE_IDLE);
					break;
				}
			}
		}
	}

	if (!this->isCompletedTransition && player->CheckMarioSlideIntoPipe())
	{
		if (!player->isInHiddenMap)
		{
			// Catch Event That position of Mario greater than position of Pipe when move in
			if (player->y > player->posY_OfPipe_HaveHiddenMap)
			{
				Alter_Opacity_AlphaForBackground_Screen_By_Time(true);
			}
		}
		else if (player->isInHiddenMap)
		{
			// Catch Event that position of Mario less than position of Pipe when move out
			if (player->y + 7.0f < player->posY_OfPipe_HaveHiddenMap)
			{
				Alter_Opacity_AlphaForBackground_Screen_By_Time(true);
			}
		}
			
	}
	if (this->isTransition_For_LightScreen)
	{
		Alter_Opacity_AlphaForBackground_Screen_By_Time(false);
	}


	cam->Update(dt,player->x);	
	boardGame->Update(dt, CGame::GetInstance()->cam_x, CGame::GetInstance()->cam_y + 220.0f);
	
	if (isTransitionScaleBg)
	{
		DetectScaleDarkScreenTransition(100);
	}
	
#pragma region End Game Render
	if (player->GetState() == MARIO_STATE_BLOCK_STATE_END_GAME)
	{
		if (!EndGameTime->isStarted)
		{
			EndGameTime->Start();
			firstRowString = new LettersEndGame(posOfTextEndGame_x - BBOX_BIT*4, posOfTextEndGame_y - BBOX_BIT*4 , "COURSES CLEAR");
		}
		else
		{
			if (EndGameTime->GetElapsedTime() >= 2200)
			{
				SwitchToSelectionWorld();
			}
			else if (EndGameTime->GetElapsedTime() >= 1500)
			{
				//if (!secondStringCreated)
				{
					secondRowString = new LettersEndGame(posOfTextEndGame_x - BBOX_BIT * 4 - BBOX_BIT/2, posOfTextEndGame_y - BBOX_BIT*3, "YOU GOT A CARD");
					boxCollectedItem = new BoxItem();
					boxCollectedItem->SetPosition(posOfTextEndGame_x + BBOX_BIT*3, posOfTextEndGame_y - BBOX_BIT*3 - BBOX_BIT / 2);
					itemCollected = new RandomItem();
					itemCollected->SetPosition(posOfTextEndGame_x + BBOX_BIT * 3 + BBOX_BIT / 4, posOfTextEndGame_y - BBOX_BIT * 3 - BBOX_BIT / 2 + BBOX_BIT / 4 + 2);
					itemCollected->isRandomizeState = false;
					itemCollected->typeItemCollected = player->listItemCollected[player->listItemCollected.size() - 1];
				}
			}
		}
	}
#pragma endregion

	//grid->ResetGrid(objects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL ) return; 

	
}

void CPlayScene::Render()
{
	map->Draw();




	for (size_t i = 0; i < enemies.size(); i++)
	{
		//if (enemies[i]->isRendered)
		{
			enemies[i]->Render();
		}
	}

	for (size_t i = 0; i < listCoinTransform.size(); i++)
	{
		listCoinTransform[i]->Render();
	}

	player->Render();

	for (size_t i = 0; i < dynamicItems.size(); i++)
	{
		dynamicItems[i]->Render();
	}

	for (size_t i = 0; i < listPortalStop.size(); i++)
	{
		listPortalStop[i]->Render();
	}
	if (player->isInWorldSelectionMap)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}
	}
			
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Render();
	}

	
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		if (staticObjects[i]->isRendered)
		{
			staticObjects[i]->Render();
		}

	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Render();
	}

	for (size_t i = 0; i < marioBullet.size(); i++)
	{
		marioBullet[i]->Render();
	}

	for (size_t i = 0; i < listFireBall.size(); i++)
	{
		listFireBall[i]->Render();
	}
		

	if (!this->isCompletedTransition && player->CheckMarioSlideIntoPipe())
	{
		if (player->y > player->posY_OfPipe_HaveHiddenMap)
		{
			DarkenTheScreen();
		}
		if (player->y + 7.0f < player->posY_OfPipe_HaveHiddenMap)
		{
			DarkenTheScreen();
		}
	}

	/// <summary>
	///  LightScreen first Game and Out of Pipe
	/// </summary>
	if (this->isTransition_For_LightScreen)
	{
		LightenTheScreen();
	}

	boardGame->Render();
	
	if (isTransitionScaleBg)
	{
		TransitionDarkScreen(18, 1 * indexScaleBackground, 0, 0);
		TransitionDarkScreen(1 * indexScaleBackground, 14, 0, 0);
		TransitionDarkScreen(18, -1 * indexScaleBackground, 0, 15 * 16);
		TransitionDarkScreen(-1 * indexScaleBackground, 14, 18 * 16, 0);
	}

#pragma region End Game Render
	if (player->GetState() == MARIO_STATE_BLOCK_STATE_END_GAME)
	{
		if (EndGameTime->isStarted)
		{
			firstRowString->Render();
			if (EndGameTime->GetElapsedTime() >= 1500)
			{
				secondRowString->Render();
				boxCollectedItem->Render();
				itemCollected->Render();
			}
		}
	}
#pragma endregion

}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	
	 mapObjects.clear();
	 listPortalStop.clear();
	
	 ObjectsInScreen.clear();
	 items.clear();
	 dynamicItems.clear();
	 enemies.clear();
	 staticObjects.clear();
	 listCBrick.clear();

	 effects.clear();
	 listFireBall.clear();
	 marioBullet.clear();
	 listPipe.clear();

	 delete Portal;

	//player = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (!scence->isWorldSeletion)
	{
		if (mario->GetState() == MARIO_STATE_DIE
			||
			mario->GetState() == MARIO_STATE_BLOCK_STATE_END_GAME
			) return;
		
		switch (KeyCode)
		{
			/*case (DIK_DOWN):
				mario->SetState(MARIO_STATE_STOP_SITTING);
				break;*/
		case (DIK_S):
				mario->isDownPressed = false;
				if (mario->level == MARIO_LEVEL_BIG_TAIL && !mario->isOnGround && mario->isFalling && mario->isEnteredFirstSpaceUp)
				{
					mario->vy = -MARIO_GRAVITY * mario->dt;
					mario->SetState(MARIO_STATE_BIG_TAIL_KEEP_JUMP);
					break;
				}
				//if (!isOnGround && !isFalling)
				if (!mario->isPreventedSpamSpace && !mario->CheckMarioSlideIntoPipe())
				{
					mario->vy = mario->vy + MARIO_GRAVITY * mario->dt * 10;
					if (mario->vy > 0 && mario->vy >= 0.1f) mario->vy = 0.1f;
					mario->isEnteredFirstSpaceUp = true;
					mario->isPreventedSpamSpace = true;
				}
			break;
		case (DIK_A):
			mario->isAttackPress = false;
			if (mario->isCanHoldingKoopas)
			{
				mario->isCanHoldingKoopas = false;
			}
			mario->SetState(MARIO_STATE_STOP_RUNNING);
			break;
		case (DIK_RIGHT):
			mario->SetState(MARIO_STATE_NOT_WALKING);
			break;
		case(DIK_LEFT):
			mario->SetState(MARIO_STATE_NOT_WALKING);
			break;
		default:
			break;
		}
	}
	else if (scence->isWorldSeletion) {
		if (KeyCode)
		{
			if (KeyCode == DIK_S)
			{
				mario->isCanGetIntoWorldMap = false;
			}
		}
		
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
		
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (scence->isWorldSeletion)
	{
		if (KeyCode)
		{
			if (mario->isWalking) 
				return;

			if (KeyCode == DIK_RIGHT)
			{
				if (!mario->isMovedToRight) return;
				mario->SetState(MARIO_SELECTION_MAP_STATE_MOVE_RIGHT);
			}
			else if (KeyCode == DIK_LEFT)
			{
				if (!mario->isMovedToLeft) return;
				mario->SetState(MARIO_SELECTION_MAP_STATE_MOVE_LEFT);

			}
			else if (KeyCode == DIK_DOWN)
			{
				if (!mario->isMovedToBottom) return;
				mario->SetState(MARIO_SELECTION_MAP_STATE_MOVE_BOTTOM);

			}
			else if (KeyCode == DIK_UP)
			{
				if (!mario->isMovedToTop) return;
				mario->SetState(MARIO_SELECTION_MAP_STATE_MOVE_TOP);
			}
			else if (KeyCode == DIK_S)
			{
				mario->isCanGetIntoWorldMap = true;
			}
		}
	}
	else if (!scence->isWorldSeletion)
	{
		if (mario->GetState() == MARIO_STATE_DIE
			||
			mario->GetState() == MARIO_STATE_BLOCK_STATE_END_GAME
			||
			mario->CheckMarioSlideIntoPipe()
			) return;

		switch (KeyCode)
		{
		case DIK_1:
			mario->SetLevel(1);
			break;
		case DIK_2:
			mario->SetLevel(2);
			break;
		case DIK_3:
			mario->SetLevel(3);
			break;
		case DIK_4:
			mario->SetLevel(4);
			break;
		case DIK_Z:
			mario->Reset();
			break;
		case DIK_S:
		{
			mario->SetState(MARIO_STATE_JUMP);
			break;
		}
		case DIK_A:
			mario->isAttackPress = true;
			if (mario->level == MARIO_LEVEL_BIG_TAIL)
			{
				if (mario->isAttacking && (mario->animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_LEFT)->IsRenderOver(375)
					|| mario->animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT)->IsRenderOver(375)))
					return;
				mario->isAttacking = true;
				mario->setTailPos();
				mario->SetState(MARIO_STATE_BIG_ATTACK);
				break;
			}
			else if (mario->level == MARIO_LEVEL_BIG_FIRE && !mario->isFireShoot)
			{
				if (mario->isAttacking && (mario->animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT)->IsRenderOver(100)
					|| mario->animation_set->at(MARIO_ANI_BIG_FIRE_ATTACKING_LEFT)->IsRenderOver(100)))
					return;

				if (!mario->isOnGround)
				{
					mario->isJumpingAttack = true;
				}

				mario->isAttacking = true;
				mario->isFireShoot = true;
				mario->SetState(MARIO_STATE_BIG_ATTACK);
				break;
			}
			else {
				mario->isAttacking = true;
			}
			break;
		case DIK_DOWN:
			mario->isDownPressed = true;
			break;
		default:
			break;
		}
	}
	
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (!scence->isWorldSeletion)
	{
		// disable control key when Mario die 
		if (mario->GetState() == MARIO_STATE_DIE
			||
			mario->GetState() == MARIO_STATE_BLOCK_STATE_END_GAME
			||
			mario->CheckMarioSlideIntoPipe()
			) return;

		if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_S))
		{
			if (mario->isOnGround && mario->imMovable)
				mario->SetState(MARIO_STATE_RUNNING);
		}
		if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_S))
		{
			if (mario->isOnGround && mario->imMovable)
				mario->SetState(MARIO_STATE_RUNNING);
		}
		if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			if (mario->isCanHoldingKoopas) mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT))
		{
			mario->SetState(MARIO_STATE_WALKING_LEFT);
			if (mario->isCanHoldingKoopas) mario->nx = -1;
		}
		else if (game->IsKeyDown(DIK_DOWN))
		{
			mario->SetState(MARIO_STATE_SITTING);
		}
		else
			mario->SetState(MARIO_STATE_IDLE);
	}

	
}