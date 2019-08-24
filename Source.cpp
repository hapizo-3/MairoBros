#include "DxLib.h"

//�f�o�b�O���[�h
#define _DEBUGMODE

//�E�B���h�E���[�h�ύX
//(��{�̓E�B���h�E���[�h)
//0���S��ʃ��[�h�A1���E�B���h�E���[�h
#define _WINDOWMODE	1

//�t���[�����[�g�萔( 60 )
#define	_FRAMERATE_60	60

/*****      �t�H���g�T�C�Y�萔      *****/
#define _FONTSIZE_SS		10	//�t�H���gSS�T�C�Y
#define _FONTSIZE_S			20	//�t�H���gS�T�C�Y
#define _FONTSIZE_M			32	//�t�H���gM�T�C�Y
#define _FONTSIZE_L			48	//�t�H���gL�T�C�Y
#define _FONTSIZE_LL		64	//�t�H���gLL�T�C�Y

/*****		�}�X�s�N�Z����		*****/
#define _MASS_X		32
#define _MASS_Y		32
#define _MASS_HALF	16

/*****		�}�b�v�̍���		*****/
#define _MAP_X		16
#define _MAP_Y		14

//�}�b�v�S�T�C�Y
#define _MAP_ALLSIZE_X 220
#define _MAP_ALLSIZE_Y 14

//�����蔻��̂���u���b�N�̐�
#define _HITBLOCK	25

//�����蔻�萧��萔
#define _HIT_FALSE	0
#define _HIT_TRUE	1
#define _HIT_BREAK	2
#define _HIT_BRING	3

#define _HIT_RIGHT	4
#define _HIT_LEFT	5
#define _HIT_ITEM	6
#define _HIT_GOAL	7
#define _HIT_CASTLE	8

#define _HITW_PLAYER	0	//�q�b�g����-�v���C���[
#define _HITW_PWRUP		1	//�q�b�g����-�p���[�A�b�v�L�m�R
#define _HITW_FBALL		2	//�q�b�g����-�t�@�C���[�{�[��
#define _HITW_ENEMY		3	//�q�b�g����-�G
#define _HITW_FLAG		4	//�q�b�g����-��

//�����̒萔
#define _DIRECT_RIGHT	0
#define _DIRECT_LEFT	1

//�}���I���
#define _MARIO_NOMAL	0
#define _MARIO_SUPER	1
#define _MARIO_FIRE		2

//�A�C�e����Ԓ萔
#define _ITEM_NHAVE		0	//�����ĂȂ�
#define _ITEM_BRING		1	//�����Ă���
#define _ITEM_ANIME		2	//�A�j���[�V�����t���O
#define _ITEM_MOVIN		3	//�A�C�e���������Ă���(�L�m�R�Ȃ�)

#define _ITEMT_COIN		5	//�R�C��
#define _ITEMT_MCOIN	6	//���񂩉�����R�C��
#define _ITEMT_PWRUP	7	//�p���[�A�b�v�A�C�e��

//�Q�[����ԕϐ�
static int GAMESTATE;

//���t���b�V�����[�g�m�F�p�ϐ�
static int RefreshRate;
HDC hdc;

/*****      �Q�[�����[�h�񋓑�      *****/
typedef enum GAME_MODE {
	GAME_TITLE,			//�^�C�g��
	GAME_INIT,			//������
	GAME_MAIN,			//���C��
	GAME_GOAL,			//�S�[�����(�J�����)
	GAME_END,			//�G���h����
	END = 99			//�G���h
};

/*****      �Q�[�����[�h�񋓑�      *****/
//typedef enum BLOCK_NUMBER {
	

/*****      �摜�\����      *****/
typedef struct PICTURE {
	int Player[ 15 ];
	int Suplayer[ 15 ];
	int StageBlock[ 90 ];
	int P_Walk[ 4 ];
};
PICTURE Pic;	//�摜�\���̐錾

/*****      �T�E���h�\����      *****/
typedef struct SOUND {
	int Sound;
};
SOUND Sound;		//���y�\���̐錾

/*****      �t���[�����[�g�\����      *****/
typedef struct FRAMERATE_CONTROL {
	int mStartTime;
	int mCount;
	int FrameCount;
	float mFps;
	int ReFreshRateState;
};
FRAMERATE_CONTROL FR_Control = { 0, 0, 0.0, 0 };	//�t���[�����[�g����\���̐錾

/*****      �L�[����\����      *****/
typedef struct OPERATE {
	//KeyInput
	int OldK;
	int NowK;
	int Kflg;
} OPERATE;

OPERATE opt;

/*****		  �}�b�v�\����		*****/
typedef struct MAP {
	int MapNum;			//�}�b�v�i���o�[
	int DrawFlg;		//DrawGraph����Ƃ��ɂ������
	int CollisionFlg;	//�Փ˃t���O
	int BreakFlg;		//��ꂽ���肷����
	int CoX;			//�u���b�N��X���W
	int CoY;			//�u���b�N��Y���W
	int ItemFlg;		//�A�C�e���������Ă��邩���Ȃ���
	int ItemType;		//�ǂ̃A�C�e���������Ă邩
	int ItemX;			//�A�C�e�����WX
	int ItemY;			//�A�C�e�����WY
	float IYSpeed;		//�����X�s�[�h
	int ItemFrame;		//�A�C�e���`��̃t���[��
	int ItemTrans;		//�A�C�e���̕`��J��
	int ItemDirect;		//�L�m�R�̓���(����)����
	int Block_UP;		//�u���b�N�̂͂˂鏈��
	int BU_F;			//�u���b�N�̂͂˂鏈���̃t���[��
};
MAP map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ];

typedef struct ITEM {
	int HataX;
	int HataY;
};
ITEM Item;

typedef struct POWERUP {
	int PwrX;
	int PwrY;
	int ItemDirect;		//�L�m�R�̓���(����)����
	int DrawMode;		//�`�悷�邩���Ȃ���
	float IYSpeed;		//�A�C�e���̗��������x
};
POWERUP Pup;

/*****		�v���C���[�\����		*****/
typedef struct PLAYER {
	int PlayerX;		//�v���C���[���WX
	int PlayerY;		//�v���C���[���WY
	float PSpeed;		//�v���C���[�X�s�[�h
	int JumpFrame;		//�W�����v�t���[��
	int JumpTime;		//�X�y�[�X�L�[�̉����ꂽ����
	int JumpMode;		//�W�����v��ԕϐ�
	int PJSpeed;		//�W�����v�X�s�[�h
	int P_i_f;			//���s�t���[��
	int P_lr_f;			//���s�����ϐ�
	int JF_f;
	int MapScrollX;		//�}�b�v�X�N���[������̂ɕK�v�Ȃ��
	int MapSSpeed;		//�}�b�v�X�N���[���̌��ݗ�
	float PYSpeed;		//�����X�s�[�h
	int PDirectMode;	//�����ŏ����A��Ƃ��̕ϐ�
	int PlayerState;	//�v���C���[�̏��( 1=Mario, 2=SuperMario )
	int GoalFlg;		//�S�[���t���O
	int DeathFlg;		//���S�t���O
	int CoinNum;		//�R�C���̖���
};
//first 3,11
PLAYER Player = { ( ( 3 * _MASS_X ) + _MASS_HALF ), ( 11 * _MASS_Y + _MASS_HALF ), 0, 0 };

int Map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ] = 
	{	
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0,  0,  6,  6,  6,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  0,  0,  0,  0,  0,  6,  2,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0, 43, 43, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  6,  2,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  6,  0,  0,  0,  0,  2,  0,  0,  2,  0,  0,  2,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  0,  0,  0,  0,  0,  0,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0, 18,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0, 35,  0,  0,  0, 43, 44, 46, 44, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 77, 78,  0, 15, 16, 17,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0, 15, 16, 17,  0,  9,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0,  0, 18,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0, 15, 16, 17,  0,  0, 35,  0,  0,  0, 46, 46, 48, 46, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{ 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0, 22, 23, 23, 24,  0, 77, 78, 15, 16, 69, 16, 17,  0,  0,  0,  0, 77, 78, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 23, 23, 24,  0,  0,  0, 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9, 23, 23,  9,  9,  9,  9, 15, 16, 69, 16,  9,  9,  9,  9,  9,  0,  0,  9,  9,  9,  9, 24, 15, 16, 17, 77, 78,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  9,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0, 15, 16, 69, 16, 17,  0,  9,  0,  0,  0, 46, 46, 70, 46, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8}
	};

//�����蔻��̂���}�b�v�i���o�[
//21
int HitBlockNum[ _HITBLOCK ] = { 1, 2, 3, 4, 5, 6, 7 , 8, 9, 37, 38, 39, 40, 41, 42, 49, 50, 51, 52, 53, 54, 65, 66, 77, 78 };

/****************************************************/
/*****											*****/
/*****			  �֐��v���g�^�C�v�錾			*****/
/*****											*****/
/****************************************************/

//�t���[�����[�g����֐�
static bool FR_Update( );	//�t���[�����[�g����֐�
static void FR_Draw( );		//�t���[�����[�g�`��֐�
static void FR_Wait( );		//�ҋ@�����֐�

//�Q�[�����C�������n�֐�
void GameInit();	//��������
void GameMain();	//���C������
void MapInit();		//�}�b�v��������

//�Q�[���X�e�[�g�`��֐�
void DrawTitle();	//�^�C�g���`��
void DrawGoal();	//�S�[���`��
void DrawEnd();		//�G���h��ʕ`��

//�Q�[�����C����ʕ`��n�֐�
void DrawStage();	//�X�e�[�W�`��
void DrawPlayer();	//�v���C���[�`��
void DrawBlock();	//�u���b�N�`��
void DrawFlag();	//�S�[�����`��
void DrawKinoko();	//�p���[�A�b�v�L�m�R�`��
int DrawItem( int iX, int iY /*, int iType, int iFrame*/ );	//�A�C�e���`��

//�}���I�����n�֐�
// int ComX, int ComY 
int HitBlockUp();		//�u���b�N�㓖���蔻��
int HitBlockDown();		//�u���b�N�������蔻��
int HitBlockRight();	//�u���b�N�E�����蔻��
int HitBlockLeft();		//�u���b�N�������蔻��

int HitBlockUp( int oX, int oY, int pX, int pY, int jMode, int who );		//�u���b�N�㓖���蔻��
int HitBlockDown( int oX, int oY, int pX, int pY, int jMode, int who );		//�u���b�N�������蔻��
int HitBlockRight( int oX, int oY, int pX, int pY, int jMode, int who );		//�u���b�N�E�����蔻��
int HitBlockLeft( int oX, int oY, int pX, int pY,int jMode, int who );		//�u���b�N�������蔻��

int HitBlockUpBreak();	//�u���b�N�㓖���蔻��A�j�󔻒�

//�Ǎ������֐�
int LoadImages();	//�摜�Ǎ�

/****************************************************/
/*****											*****/
/*****				  ���C������				*****/
/*****											*****/
/****************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	const int _WINDOWSIZE_X = 768;
	const int _WINDOWSIZE_Y = 672;

	GAMESTATE = GAME_TITLE;
	SetMainWindowText( "Super Mairo Bros" );					//�E�B���h�E�e�L�X�g�ύX
	SetGraphMode( _WINDOWSIZE_X, _WINDOWSIZE_Y, 32 );			//�E�B���h�E�T�C�Y�ύX

	ChangeWindowMode( _WINDOWMODE );							//�E�B���h�E���[�h�ύX
	if ( DxLib_Init() == -1 )	return -1;		//Dx���C�u����������

	int offscreen_handle = MakeScreen( 512, 448, FALSE );
	SetDrawScreen( offscreen_handle );							//�`��X�N���[���ύX
	/********************     ���t���b�V�����[�g�m�F     *************************/
	hdc = GetDC( GetMainWindowHandle() ) ;			// �f�o�C�X�R���e�L�X�g�̎擾
	RefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;	// ���t���b�V�����[�g�̎擾
	ReleaseDC( GetMainWindowHandle(), hdc ) ;		// �f�o�C�X�R���e�L�X�g�̉��
	/*****************************************************************************/

	if ( LoadImages() == -1 )	return -1;		//�摜�Ǎ�����

	/*************************     ���C�����[�v����     **************************/
	while ( ProcessMessage() == 0 && ClearDrawScreen() == 0 && GAMESTATE != 99 ) {
	

		SetDrawScreen( offscreen_handle );

		opt.OldK = opt.NowK;
		opt.NowK = GetJoypadInputState( DX_INPUT_KEY_PAD1 );
		opt.Kflg = opt.NowK & ~opt.OldK;

		

		switch( GAMESTATE ) {

			case GAME_TITLE:
				DrawTitle();
				break;
			case GAME_INIT:
				GameInit();
				break;
			case GAME_MAIN:
				GameMain();
				break;
			case GAME_GOAL:
				DrawGoal();
				break;
			case GAME_END:
				DrawEnd();
				break;
		}
	
		FR_Update();
		SetDrawScreen( DX_SCREEN_BACK );
		DrawExtendGraph( 0, 0, _WINDOWSIZE_X, _WINDOWSIZE_Y, offscreen_handle, FALSE );
#ifdef _DEBUGMODE
			FR_Draw();
#endif
		ScreenFlip();
		FR_Wait();

	}
	
	DxLib_End();
	return 0;

}

/******�@�@�@�@�@�@�@�t���[��������֐�         ******/
static bool FR_Update( ) {
	if ( FR_Control.mCount == 0 ) {
		FR_Control.mStartTime = GetNowCount();
	}
	if ( FR_Control.mCount == RefreshRate ) {
		int t = GetNowCount();
		FR_Control.mFps = 1000.f / ( ( t - FR_Control.mStartTime ) / ( float )RefreshRate );
		FR_Control.mCount = 0;
		FR_Control.mStartTime = t;
	}
	FR_Control.mCount++;
	FR_Control.FrameCount++;
	return true;
}

/******�@�@�@�@�@�@�@�t���[�����[�g�`��         ******/
static void FR_Draw( ) {
	
	SetFontSize( _FONTSIZE_S );
	DrawFormatString( 516, 0, 0xff0000, "%d", FR_Control.FrameCount );
	DrawFormatString( 516, 20, 0xff0000, "%d", RefreshRate );
	DrawFormatString( 570, 0, 0xff0000, "%.1f", FR_Control.mFps );

}

/******�@�@�@�@�@�@�@ �����҂��֐�          ******/
static void FR_Wait( ) {
	int tookTime = GetNowCount() - FR_Control.mStartTime;
	int waitTime = FR_Control.mCount * 1000 / _FRAMERATE_60 - tookTime;

	if ( waitTime > 0 ) {
		Sleep( waitTime );
	}
}

//�^�C�g���`��
void DrawTitle() {
//	DrawGraph(100,100,Pic.Player[1],TRUE);
	int x;

	x = 256 - (GetDrawStringWidth( "MARIO", 5 )/2);
	DrawString( x, 240, "MARIO", 0xff0000 );

	x = 256 - (GetDrawStringWidth( "Push Space", 10 )/2);
	DrawString( x, 400, "Push Space", 0xffffff );
	
	

	if ( opt.Kflg & PAD_INPUT_M ) {
		GAMESTATE = GAME_INIT;
	} else if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_END;
	}
}

void DrawGoal() {
	int x;

	x = 256 - (GetDrawStringWidth( "GOAL", 4 )/2);
	DrawString( x, 240, "GOAL", 0xff0000 );

	x = 256 - (GetDrawStringWidth( "Push Space", 10 )/2);
	DrawString( x, 400, "Push Space", 0xffffff );
	
	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_END;
	}
}

//�G���h���
void DrawEnd() {
	GAMESTATE = END;
}

//���C����������
void GameInit() {

	MapInit();
	Player.PYSpeed = 0.0f;
	Player.PSpeed = 0.0f;
	Player.PJSpeed = 0.0f;
	Player.JumpFrame = 0;
	Player.JumpMode = FALSE;
	Player.DeathFlg = 0;
	Player.GoalFlg = 0;
	Player.MapScrollX = 0;
	Player.MapSSpeed = 0;
	Player.PDirectMode = 0;
	Player.PJSpeed = 0;
	Player.PlayerState = _MARIO_NOMAL;

	Item.HataX = 200 * _MASS_X;
	Item.HataY = 2 * _MASS_Y + 5;

	GAMESTATE = GAME_MAIN;

}

//�Q�[�����C������
void GameMain() {

	DrawStage();		//�X�e�[�W�`��
	DrawFlag();			//�S�[���t���O
	DrawPlayer();		//�v���C���[�`��
	//DrawKinoko();		//�L�m�R�`��

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
}

//�X�e�[�W�`��
void DrawStage() {

	static int A_frame=0;

	//�w�i�`��
	DrawBox( 0, 0, 512, 448, 0x5080f8, TRUE );
	
	//���C���`��
#ifdef _DEBUGMODE
	for ( int StageX = 0; StageX <= _MAP_X; StageX++ ) {
		DrawLine( StageX * _MASS_X, 0, StageX * _MASS_X, 448, 0xffffff );
	}
	for ( int StageY = 0; StageY < _MAP_Y ; StageY++ ) {
		DrawLine( 0, StageY * _MASS_Y, 512, StageY * _MASS_Y, 0xffffff );
	}
#endif

	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			map[ StageY ][ StageX ].CoX -= Player.MapSSpeed;
			if ( map[ StageY ][ StageX ].ItemFlg == _ITEM_BRING || map[ StageY ][ StageX ].ItemFlg == _ITEM_ANIME ) {
				map[ StageY ][ StageX ].ItemX -= Player.MapSSpeed;
			}
		}
	}

	//�}�b�v�`��
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {

			//�R�C��&�p���[�A�b�v�A�C�e���̃A�j���[�V����
			if ( map[ StageY ][ StageX ].ItemFlg == _ITEM_ANIME || map[ StageY ][ StageX ].ItemFlg == _ITEM_MOVIN ) {
				//��������(�n�e�i�u���b�N�ύX)
				if ( map[ StageY ][ StageX ].MapNum == 2 || map[ StageY ][ StageX ].MapNum == 3 || map[ StageY ][ StageX ].MapNum == 4 || map[ StageY ][ StageX ].MapNum == 5 ) {
					map[ StageY ][ StageX ].MapNum = 1;
					if ( map[ StageY ][ StageX ].ItemType == _ITEMT_COIN ) {
						Player.CoinNum++;
					}
					//else if ( map[ StageY ][ StageX ].ItemType == _ITEMT_PWRUP ) {
					//	Player.PlayerState = _MARIO_SUPER;
					//}
				}
				if ( DrawItem( StageX, StageY ) == TRUE ) {
					map[ StageY ][ StageX ].ItemFlg = _ITEM_NHAVE;
				}
			}

			//�n�e�i�u���b�N�̐F�̑J��
			if(map[StageY][StageX].MapNum==2||map[StageY][StageX].MapNum==3||map[StageY][StageX].MapNum==4||map[StageY][StageX].MapNum==5){
				if(A_frame++<250)map[StageY][StageX].MapNum=2;
				else if(A_frame<350)map[StageY][StageX].MapNum=4;
				else if(A_frame<450)map[StageY][StageX].MapNum=5;
				else if(A_frame<550)map[StageY][StageX].MapNum=4;
				else if(A_frame==550)A_frame=0;
			}

			//�u���b�N�̂͂˂鏈��
			if(map[ StageY ][ StageX ].BU_F!=0){
				switch(map[ StageY ][ StageX ].BU_F){
					case 1:
						map[ StageY ][ StageX ].Block_UP-=3;
						if(map[ StageY ][ StageX ].Block_UP<-9)map[ StageY ][ StageX ].BU_F=2;
						break;
					case 2:
						map[ StageY ][ StageX ].Block_UP+=3;
						if(map[ StageY ][ StageX ].Block_UP==0)map[ StageY ][ StageX ].BU_F=0;
						break;
				
				}
			}

			//�}�b�v�`��
			DrawRotaGraph( map[ StageY ][ StageX ].CoX + _MASS_HALF, map[ StageY ][ StageX ].CoY + _MASS_HALF + map[ StageY ][ StageX ].Block_UP,
								1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
		}
	}

	//�}�b�v�`��
	//for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
	//	for ( int StageX = ( Player.MapScrollX / _MASS_X ); StageX < ( Player.MapScrollX / _MASS_X ) * (_MAP_X + 4 ); StageX++ ) {
	//		DrawRotaGraph( map[ StageY ][ StageX ].CoX + _MASS_HALF, map[ StageY ][ StageX ].CoY + _MASS_HALF,
	//							1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
	//	}
	//}
	//Player.Scroll=-map[0][0].CoX;

}

//�S�[�����̕`��
void DrawFlag() {

	if ( Player.GoalFlg == 1 && HitBlockDown( Item.HataX, Item.HataY, 0, _MASS_HALF, FALSE, _HITW_FLAG ) != _HIT_TRUE ) {
		Item.HataY += 3;
	} else if ( Player.GoalFlg == 1 && HitBlockDown( Item.HataX, Item.HataY, 0, _MASS_HALF, FALSE, _HITW_FLAG ) == _HIT_TRUE ) {
		Player.GoalFlg = 2;
	}
	DrawRotaGraph( Item.HataX - Player.MapScrollX, Item.HataY, 1.0f, 0, Pic.StageBlock[ 36 ], TRUE );

}

int DrawItem( int iX, int iY /*, int iType, int iFrame*/ ) {

	//�R�C���̕`��
	if ( map[ iY ][ iX ].ItemType == _ITEMT_COIN ) {

		//if ( map[ iY ][ iX ].ItemTrans++ < 2 ) {
		//	map[ iY ][ iX ].ItemTrans = 27;
		//}
		//else if ( map[ iY ][ iX ].ItemTrans >= 3 && map[ iY ][ iX ].ItemTrans < 4 ) {
		//	map[ iY ][ iX ].ItemTrans = 28;
		//}
		//else if ( map[ iY ][ iX ].ItemTrans >= 5 && map[ iY ][ iX ].ItemTrans < 6 ) {
		//	map[ iY ][ iX ].ItemTrans = 29;
		//}
		//else if ( map[ iY ][ iX ].ItemTrans >= 7 && map[ iY ][ iX ].ItemTrans < 8 ) {
		//	map[ iY ][ iX ].ItemTrans = 30;
		//}
		//else if ( map[ iY ][ iX ].ItemFrame >= 9 ) {
		//	map[ iY ][ iX ].ItemTrans = 27;
		//}

		if ( map[ iY ][ iX ].ItemTrans++ > 6 ) {
			map[ iY ][ iX ].ItemTrans = 0;
		}

		if ( map[ iY ][ iX ].ItemFrame++ < 12 ) {
			map[ iY ][ iX ].ItemY -= 8;
		} else if ( map [ iY ][ iX ].ItemFrame >= 12 ) {
			map[ iY ][ iX ].ItemY += 8;
		}
		DrawRotaGraph( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, 1.0f, 0, Pic.StageBlock[ 27 + ( ( map[ iY ][ iX ].ItemTrans ) / 2 ) ], TRUE );
		if ( map[ iY ][ iX ].ItemFrame > 20 ) {
			return TRUE;
		}
	}

	//�p���[�A�b�v�L�m�R�̕`��
	if ( map[ iY ][ iX ].ItemType == _ITEMT_PWRUP ) {

		if ( map[ iY ][ iX ].ItemFrame++ < 1 ) {
			map[ iY ][ iX ].ItemY -= 10;
		}
		else if ( map[ iY ][ iX ].ItemFrame < 25 ) {
			map[ iY ][ iX ].ItemY -= 1;
		}
		else if ( map[ iY ][ iX ].ItemFrame >= 26 && map[ iY ][ iX ].ItemFrame < 27 ) {
			map[ iY ][ iX ].ItemFlg = _ITEM_MOVIN;
		}
		else if ( map[ iY ][ iX ].ItemFrame >= 28 ) {
			//Pup.PwrX = map[ iY ][ iX ].ItemX;
			//Pup.PwrY = map[ iY ][ iX ].ItemY;
			//Pup.ItemDirect = map[ iY ][ iX ].ItemDirect;
			//Pup.IYSpeed = 0.0f;
			//Pup.DrawMode = 1;

			//���E�ɓ���
			if ( map[ iY ][ iX ].ItemDirect == _DIRECT_RIGHT ) {
				if ( HitBlockRight( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
					map[ iY ][ iX ].ItemX += 2;
				} else if ( HitBlockRight( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
					map[ iY ][ iX ].ItemDirect = _DIRECT_LEFT;
				}
			}
			else if ( map[ iY ][ iX ].ItemDirect == _DIRECT_LEFT ) {
				if ( HitBlockLeft( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
					map[ iY ][ iX ].ItemX -= 2;
				} else if ( HitBlockLeft( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
					map[ iY ][ iX ].ItemDirect = _DIRECT_RIGHT;
				}
			}

			/*****     ��������     *****/
			if ( HitBlockDown( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF , _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
				if ( map[ iY ][ iX ].IYSpeed < 8.0f )	map[ iY ][ iX ].IYSpeed += 0.8f;
				map[ iY ][ iX ].ItemY += ( int )map[ iY ][ iX ].IYSpeed;
				//�߂荞�ݖh�~����
				//if ( HitBlockDown( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
				//	int Difference = 0;
				//	Difference = ( map[ iY ][ iX ].ItemY + _MASS_HALF ) - map[ ( map[ iY ][ iX ].ItemY / 32 ) + 1 ][ ( Player.MapScrollX + map[ iY ][ iX ].ItemX )/32 ].CoY;
				//	map[ iY ][ iX ].ItemY -= Difference;
				//	map[ iY ][ iX ].IYSpeed = 0.0f;
				//}
			} else {
				map[ iY ][ iX ].IYSpeed = 0.0f;
			}

		}

		if ( map[ iY ][ iX ].ItemY >= ( 15 * _MASS_Y ) || map[ iY ][ iX ].ItemX >= ( 15 * _MASS_X ) || map[ iY ][ iX ].ItemX < ( 0 - _MASS_X ) ) {
			return TRUE;
		}
		
		//if ( map[ iY ][ iX ].ItemFrame > 30 ) {
		//	return TRUE;
		//}
		DrawRotaGraph( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, 1.0f, 0, Pic.StageBlock[ 13 ], TRUE );
	}

	return FALSE;

}

//�L�m�R�`��
void DrawKinoko() {
	if ( Pup.DrawMode == 1 ) {
		//���E�ɓ���
		if ( Pup.ItemDirect == _DIRECT_RIGHT ) {
			if ( HitBlockRight( Pup.PwrX, Pup.PwrY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
				Pup.PwrX += 2;
			} else if ( HitBlockRight( Pup.PwrX, Pup.PwrY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
				Pup.ItemDirect = _DIRECT_LEFT;
			}
		}
		else if ( Pup.ItemDirect == _DIRECT_LEFT ) {
			if ( HitBlockLeft( Pup.PwrX, Pup.PwrY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
				Pup.PwrX -= 2;
			} else if ( HitBlockLeft( Pup.PwrX, Pup.PwrY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
				Pup.ItemDirect = _DIRECT_RIGHT;
			}
		}
		///*****     ��������     *****/
		if ( HitBlockDown( Pup.PwrX, Pup.PwrY, _MASS_HALF , _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
			if ( Pup.IYSpeed < 8.0f )	Pup.IYSpeed += 0.8f;
			Pup.PwrY += ( int )Pup.IYSpeed;
			//�߂荞�ݖh�~����
			//if ( HitBlockDown( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
			//	int Difference = 0;
			//	Difference = ( map[ iY ][ iX ].ItemY + _MASS_HALF ) - map[ ( map[ iY ][ iX ].ItemY / 32 ) + 1 ][ ( Player.MapScrollX + map[ iY ][ iX ].ItemX )/32 ].CoY;
			//	map[ iY ][ iX ].ItemY -= Difference;
			//	map[ iY ][ iX ].IYSpeed = 0.0f;
			//}
		} else {
			Pup.IYSpeed = 0.0f;
		}
	}

}

//�v���C���[�`��
void DrawPlayer() {

	static int PDrawMode = 0;	//0=�����ĂȂ��A1=�E�����Ă���A 2=�������Ă�
	static int SlideMode = 0;	//1=�E����A2=������
	static int JumpState = 1;	//�W�����v�̏�Ԃ������Ă���
	int InMode = 0;
	int InMode2 = 0;
	int WalkMove = 9 - ( int )Player.PSpeed;

	if ( ( ( opt.NowK & PAD_INPUT_M ) == 0 ) && JumpState == 1 ) {
		JumpState = 0;
	}

	//�����A�j���[�V����
	if( ( 0 == FR_Control.FrameCount % WalkMove ) && /*opt.NowK != NULL*/ Player.PSpeed >= 0.0f ){
		if ( Player.JumpFrame == 0 )	Player.P_i_f++;
		if(Player.P_i_f >= 3 && Player.JumpFrame == 0 )	Player.P_i_f=0;
		if ( Player.JumpFrame > 0 )	Player.P_i_f = 3;
	}
	else if( Player.PSpeed <= 0.0f ){
		Player.P_i_f = 0;	//�L�[�������߂���
	}

	//���s����
	/*****     �E�ړ�����     *****/
	if ( Player.GoalFlg == 0 && Player.PlayerX <= ( 15 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {

		/** �ʏ폈�� **/
		if ( PDrawMode == 0 || ( PDrawMode == 2 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 1;
		}
		if ( PDrawMode == 1 ) {
			if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				if ( Player.PSpeed < 6.0f ) {
					Player.PSpeed += 0.2f;				//�����x�ݒ�
				}

				if ( Player.PlayerX < ( 8 * _MASS_X + _MASS_HALF ) && ( Player.MapScrollX + Player.PlayerX <= ( 200 * _MASS_X + _MASS_HALF ) ) ) {
					Player.PlayerX += Player.PSpeed;
				}
				else if ( Player.PlayerX >= ( 8 * _MASS_X + _MASS_HALF ) ) {
					Player.MapSSpeed = Player.PSpeed;
					Player.MapScrollX += Player.MapSSpeed;
				}
					
			} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
			} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
				Player.GoalFlg = 1;
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
				Player.PJSpeed = 0;
				JumpState = 0;
				Player.JumpFrame = 0;
			}
			Player.P_lr_f = _DIRECT_RIGHT;					//���E���]�t���O
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		//���s���̃v���C���[�`��
		}
		/** �}�u���[�L�̏��� **/
		if ( PDrawMode == 2 && Player.PSpeed > 0.0f ) {
			Player.P_lr_f=0;					//���E���]�t���O
			SlideMode = 1;
			if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				Player.PSpeed -= 0.4f;
				if ( ( Player.PlayerX -= Player.PSpeed ) < ( 0 * _MASS_X + _MASS_HALF ) ) {
					Player.PlayerX += Player.PSpeed;
					Player.PSpeed = 0.0f;
					Player.MapSSpeed = 0;
				}
			} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
			} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
				GAMESTATE = GAME_GOAL;
			}
			if ( Player.PSpeed <= 0.0f ) {
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
				PDrawMode = 1;
			}
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, FALSE );
		}
	}
	/*****     ���ړ�����     *****/
	else if ( Player.GoalFlg == 0 && Player.PlayerX > ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X ) ) {

		/** �ʏ폈�� **/
		if ( PDrawMode == 0 || ( PDrawMode == 1 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 2;
		}
		if ( PDrawMode == 2 ) {
			InMode = 2;
			if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				if ( Player.PSpeed < 6.0f ) {
					Player.PSpeed += 0.2f;			//�����x�ݒ�
				}
				Player.PlayerX -= Player.PSpeed;
			} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
			} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
				GAMESTATE = GAME_GOAL;
			}
			Player.P_lr_f = _DIRECT_LEFT;					//���E���]�t���O
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//���s���̃v���C���[�`��
		}
		/** �}�u���[�L�̏��� **/
		if ( PDrawMode == 1 && Player.PSpeed > 0.0f ) {
			SlideMode = 2;
			InMode2 = 2;
			Player.P_lr_f=1;					//���E���]�t���O
			if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				Player.PSpeed -= 0.4f;
				if ( Player.PlayerX < ( 8 * _MASS_X + _MASS_HALF ) && ( Player.MapScrollX + Player.PlayerX < ( 200 * _MASS_X + _MASS_HALF ) ) ) {
					Player.PlayerX += Player.PSpeed;
				}
				else if ( Player.PlayerX >= ( 8 * _MASS_X + _MASS_HALF ) ) {
					Player.MapSSpeed = Player.PSpeed;
					Player.MapScrollX += Player.MapSSpeed;
				}
			} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
			} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
				Player.GoalFlg = 1;
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
				Player.PJSpeed = 0;
				JumpState = 0;
				Player.JumpFrame = 0;
			}
			if ( Player.PSpeed <= 0.0f )	{
				Player.PSpeed = 0.0f;
				Player.MapSSpeed = 0;
				PDrawMode = 2;
			}
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, TRUE );
		}
	}
	/*****  ���̑�����   *****/
	else {
		if ( Player.PSpeed > 0.0f ) {
			Player.PSpeed -= 0.2f;
			/*****  �}���I���菈��   *****/
			if ( Player.PSpeed > 0.0f ) {
				//�E����
				if ( PDrawMode == 1 && ( Player.PlayerX < 15 * _MASS_X + _MASS_HALF ) ) { 
					if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						if ( Player.PlayerX < ( 8 * _MASS_X + _MASS_HALF ) && ( Player.MapScrollX + Player.PlayerX <= ( 200 * _MASS_X + _MASS_HALF ) ) ) {
							Player.PlayerX += Player.PSpeed;
						}
						else if ( Player.PlayerX >= ( 8 * _MASS_X + _MASS_HALF ) ) {
							Player.MapSSpeed = Player.PSpeed;
							Player.MapScrollX += Player.MapSSpeed;
						}
					} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
						Player.GoalFlg = 1;
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
						Player.PJSpeed = 0;
						JumpState = 0;
						Player.JumpFrame = 0;
					}
					InMode2 = 3;
				} 
				//������
				else if ( PDrawMode == 2 && ( Player.PlayerX > 0 * _MASS_X + _MASS_HALF ) ) {
					if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						Player.PlayerX -= Player.PSpeed;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
						GAMESTATE = GAME_GOAL;
					}
				}
			}
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ Player.P_i_f ], TRUE , Player.P_lr_f );
		}
		if ( Player.PSpeed <= 0.0f ) {
			PDrawMode = 0;
			Player.PSpeed = 0.0f;
			Player.MapSSpeed = 0;
		}
		InMode = 1;
	}

	/*****     �W�����v����     *****/
	if ( Player.GoalFlg == 0 && JumpState == 0 && Player.JumpMode == FALSE && ( opt.NowK & PAD_INPUT_M ) ) {
		Player.JumpMode = TRUE;
		Player.JumpFrame = 0;
		Player.PJSpeed = 7.0f;
		JumpState = 2;
	}
	
	if ( Player.JumpMode == TRUE ) {

		//������̃u���b�N�̓����蔻��
		if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {			
			//�^�񒆂Ƀq�b�g����ꍇ
			//map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].BU_F=1;
			Player.PJSpeed = 0.0f;
			Player.JumpMode = FALSE;
			Player.JumpFrame = 0;
		//} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_ITEM ) {
		//	//map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum = 1;
		//	Player.PJSpeed = 0.0f;
		//	Player.JumpMode = FALSE;
		//	Player.JumpFrame = 0;
		//} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_BREAK ) {	
		//	//�^�񒆂Ƀq�b�g�����ꍇ(�j��)
		//	Player.PJSpeed = 0.0f;
		//	Player.JumpMode = FALSE;
		//	Player.JumpFrame = 0;
		} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_LEFT ) {	
			//(�}���I��)�����ɓ���������
			int Difference = 0;
			Difference = map[ ( Player.PlayerY-_MASS_HALF+4 )/32][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX -
							( ( Player.PlayerX ) - _MASS_HALF + 4 );
			Player.PlayerX += Difference;
		} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_RIGHT ) {	
			//(�}���I��)�����ɓ���������
			int Difference = 0;
			Difference = ( ( Player.PlayerX )+_MASS_HALF-4 ) - 
						map[ ( Player.PlayerY-_MASS_HALF+4 )/32 ][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX;
			Player.PlayerX -= Difference;
		} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {	
			//�q�b�g���Ȃ��ꍇ
			if ( Player.JumpFrame++ < 6 ) {
				if ( ( opt.NowK & PAD_INPUT_M ) == 0 ) {
					JumpState = 3;
				}
			} else if ( Player.JumpFrame >= 6 && Player.JumpFrame < 18 ) {
				if ( opt.NowK & PAD_INPUT_M ) {
					if ( JumpState != 3 )	Player.PJSpeed =  8.0f;
				} else if ( ( opt.NowK & PAD_INPUT_M ) == 0 ) { 
					if ( Player.PJSpeed > 0.0f ) {
						Player.PJSpeed -= 0.4f;
					} else if ( Player.PJSpeed <= 0.0f ) {
						Player.PJSpeed = 0.0f;
						Player.JumpMode = FALSE;
						Player.JumpFrame = 0;
					}
					JumpState = 3;
				}
			} else if ( Player.JumpFrame >= 18 ) {
				if ( Player.PJSpeed > 0.0f ) {
					Player.PJSpeed -= 0.4f;
				} else if ( Player.PJSpeed <= 0.0f ) {
					Player.PJSpeed = 0.0f;
					Player.JumpMode = FALSE;
					Player.JumpFrame = 0;
				}
			} 
			Player.PlayerY -= Player.PJSpeed;
		}

	} else if ( JumpState == 2 && Player.JumpMode == FALSE ) {
		JumpState = 3;
	}

	/*****     ��������     *****/
	if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
		JumpState = 4;
		if ( Player.PYSpeed < 8.0f )	Player.PYSpeed += 0.8f;
		Player.PlayerY += ( int )Player.PYSpeed;
		//�߂荞�ݖh�~����
		if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
			int Difference = 0;
			Difference = ( Player.PlayerY + _MASS_HALF ) - map[ ( Player.PlayerY / 32 ) + 1 ][ ( Player.MapScrollX + Player.PlayerX )/32 ].CoY;
			Player.PlayerY -= Difference;
			Player.PYSpeed = 0.0f;
		}
	} 
	else if ( Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
		if ( JumpState == 3 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
			JumpState = 0;
		}
		if ( JumpState == 4 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
			JumpState = 0;
		}
	}
	else {
		Player.PYSpeed = 0.0f;
	}

	//�S�[���A�j���[�V����
	static int pg_walk;
	if ( Player.GoalFlg == 1 ) {
		if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) != _HIT_TRUE ) {
			Player.PlayerY += 3;
		}
	} else if ( Player.GoalFlg == 2 ) {
		static int i = 0;
		if ( i == 0 ) {
			Player.PlayerX += _MASS_X;
		}
		if ( ++i > 50 ) {
			Player.GoalFlg = 3;
		}
	} else if ( Player.GoalFlg == 3 ) {
		//GAMESTATE = GAME_GOAL;
		
		Player.PlayerX+=3;
		if(FR_Control.FrameCount%3){
			switch(pg_walk){
				case 0:
					pg_walk=1;
					break;
				case 1:
					pg_walk=2;
					break;
				case 2:
					pg_walk=3;
					break;
				case 3:
					pg_walk=1;
					break;
			}
		}
		if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_CASTLE ){
			Player.GoalFlg=4;
			GAMESTATE=GAME_GOAL;
		}
	}

	static int i=7; 
	if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
		if(i==8)i=7;
		else if(i==7)i=8;
	}

	//�v���C���[�`��
	if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );	//�W�����v���ĂȂ��Ƃ�
	if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );	//�W�����v������
	if ( Player.GoalFlg == 1 ) {
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[i], TRUE, FALSE );			//�S�[��������
	} else if ( Player.GoalFlg == 2 ) {
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[7], TRUE, TRUE );			//�S�[������(��ԉ��ɍ~�肽)�Ƃ�(���])
	}
#ifdef _DEBUGMODE
	SetFontSize( 12 );
	DrawFormatString( 420,  50, 0xff0000, "OldK = %d", opt.OldK );				//OldK�`��
	DrawFormatString( 420,  70, 0xff0000, "NowK = %d", opt.NowK );				//NowK�`��
	DrawFormatString( 420,  90, 0xff0000, "PdrM = %d", PDrawMode );				//PDrawMode�`��
	DrawFormatString( 420, 110, 0xff0000, "LR_F = %d", Player.P_lr_f );			//P_lr_f�`��
	DrawFormatString( 420, 130, 0xff0000, "PlrX = %d", Player.PlayerX );		//PlayerX�`��
	DrawFormatString( 420, 150, 0xff0000, "PlrY = %d", Player.PlayerY );		//PlayerY�`��
	DrawFormatString( 420, 170, 0xff0000, "Scrl = %d", Player.MapScrollX );		//Scroll�`��
	DrawFormatString( 420, 190, 0xff0000, "Pspd = %.2f", Player.PSpeed );		//PSpeed�`��
	DrawFormatString( 420, 210, 0xff0000, "ItmX = %d", map[ 8 ][ 23 ].ItemX );				//InMode�`��
	DrawFormatString( 420, 230, 0xff0000, "Coin = %d", Player.CoinNum );		//�R�C�������`��
	DrawFormatString( 420, 250, 0xff0000, "PlrS = %d", Player.PlayerState );	//InMode2�`��
	DrawFormatString( 420, 270, 0xff0000, "JmpM = %d", ( ( Player.PlayerX ) - _MASS_HALF + 4 ) );			//JumpMode�`��
	DrawFormatString( 420, 290, 0xff0000, "CoX  = %d", map[ ( Player.PlayerY-_MASS_HALF+4 )/32][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX );	//InMode2�`��

	DrawCircle( Player.PlayerX, Player.PlayerY, 2, 0xFF69B4, 1 );
	DrawCircle( map[ ( ( Player.PlayerY )/32 ) ][ ( ( Player.PlayerX )/32 ) ].CoX, map[ ( ( Player.PlayerY )/32 ) ][ ( ( Player.PlayerX )/32 ) ].CoY, 2, 0x0000ff, 1 );
	DrawCircle( Player.PlayerX - _MASS_HALF + 8, Player.PlayerY + _MASS_HALF, 2, 0xffff00, 1 );
	DrawCircle( map[ ( ( Player.PlayerY )/32 ) + 1 ][ ( ( Player.PlayerX - _MASS_HALF + 8 )/32 ) ].CoX, map[ ( ( Player.PlayerY )/32 ) + 1 ][ ( ( Player.PlayerX - _MASS_HALF + 8 )/32 ) ].CoY, 2, 0x000000, 1 );
#endif
}

//�摜�Ǎ�
int LoadImages() {

	//�u���b�N�Ǎ�
	if ( LoadDivGraph( "images/backimage.png", 84, 12, 7, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//�L�����N�^�[�Ǎ�
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )			return -1;
	//�X�[�p�[�}���I�ǂݍ���
	if ( LoadDivGraph( "images/super_mario_chara.png", 15, 5, 3, 32, 64, Pic.Suplayer ) == -1 )	return -1;

	Pic.P_Walk[0]=Pic.Player[1];
	Pic.P_Walk[1]=Pic.Player[2];
	Pic.P_Walk[2]=Pic.Player[3];
	Pic.P_Walk[3]=Pic.Player[4];

	return TRUE;
}

//�}�b�v��������
void MapInit() {
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			map[ StageY ][ StageX ].MapNum = Map[ StageY ][ StageX ];
			map[ StageY ][ StageX ].CoX = StageX * _MASS_X;
			map[ StageY ][ StageX ].CoY = StageY * _MASS_Y;
			map[ StageY ][ StageX ].DrawFlg = TRUE;
			map[ StageY ][ StageX ].BreakFlg = FALSE;
			map[ StageY ][ StageX ].ItemFlg = _ITEM_NHAVE;
			map[ StageY ][ StageX ].ItemType = 0;
			map[ StageY ][ StageX ].ItemTrans = 0;
			map[ StageY ][ StageX ].ItemDirect = 0;
			//�n�e�i�u���b�N�������ꍇ
			if ( Map[ StageY ][ StageX ] == 2 ) {
				map[ StageY ][ StageX ].ItemFlg = _ITEM_BRING;
				map[ StageY ][ StageX ].ItemType = _ITEMT_COIN;
				map[ StageY ][ StageX ].ItemX = StageX * _MASS_X + _MASS_HALF;
				map[ StageY ][ StageX ].ItemY = StageY * _MASS_Y + _MASS_HALF;
			}
		}
	}

	map[ 8 ][ 4 ].ItemType = _ITEMT_PWRUP;
	map[ 8 ][ 23 ].ItemType = _ITEMT_PWRUP;
}

// int ComX, int ComY 
//�㑤����
int HitBlockUp() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		//if ( map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) - 1 ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ] )	return _HIT_TRUE;
		if ( map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF+4 )/32 )  ].MapNum == HitBlockNum[ i ] 
			|| map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF-4 )/32 )  ].MapNum == HitBlockNum[ i ]  ) {
				return _HIT_TRUE;
		}

		if ( map[ ( ( Player.PlayerY-_MASS_HALF+4 )/32 ) ][  ( ( Player.PlayerX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
			return _HIT_BREAK;
		}
	}
	
	return _HIT_FALSE;
}

//��������
int HitBlockDown() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( map[ ( ( Player.PlayerY+_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF+4 )/32 )  ].MapNum == HitBlockNum[ i ] 
		|| map[ ( ( Player.PlayerY+_MASS_HALF )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF-4 )/32 )  ].MapNum == HitBlockNum[ i ]  ) {
				return _HIT_TRUE;
		}
	
	}
	return _HIT_FALSE;
}

//�E������
int HitBlockRight() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		//if ( map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) + 1 ].MapNum == HitBlockNum[ i ] )	return _HIT_TRUE;
		if ( map[ ( ( Player.PlayerY-_MASS_HALF+4 )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( Player.PlayerY+_MASS_HALF-4 )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ]) {
			return _HIT_TRUE;
		}
	}
	
	return _HIT_FALSE;
}

//��������
int HitBlockLeft() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		//if ( map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ] )	return _HIT_TRUE;
		if ( map[ ( ( Player.PlayerY-_MASS_HALF+4 )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( Player.PlayerY+_MASS_HALF-4 )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ]) {
			return _HIT_TRUE;
		}
	}
	
	return _HIT_FALSE;
}

//�u���b�N�㓖���蔻��
int HitBlockUp( int oX, int oY, int pX, int pY, int jMode, int who ){
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
			if ( map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].ItemFlg == _ITEM_BRING ) {
				map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].ItemFlg = _ITEM_ANIME;
			}
			if ( who == _HITW_PLAYER ) {
				if ( map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 2 ||
					 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 3 ||
					 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 4 ||
					 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 5 ||
					 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 6 ||
					 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 7 ) {
						map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].BU_F=1;
				}
			}
			return _HIT_TRUE;
		} 
		else if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX+6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
			return _HIT_LEFT;
		}
		else if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX+pX-6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
			return _HIT_RIGHT;
		}
	}
	
	return _HIT_FALSE;
}

//�u���b�N�������蔻��
int HitBlockDown( int oX, int oY, int pX, int pY, int jMode, int who ) {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( map[ ( ( oY+pY )/32 ) ][ ( ( oX-pX+6 )/32 ) ].MapNum == HitBlockNum[ i ]
		|| map[ ( ( oY+pY )/32 ) ][ ( ( oX+pX-6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
			return _HIT_TRUE;
		}
	
	}
	return _HIT_FALSE;
}

//�u���b�N�E�����蔻��
int HitBlockRight( int oX, int oY, int pX, int pY, int jMode, int who ) {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( who == _HITW_PLAYER ) {
			if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] 
			|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			} 
			else if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 34
			|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 35 ) {
				return _HIT_GOAL;
			}
			else if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 70) {
				return _HIT_CASTLE;
			}
		} else if ( who == _HITW_PWRUP ) {
			if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] 
			|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			} 
		}
	}
	
	return _HIT_FALSE;
}

//�u���b�N�������蔻��
int HitBlockLeft( int oX, int oY, int pX, int pY, int jMode, int who ) {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
			return _HIT_TRUE;
		}
	}
	
	return _HIT_FALSE;
}