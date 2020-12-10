#include <iostream>
#include <fstream>
#include "PlayScence.h"
#include "P_Switch.h"


using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
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
		//DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

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
	//grid->PushGrid(objects);
	map = new TileMap(ID, filePath_texture.c_str(), filePath_data.c_str(), num_row_on_texture, num_col_on_textture, num_row_on_tilemap, num_col_on_tilemap, tileset_width, tileset_height);
	boardGame = new BoardGame(player);
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
		if (player!=NULL) 
		{
			//DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		else
		{
			obj = new CMario(x,y); 
			player = (CMario*)obj;  
			player->SetAnimationSet(ani_set);
			player->SetPosition(x, y);
			cam = new Camera(player);
			return;
		}
		//DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		int typeGoomba = atof(tokens[4].c_str());
		obj = new CGoomba(typeGoomba); 
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
			int isHaveItem = atof(tokens[6].c_str());
		obj = new WeakBrick(width, height, isHaveItem);
		break;
	}
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
		obj = new CKoopas(typeKoopas, typeColorKoopas);
		break;
	}
	case OBJECT_TYPE_Flower:
	{
		float HeightPipe = atof(tokens[4].c_str());
		obj = new CFlower(y, HeightPipe);
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
		obj = new Pipe(width, height);
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
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		//DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetAnimationSet(ani_set);
	obj->SetPosition(x, y);

	objects.push_back(obj);
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


Item *CreateItemOfMario(CMario *player, QuestionBrick *object)
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
		//int kindAni = (player->level < MARIO_LEVEL_BIG_TAIL) ? ITEM_ANI_MUSHROOM : 2;
		int kindAni = 2;
		if (kindAni == 2)
		{
			Leaf *leaf = new Leaf(object->x, object->y);
			leaf->SetPosition(object->x, object->y);
			if (player->x > (leaf->x)) leaf->nx = -1;
			else if (player->x < (leaf->x)) leaf->nx = 1;
			leaf->SetState(ITEM_STATE_LEAF_FALL);
			return leaf;
		}
		else if (kindAni == ITEM_ANI_MUSHROOM)
		{
			Mushroom *mushroom = new Mushroom(object->start_x, object->start_y);
			mushroom->SetPosition(object->x, object->y);
			if (player->x > (mushroom->x )) mushroom->nx = -1;
			else if (player->x < (mushroom->x )) mushroom->nx = 1;
			mushroom->SetState(ITEM_STATE_SPECIAL_ITEM);
			return mushroom;
		}
		else if (kindAni == 3)
		{
			P_Switch* p = new P_Switch(object->start_x, object->start_y);
			p->SetPosition(object->x, object->y);
			p->SetState(ITEM_STATE_SPECIAL_ITEM);
			return p;
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


// special item mean mushroom or leaf or more
void CPlayScene::playerHittingSpecialItem(LPGAMEOBJECT& item)
{
	item->SetState(ITEM_STATE_HITTING_MARIO);
	player->level = (player->level >= MARIO_LEVEL_BIG_TAIL) ? MARIO_LEVEL_BIG_TAIL : ++player->level;
	player->constant->changeLevelMario(player->level);
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
	float RangeOxRight = cam_x + SCREEN_WIDTH;
	float RangeOxLeft = cam_x;
	float RangeOyTop = cam_y + SCREEN_HEIGHT;
	float RangeOyBottom = cam_y;

	if (obj->x > RangeOxRight
		||
		obj->x < RangeOxLeft
		||
		obj->y > RangeOyTop
		||
		obj->y < RangeOyBottom
		)
	{
		return true;
	}

	return false;
}

void CPlayScene::removeObjOutOfCamera(LPGAMEOBJECT& obj)
{
	float cam_x = CGame::GetInstance()->GetCamPosX();
	float cam_y = CGame::GetInstance()->GetCamPosY();
	float RangeOxRight = SCREEN_WIDTH - cam_x;
	float RangeOxLeft = player->x - cam_x;
	float RangeOyTop = cam_y + SCREEN_HEIGHT;
	float RangeOyBottom = cam_y;
	if (obj->GetX() - player->x >= SCREEN_WIDTH - (player->x - CGame::GetInstance()->GetCamPosX())
		|| player->x - obj->GetX() >= player->x - CGame::GetInstance()->GetCamPosX()
		||
		obj->y > RangeOyTop
		||
		obj->y < RangeOyBottom
		)
	{
		obj->setObjDisappear();
	}
	

}


void CPlayScene::GetObjectGrid()
{
	ObjectsInScreen.clear();
	items.clear();
	staticObjects.clear();
	listCBrick.clear();
	enemies.clear();
	//effects.clear();
	grid->GetGrid(ObjectsInScreen);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	float playerPositionX = player->x;
	float playerPositionY = player->y;

	grid->CheckCamGrid(objects);
	GetObjectGrid();


	for (size_t i = 0; i < ObjectsInScreen.size(); i++)
	{
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
			if (ObjectsInScreen[i]->getTypeObject() == Type::SUPER_LEAF)
			{
				dynamicItems.push_back(ObjectsInScreen[i]);
			}
			else if (ObjectsInScreen[i]->getTypeObject() == Type::SUPER_MUSHROOM)
			{
				dynamicItems.push_back(ObjectsInScreen[i]);
			}
			else if (ObjectsInScreen[i]->getTypeObject() == Type::COIN)
			{
				items.push_back(ObjectsInScreen[i]);
			}
		}
		else if (ObjectsInScreen[i]->getCategoryObject() == Category::BRICK)
		{
			listCBrick.push_back(ObjectsInScreen[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < items.size(); i++)
	{
		coObjects.push_back(items[i]);
	}
	
	//cam->Update(dt);

	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (fabs(enemies[i]->x - playerPositionX) < 90.0f && enemies[i]->getTypeObject() == Type::FLOWER)
		{
			CFlower* flower = dynamic_cast<CFlower*>(enemies[i]);

			if (flower->state == FLOWER_STATE_UP)
			{
				if (flower->isWaitingShooting && flower->y == 336.0f)
				{
					flower->isShooting = true;
					if (flower->isReadyFire() && flower->delayBullet != 0)
					{
						flower->isWaitingShooting = false;
						flowerBullet.push_back(CreateFlowerBullet(flower));
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
		else if (dynamic_cast<CKoopas*>(objects[i]) && objects[i]->isAliveObject())
		{
			CKoopas* koopas = dynamic_cast<CKoopas*>(objects[i]);

			/*if (koopas->isCreatePointEffect)
			{
				koopas->addPointToItem();
				effects.push_back(koopas->effectPoint);
			}*/

			if (!player->isCanHoldingKoopas && koopas->isOutOfControl)
			{
				player->ChainKickKoopas(koopas, false);
			}
			if (player->isCanHoldingKoopas && koopas->isOutOfControl)
			{
				// detect Nx to accurately hold koopasf
				float distanceKoopasByOy = (player->level == MARIO_LEVEL_SMALL) ? 14 : 0;
				float distanceKoopasbyOx = (player->nx == 1) ? 14 : -5;
				if (player->level == MARIO_LEVEL_SMALL)
				{
					distanceKoopasbyOx = (player->nx == 1) ? 10 : -8;

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
		} 
		else if (dynamic_cast<CGoomba*>(objects[i]) && objects[i]->health == 2)
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(objects[i]);
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
	}

	for (size_t i = 0; i < listCBrick.size(); i++)
	{
		if (listCBrick[i]->health == 0 && dynamic_cast<QuestionBrick*>(listCBrick[i]) && !listCBrick[i]->isCreated)
		{
			QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*>(listCBrick[i]);
			// TODO: EFFECT DISAPEAR SO COIN MUST DISAPEAR TOO, so Calculate it
			// DONE
			dynamicItems.push_back(CreateItemOfMario(player, questionBrick));
		}
	}

	//for (size_t i = 0; i < objects.size(); i++)
	//{
	//	/*if (objects[i]->objectDisappear())
	//	{
	//		objects.erase(objects.begin() + i);

	//	}*/
	//	if (dynamic_cast<Enemy*>(objects[i]))
	//	{
	//		player->AccurateCollisionWithEnemy(objects[i]);
	//	}
	
	//	// K Update thg nao bien mat
	//	if (objects[i]->health == 0 && objects[i]->isDisappeared)
	//	{
	//		if (dynamic_cast<MarioBullet*>(objects[i]) && !(objects[i])->isAddedEffect)
	//		{
	//			(objects[i])->isAddedEffect = true;
	//			MarioBullet* marioBullet = dynamic_cast<MarioBullet*>(objects[i]);
	//			EffectsFire* effect = new EffectsFire();
	//			effect->SetState(EFFECTS_DEFLECT);
	//			effect->SetPosition(marioBullet->pointCollisionX, marioBullet->pointCollisionY);
	//			effects.push_back(effect);
	//		}
	//		continue;
	//	}
	//	if (player->isFireShoot)
	//	{
	//		MarioBullet* bullet = new MarioBullet();
	//		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	//		LPANIMATION_SET ani_set = animation_sets->Get(7);


	//		if (player->IsReloadedBullets() && player->timeReload != 0)
	//		{
	//			player->totalOfBulletsReadyForFiring = 2;
	//			player->timeReload = 0;
	//			player->resetAutoReload();
	//		}
	//		if (player->isReloaded() && player->autoReloadTime != 0)
	//		{
	//			player->totalOfBulletsReadyForFiring = 2;
	//			player->timeReload = 0;
	//			player->autoReloadTime = 0;
	//		}
	//		if (player->totalOfBulletsReadyForFiring > 0)
	//		{
	//			bullet->SetAnimationSet(ani_set);
	

	//			player->totalOfBulletsReadyForFiring--;
	//			player->setAutoReload();
	//			player->isFireShoot = false;

	//			objects.push_back(bullet);
	//		}
	//		if (player->totalOfBulletsReadyForFiring == 0 && player->timeReload == 0)
	//		{
	//			player->ReloadBullets();
	//		}
	//	}

	//	// 1 Mean Koopas is Shell appearance
	//	if (dynamic_cast<CKoopas*>(objects[i]) && objects[i]->isAliveObject() )
	//	{
	//		CKoopas* koopas = dynamic_cast<CKoopas*>(objects[i]);

	//		/*if (koopas->isCreatePointEffect)
	//		{
	//			koopas->addPointToItem();
	//			effects.push_back(koopas->effectPoint);
	//		}*/

	//		if (!player->isCanHoldingKoopas && koopas->isOutOfControl)
	//		{
	//			player->ChainKickKoopas(koopas, false);
	//		}
	//		if (player->isCanHoldingKoopas &&  koopas->isOutOfControl)
	//		{
	//			// detect Nx to accurately hold koopasf
	//			float distanceKoopasByOy = (player->level == MARIO_LEVEL_SMALL)?14:0;
	//			float distanceKoopasbyOx = (player->nx == 1) ? 14 : -5;
	//			if (player->level == MARIO_LEVEL_SMALL)
	//			{
	//				distanceKoopasbyOx = (player->nx == 1) ? 10 : -8;
	//				
	//			}
	//			// Hold koopas
	//			koopas->updatePositionKoopasByPositionMario(player->x + distanceKoopasbyOx, player->y - distanceKoopasByOy);
	//		}
	//		else if (player->isCanHoldingKoopas && !koopas->isOutOfControl && koopas->isPlayerHolding)
	//		{
	//			// When koopas revive, player Cant hold it
	//			player->isCanHoldingKoopas = false; 
	//			koopas->isPlayerHolding = false;
	//		}
	//	}

	//	// HEALTH == 2 mean, goomba have wing
	//	if (dynamic_cast<CGoomba*>(objects[i]) && objects[i]->health == 2)
	//	{
	//		CGoomba* goomba = dynamic_cast<CGoomba*>(objects[i]);
	//		float goombaPositionX = goomba->x;
	//		float goombaPositionY = goomba->y;
	//		if (goomba->level == PARAGOOMBA)
	//		{
	//			goomba->followPlayerByNx(((goombaPositionX - playerPositionX) > 0)?-1:1);
	//			if (fabs(goombaPositionX - playerPositionX) < 250.0f 
	//				&& !goomba->IsBlockingChangeState
	//				&& goomba->health == 2
	//				)
	//			{
	//				// Become crazy goomba and it have 3 state change circle
	//				goomba->blockingChangeState(true);
	//				goomba->startTimeChangeState();
	//			}
	//		}
	//		

	//	}

	//	objects[i]->Update(dt, &coObjects);
	//}


	

	{
		if (player->isFireShoot)
		{
			if (marioBullet.size() < 2)
			{
				player->bullet = new MarioBullet();
				player->bullet->x = player->x + 5*player->nx;
				player->bullet->y = player->y;
				player->bullet->nx = player->nx;
				player->bullet->SetState(BULLET_STATE_NORMAL_MOVING);
				marioBullet.push_back(player->bullet);
			}
			player->isFireShoot = false;
		}
		player->Update(dt,&objects);
	}

	for (size_t i = 0; i < listCBrick.size(); i++)
	{
		listCBrick[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < marioBullet.size(); i++)
	{
		marioBullet[i]->Update(dt, &coObjects);
		this->removeObjOutOfCamera(marioBullet[i]);
	}
	
	for (size_t i = 0; i < flowerBullet.size(); i++)
	{
		flowerBullet[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt, &coObjects);
	}
	
	for (size_t i = 0; i < dynamicItems.size(); i++)
	{
		if (dynamicItems[i]->getCategoryObject() == Category::ITEM)
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
		dynamicItems[i]->Update(dt, &coObjects);
	}
	
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt, &coObjects);
	}

	player->CollideWithItem(items, effects);
	player->CollideWithItem(dynamicItems,effects);

	for (size_t i = 0; i < enemies.size(); i++)
	{
		// Flower Fire Mario
		enemies[i]->Update(dt, &coObjects);
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

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->objectDisappear())
		{
			objects.erase(objects.begin() + i);
		}
	}
	
	for (size_t i = 0; i < marioBullet.size(); i++)
	{
		if (marioBullet[i]->objectDisappear())
		{
			marioBullet.erase(marioBullet.begin() + i);
		}
	}

	for (size_t i = 0; i < flowerBullet.size(); i++)
	{
		if (flowerBullet[i]->objectDisappear())
		{
			flowerBullet.erase(flowerBullet.begin() + i);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	/*CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;*/
	CGame::GetInstance()->cam_y = 200.0f;


	if (player->x > (SCREEN_WIDTH / 4) && player->x + (SCREEN_WIDTH / 4) < map->GetWidthTileMap())
	{
		cx = player->x - (SCREEN_WIDTH / 4);
  		CGame::GetInstance()->cam_x = cx;
	}
	float cam_y = 432;
	boardGame->Update(dt, CGame::GetInstance()->cam_x, cam_y);
}

void CPlayScene::Render()
{
	//mapScence->DrawMap();
	map->Draw();
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		staticObjects[i]->Render();
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Render();
    }

	for (size_t i = 0; i < enemies.size(); i++)
	{
 		enemies[i]->Render();
	}

	for (size_t i = 0; i < listCBrick.size(); i++)
	{
		listCBrick[i]->Render();
	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Render();
	}

	for (size_t i = 0; i < dynamicItems.size(); i++)
	{
		dynamicItems[i]->Render();
	}

	for (size_t i = 0; i < marioBullet.size(); i++)
	{
		marioBullet[i]->Render();
	}

	for (size_t i = 0; i < flowerBullet.size(); i++)
	{
		flowerBullet[i]->Render();
	}

	player->Render();
	boardGame->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	//DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}



void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE) return;
	

	switch (KeyCode)
	{
	/*case (DIK_DOWN):
		mario->SetState(MARIO_STATE_STOP_SITTING);
		break;*/
	case (DIK_SPACE):
		if (mario->level == MARIO_LEVEL_BIG_TAIL && !mario->isOnGround && mario->isFalling && mario->isEnteredFirstSpaceUp)
		{
			mario->vy = -MARIO_GRAVITY * mario->dt;
			mario->SetState(MARIO_STATE_BIG_TAIL_KEEP_JUMP);
			break;
		}
		//if (!isOnGround && !isFalling)
		if (!mario->isPreventedSpamSpace)
		{
			mario->vy = mario->vy + MARIO_GRAVITY*mario->dt * 10;
			mario->isEnteredFirstSpaceUp = true;
			mario->isPreventedSpamSpace = true;
		}
		//mario->SetState(MARIO_ANI_SHORT_JUMP);
		break;
	case (DIK_LSHIFT):
		mario->SetState(MARIO_STATE_STOP_RUNNING);
		break;
	case (DIK_RIGHT):
		mario->SetState(MARIO_STATE_NOT_WALKING);
		break;
	case(DIK_LEFT):
		mario->SetState(MARIO_STATE_NOT_WALKING);
		break;
	case (DIK_Z):
		mario->isAttackPress = false;
		if (mario->isCanHoldingKoopas)
		{
			mario->isCanHoldingKoopas = false;
		}
		break;
	default:
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE) return;
	

	switch (KeyCode)
	{
	/*case DIK_SPACE:
		if (mario->level == MARIO_LEVEL_BIG_TAIL)
		{
			mario->SetState(MARIO_STATE_BIG_TAIL_KEEP_JUMP);
		}
		break;*/
	case DIK_Z:
		//mario->isCanHoldingKoopas = true;
		mario->isAttackPress = true;
		if ( mario->level == MARIO_LEVEL_BIG_TAIL )
		{
			if(mario->isAttacking && (mario->animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_LEFT)->IsRenderOver(375)
			|| mario->animation_set->at(MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT)->IsRenderOver(375)))
				return;

			mario->setTailPos();
			mario->isAttacking = true;
			mario->SetState(MARIO_STATE_BIG_ATTACK);
			break;
		}
		else if (mario->level == MARIO_LEVEL_BIG_FIRE && mario->IsReloadedBullets() && !mario->isFireShoot)
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
	case DIK_SPACE:
	{
		mario->SetState(MARIO_STATE_JUMP);
		break;
	}
	case DIK_A: 
		mario->Reset();
		CGame::GetInstance()->cam_x = 0;
		break;
	default:
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (game->IsKeyDown(DIK_LSHIFT) && game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_SPACE))
	{
		if (mario->isOnGround && mario->imMovable)
			mario->SetState(MARIO_STATE_RUNNING);
	}
	if (game->IsKeyDown(DIK_LSHIFT) && game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_SPACE))
	{
		if (mario->isOnGround && mario->imMovable)
			mario->SetState(MARIO_STATE_RUNNING);
	}
	if(game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		if (mario->isCanHoldingKoopas) mario->nx = 1;
		//return;
	}
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		if (mario->isCanHoldingKoopas) mario->nx = -1;
		//return;
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_SITTING);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}