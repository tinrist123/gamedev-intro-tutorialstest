#include <vector>
#include <iostream>
using namespace std;

#define MARIO_RUNNING_PRE_MAX_SPEED						0.2f
#define MARIO_RUNNING_MAX_SPEED							0.3f

#define MARIO_WALKING_DECELERATION						0.006875f
#define MARIO_WALKING_ACCELEROMETER						0.001875f
#define MARIO_MAX_WALKING_SPEED							0.1f
#define FRICTION										0.0056875f

#define MARIO_JUMP_SPEED_Y								0.3f
#define MARIO_JUMP_MAX_SPEED_Y							0.3f
#define MARIO_JUMP_DEFLECT_SPEED						0.2f
#define MARIO_GRAVITY									0.0005f
#define MARIO_DIE_DEFLECT_SPEED							0.1f

#define MARIO_STATE_IDLE								0
#define MARIO_STATE_WALKING_RIGHT						100
#define MARIO_STATE_WALKING_LEFT						200
#define MARIO_STATE_JUMP								300
#define MARIO_ANI_SHORT_JUMP							301
#define MARIO_STATE_BIG_TAIL_KEEP_JUMP					302
#define MARIO_STATE_DIE									400
#define MARIO_STATE_SITTING								500
#define MARIO_STATE_STOP_SITTING						501
#define MARIO_STATE_WALKING								600
#define MARIO_STATE_NOT_WALKING							601
#define MARIO_STATE_RUNNING								700
#define MARIO_STATE_STOP_RUNNING						701
#define MARIO_STATE_BIG_TAIL_ATTACK						800

// 66

//================================IDLE CONSTANT ZONE=================================
#define MARIO_ANI_BIG_IDLE_RIGHT						0
#define MARIO_ANI_BIG_IDLE_LEFT							1
#define MARIO_ANI_SMALL_IDLE_RIGHT						2
#define MARIO_ANI_SMALL_IDLE_LEFT						3
#define MARIO_ANI_BIG_TAIL_IDLE_RIGHT					19
#define MARIO_ANI_BIG_TAIL_IDLE_LEFT					20
#define MARIO_ANI_BIG_FIRE_IDLE_RIGHT					69
#define MARIO_ANI_BIG_FIRE_IDLE_LEFT					70
//================================END IDLE CONSTANT ZONE=================================

//================================WALKING CONSTANT ZONE=================================
#define MARIO_ANI_BIG_WALKING_RIGHT						4
#define MARIO_ANI_BIG_WALKING_LEFT						5

#define MARIO_ANI_BIG_TAIL_WALKING_RIGHT				21
#define MARIO_ANI_BIG_TAIL_WALKING_LEFT					22

#define MARIO_ANI_BIG_FIRE_WALKING_RIGHT				51
#define MARIO_ANI_BIG_FIRE_WALKING_LEFT					52

#define MARIO_ANI_SMALL_WALKING_RIGHT					6
#define MARIO_ANI_SMALL_WALKING_LEFT					7
//================================END WALKING CONSTANT ZONE=================================

//================================RUNNING CONSTANT ZONE=================================
#define MARIO_ANI_BIG_RUNNING_RIGHT						37
#define MARIO_ANI_BIG_RUNNING_LEFT						38

#define MARIO_ANI_BIG_TAIL_RUNNING_RIGHT				39
#define MARIO_ANI_BIG_TAIL_RUNNING_LEFT					40

#define MARIO_ANI_BIG_FIRE_RUNNING_RIGHT				63
#define MARIO_ANI_BIG_FIRE_RUNNING_LEFT					64

#define MARIO_ANI_SMALL_RUNNING_RIGHT					35
#define MARIO_ANI_SMALL_RUNNING_LEFT					36
//================================END RUNNING CONSTANT ZONE=================================


//================================SITTING CONSTANT ZONE=================================
#define MARIO_ANI_DIE									8
#define MARIO_ANI_BIG_SITTING_LEFT						9
#define MARIO_ANI_BIG_SITTING_RIGHT						10

#define MARIO_ANI_BIG_TAIL_SITTING_RIGHT				23
#define MARIO_ANI_BIG_TAIL_SITTING_LEFT					24

#define MARIO_ANI_BIG_FIRE_SITTING_RIGHT				53
#define MARIO_ANI_BIG_FIRE_SITTING_LEFT					54
//================================END SITTING CONSTANT ZONE=================================


//================================STOP CONSTANT ZONE=================================
#define MARIO_ANI_BIG_WALKING_STOP_LEFT					13
#define MARIO_ANI_BIG_WALKING_STOP_RIGHT				14	

#define MARIO_ANI_BIG_TAIL_WALKING_STOP_LEFT			25
#define MARIO_ANI_BIG_TAIL_WALKING_STOP_RIGHT			26

#define MARIO_ANI_BIG_FIRE_WALKING_STOP_LEFT			55
#define MARIO_ANI_BIG_FIRE_WALKING_STOP_RIGHT			56

#define MARIO_ANI_SMALL_WALKING_STOP_LEFT				16
#define MARIO_ANI_SMALL_WALKING_STOP_RIGHT				15	
//================================END STOP CONSTANT ZONE=================================


//================================FLYING CONSTANT ZONE=================================
#define MARIO_ANI_BIG_FLYING_RIGHT						11
#define MARIO_ANI_BIG_FLYING_LEFT						12

#define MARIO_ANI_SMALL_FLYING_RIGHT					17	
#define MARIO_ANI_SMALL_FLYING_LEFT						18

#define MARIO_ANI_BIG_TAIL_FLYING_RIGHT					27
#define MARIO_ANI_BIG_TAIL_FLYING_LEFT					28

#define MARIO_ANI_BIG_FIRE_FLYING_RIGHT					57
#define MARIO_ANI_BIG_FIRE_FLYING_LEFT					58
//================================END FLYING CONSTANT ZONE=================================

//================================MARIO FALLING CONSTANT ZONE=================================
#define MARIO_ANI_BIG_FALLING_RIGHT						29
#define MARIO_ANI_BIG_FALLING_LEFT						30

#define MARIO_ANI_BIG_TAIL_FALLING_RIGHT				31
#define MARIO_ANI_BIG_TAIL_FALLING_LEFT					32

#define MARIO_ANI_BIG_FIRE_FALLING_RIGHT				59
#define MARIO_ANI_BIG_FIRE_FALLING_LEFT					60
//================================END MARIO FALLING CONSTANT ZONE=================================


//================================MARIO ATTACKING CONSTANT ZONE=================================
#define MARIO_ANI_BIG_TAIL_ATTACKING_RIGHT				33
#define MARIO_ANI_BIG_TAIL_ATTACKING_LEFT				34

#define MARIO_ANI_BIG_FIRE_ATTACKING_RIGHT				61
#define MARIO_ANI_BIG_FIRE_ATTACKING_LEFT				62

//================================FLYING WHEN RUNNING CONSTANT ZONE=================================
#define MARIO_ANI_BIG_RUNNING_FLYING_RIGHT				41
#define MARIO_ANI_BIG_RUNNING_FLYING_LEFT				42

#define MARIO_ANI_BIG_TAIL_RUNNING_FLYING_RIGHT			43
#define MARIO_ANI_BIG_TAIL_RUNNING_FLYING_LEFT			44

#define MARIO_ANI_BIG_FIRE_RUNNING_FLYING_RIGHT			65
#define MARIO_ANI_BIG_FIRE_RUNNING_FLYING_LEFT			66

#define MARIO_ANI_SMALL_RUNNING_FLYING_RIGHT			45
#define MARIO_ANI_SMALL_RUNNING_FLYING_LEFT				46
//================================END FLYING WHEN RUNNING  ZONE=================================


//================================FALLING  SLOWNLY WHEN SPACE ENTER CONSTANT ZONE=================================
#define MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_RIGHT	47
#define MARIO_ANI_BIG_TAIL_SLOW_FALLING_FLYING_LEFT		48
//================================END FALLING  SLOWNLY WHEN SPACE ENTER   ZONE=================================


//================================RUNNING FAILLING WHEN SPACE=================================
#define MARIO_ANI_BIG_TAIL_RUNNING_FALLING_RIGHT		49
#define MARIO_ANI_BIG_TAIL_RUNNING_FALLING_LEFT			50
//================================END RUNNING FAILLING  ZONE=================================


//================================RUNNING FAILLING WHEN SPACE=================================
#define MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_RIGHT		67
#define MARIO_ANI_BIG_FIRE_FLYING_ATTACKING_LEFT		68
//================================END RUNNING FAILLING  ZONE=================================

//================================MARIO LEVEL CONSTANT ZONE=================================
#define	MARIO_LEVEL_SMALL								1
#define	MARIO_LEVEL_BIG									2
#define	MARIO_LEVEL_BIG_TAIL							3
#define	MARIO_LEVEL_BIG_FIRE							4


//================================MARIO RENDER BOX CONSTANT ZONE=================================
#define MARIO_BIG_BBOX_WIDTH							15
#define MARIO_BIG_BBOX_HEIGHT							28

#define MARIO_BIG_TAIL_BBOX_WIDTH						23
#define MARIO_BIG_TAIL_BBOX_HEIGHT						27

#define MARIO_BIG_BBOX_SITTING_HEIGHT					19
#define MARIO_BIG_TAIL_BBOX_SITTING_HEIGHT				20

#define MARIO_SMALL_BBOX_WIDTH							13
#define MARIO_SMALL_BBOX_HEIGHT							15


//================================ANOTHER CONSTANT ZONE=================================
#define MARIO_UNTOUCHABLE_TIME							1000
#pragma once
class Constant
{
	int kindMario;
public:
	Constant(int kindMario);
	void changeLevelMario(int level);
	vector<int> listAni_Mario_Big;
	vector<int> listBBox_Mario_Big;
	vector<int> getBBox_MARIO_BIG_HAVE_TAIL();
	vector<int> getBBox_MARIO_BIG();
	vector<int> getAnimationMARIO_BIG_HAVE_TAIL();
	vector<int> getAnimationMARIO_BIG_HAVE_FIRE();
	vector<int> getAnimationMARIO_BIG();
};

