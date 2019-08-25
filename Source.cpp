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
#define _MASS_ENEMY	24

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
#define _HIT_DEATH	9

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

#define _ENEMY_NUM		6

//�Q�[����ԕϐ�
static int GAMESTATE;

//���t���b�V�����[�g�m�F�p�ϐ�
static int RefreshRate;
HDC hdc;

//�E�B���h�E�T�C�Y
const int _WINDOWSIZE_X = 768;
const int _WINDOWSIZE_Y = 672;

/*****      �Q�[�����[�h�񋓑�      *****/
typedef enum GAME_MODE {
	GAME_TITLE,			//�^�C�g��
	GAME_INIT,			//������
	GAME_MAIN,			//���C��
	GAME_INITB,			//�c�@�Ƃ��\���̉��
	GAME_GOAL,			//�S�[�����(�J�����)
	GAME_OVER,			//�Q�[���I�[�o�[�`��
	GAME_END,			//�G���h����
	END = 99			//�G���h
};

/*****      �Q�[�����[�h�񋓑�      *****/
//typedef enum BLOCK_NUMBER {
	

/*****      �摜�\����      *****/
typedef struct PICTURE {
	int Player[ 15 ];		//�m�[�}���}���I
	int Suplayer[ 15 ];		//�X�[�p�[�}���I
	int FirePlayer[ 15 ];	//�t�@�C���[�}���I
	int StageBlock[ 90 ];	//�X�e�[�W�`��u���b�N
	int P_Walk[ 4 ];		//���s����(�m�[�}��)
	int P_WalkS[ 4 ];		//���s����(�X�[�p�[)
	int P_WalkF[ 4 ];		//���s����(�t�@�C���[)
	int EnemyMob[ 20 ];		//�G�摜
	int TitleImg;
};
PICTURE Pic;	//�摜�\���̐錾

/*****      �T�E���h�\����      *****/
typedef struct SOUND {
	int coin;
	int kinoko;
	int pole;
	int goal;
	int powerUp;
	int bgm_ground;
	int bgm_death;
	int BounceBlock;
	int BreakBlock;
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
	int PlayerState;	//�v���C���[�̏��( 1=Mario, 2=SuperMario, 3=FireMario )
	int PlayerAnime;	//�A�j���[�V�������(�S�̂���x�~�܂鏈��)
	int PAnimeFrame;	//�v���C���[�A�j���[�V�����t���[��
	int dgnFlg;			//�v���C���[���މ��������Ɉꎞ�I�ɖ��G�ɂȂ鏈��
	int GoalFlg;		//�S�[���t���O
	int DeathFlg;		//���S�t���O
	int Life;			//�c�@
	int CoinNum;		//�R�C���̖���
	int Point;			//�|�C���g
	int DownFlg;		//������t���O
};
//first 3,11
PLAYER Player /*= { ( ( 3 * _MASS_X ) + _MASS_HALF ), ( 11 * _MASS_Y + _MASS_HALF ), 0, 0 }*/;

typedef struct ENEMY {
	int EnemyType;	//�G�̎��( 1=�N���{�[, 2=�̂��̂� )
	int MoveFlg;	//�G��������͈͂ɗ������ɗ���
	int EnemyX;		//�GX���W
	int EnemyY;		//�GY���W
	int EDirect;	//�G����
	int PressFlg;	//���݂���ꂽ�t���O
	float ESpeed;	//�GX�X�s�[�h
	float EYSpeed;	//�GY�X�s�[�h
};
ENEMY Enemy[ _ENEMY_NUM ];

int Map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ] = 
	{	
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0, 72, 72,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0,  0,  6,  6,  6,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  0,  0,  0,  0,  0,  6,  2,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0, 43, 43, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  6,  2,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  6,  0,  0,  0,  0,  2,  0,  0,  2,  0,  0,  2,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  0,  0,  0,  0,  0,  0,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0, 45, 46, 47,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0, 18,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0, 35,  0,  0,  0, 43, 44, 46, 44, 43,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 77, 78,  0, 15, 16, 17,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0, 15, 16, 17,  0,  9,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0,  0, 18,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0, 15, 16, 17,  0,  0, 35,  0,  0,  0, 46, 46, 48, 46, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{ 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0, 81, 72, 22, 23, 24,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0, 22, 23, 23, 24,  0, 77, 78, 15, 16, 69, 16, 17,  0, 72, 72,  0, 77, 78, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 23, 23, 24,  0,  0,  0, 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9, 23, 23,  9,  9,  9,  9, 15, 16, 69, 16,  9,  9,  9,  9,  9,  0,  0,  9,  9,  9,  9, 24, 15, 16, 17, 77, 78,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  9,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0, 15, 16, 69, 16, 17,  0,  9,  0,  0,  0, 46, 46, 70, 46, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
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
void GameInitB();	//�c�@�\��
void GameMain();	//���C������
void MapInit();		//�}�b�v��������
void PlayerInit();	//�v���C���[��������
void EnemyInit();	//�G��������

//�Q�[���X�e�[�g�`��֐�
void DrawTitle();	//�^�C�g���`��
void DrawGoal();	//�S�[���`��
void DrawEnd();		//�G���h��ʕ`��

//�Q�[�����C����ʕ`��n�֐�
void DrawStage();	//�X�e�[�W�`��
void DrawPlayer();	//�v���C���[�`��
void DrawBlock();	//�u���b�N�`��
void DrawFlag();	//�S�[�����`��
void DrawEnemy();	//�G�`��
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

int HitObjUp( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad );		//�G,�A�C�e���̓����蔻���
int HitObjDown( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad );		//�G,�A�C�e���̓����蔻�艺
int HitObjRight( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad );		//�G,�A�C�e���̓����蔻��E
int HitObjLeft( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad );		//�G,�A�C�e���̓����蔻�荶

int HitObj( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad );			//�����蔻��S����

int HitBlockUpBreak();	//�u���b�N�㓖���蔻��A�j�󔻒�

//�Ǎ������֐�
int LoadImages();	//�摜�Ǎ�
int LoadSounds();	//�T�E���h�ǂݍ���

/****************************************************/
/*****											*****/
/*****				  ���C������				*****/
/*****											*****/
/****************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

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
	if ( LoadSounds() == -1 )	return -1;		//���Ǎ�����

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
			case GAME_INITB:
				GameInitB();
				break;
			case GAME_MAIN:
				GameMain();
				break;
			case GAME_GOAL:
				DrawGoal();
				break;
			case GAME_OVER:
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
	static int x = 0;
	if ( x == 0 ) {
		MapInit();
		PlayerInit();
	}
	//x = 256 - (GetDrawStringWidth( "MARIO", 5 )/2);
	//DrawString( x, 240, "MARIO", 0xff0000 );
	//x = 256 - (GetDrawStringWidth( "Push Space", 10 )/2);
	//DrawString( x, 400, "Push Space", 0xffffff );
	
	//���[���h
	//DrawRotaGraph( 18*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 32 ] , TRUE);
	//DrawRotaGraph( 19*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 24 ] , TRUE);
	//DrawRotaGraph( 20*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 27 ] , TRUE);
	//DrawRotaGraph( 21*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 21 ] , TRUE);
	//DrawRotaGraph( 22*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 13 ] , TRUE);

	////�X�e�[�W
	//DrawRotaGraph( 19*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 1 ] , TRUE);
	//DrawRotaGraph( 20*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 36 ] , TRUE);
	//DrawRotaGraph( 21*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 1 ] , TRUE);
	//�^�C�g�����
	DrawStage();
	DrawPlayer();
	DrawRotaGraph( 256, 134, 2.1f, 0, Pic.TitleImg , TRUE );
	
	if ( opt.Kflg & PAD_INPUT_M ) {
		GAMESTATE = GAME_INIT;
		x = 0;
	} else if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_END;
		x = 0;
	}
	DrawFormatString( 0, 0, 0xffffff, "1" );
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
	//static int inFrame = 0;

	//if ( inFrame++ < 1 ) {
	//	MapInit();
	//	PlayerInit();

	//	Item.HataX = 200 * _MASS_X;
	//	Item.HataY = 2 * _MASS_Y + 5;
	//}
	//else if ( inFrame > 60 ) {
	//	GAMESTATE = GAME_MAIN;
	//	inFrame = 0;
	//}

	Item.HataX = 200 * _MASS_X;
	Item.HataY = 2 * _MASS_Y + 5;

	MapInit();
	PlayerInit();

	GAMESTATE = GAME_INITB;
	DrawFormatString( 0, 0, 0xffffff, "2" );

}

void GameInitB(){
	DrawBox( 0, 0, _WINDOWSIZE_X, _WINDOWSIZE_Y, 0x000000, TRUE );
	static int inFrame = 0;
	DrawFormatString( 100, 100, 0xffffff, "%d", Player.Life );

	if ( inFrame++ > 90 ) {
		GAMESTATE = GAME_MAIN;
		inFrame = 0;
		DrawBox( 0, 0, _WINDOWSIZE_X, _WINDOWSIZE_Y, 0x000000, TRUE );
	}
}

//�Q�[�����C������
void GameMain() {
	if ( CheckSoundMem( Sound.bgm_ground ) == 0 ) {
		PlaySoundMem( Sound.bgm_ground, DX_PLAYTYPE_BACK );
	}
	if ( ( Player.DeathFlg == 1 || Player.GoalFlg != 0 ) && CheckSoundMem( Sound.bgm_ground ) == 1 ) {
		StopSoundMem( Sound.bgm_ground );
	}

	DrawStage();		//�X�e�[�W�`��
	DrawFlag();			//�S�[���t���O
	DrawPlayer();		//�v���C���[�`��
	DrawEnemy();		//�G�`��
	//DrawKinoko();		//�L�m�R�`��

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
	DrawFormatString( 0, 0, 0xffffff, "3" );
}

//�X�e�[�W�`��
void DrawStage() {

	//if ( /*Player.GoalFlg == 0 || */GAMESTATE == GAME_MAIN ) {
		//PlaySoundMem( Sound.bgm_ground, DX_PLAYTYPE_BACK );
	//}
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

	for ( int i = 0; i < _ENEMY_NUM; i++ ) {
		if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
			Enemy[ i ].EnemyX -= Player.MapSSpeed;
		}
	}
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				map[ StageY ][ StageX ].CoX -= Player.MapSSpeed;
				if ( map[ StageY ][ StageX ].ItemFlg == _ITEM_BRING || map[ StageY ][ StageX ].ItemFlg == _ITEM_ANIME ) {
					map[ StageY ][ StageX ].ItemX -= Player.MapSSpeed;
				}
			}
		}
	}

	//�}�b�v�`��
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				//�R�C��&�p���[�A�b�v�A�C�e���̃A�j���[�V����
				if ( map[ StageY ][ StageX ].ItemFlg == _ITEM_ANIME || map[ StageY ][ StageX ].ItemFlg == _ITEM_MOVIN ) {
					//��������(�n�e�i�u���b�N�ύX)
					if ( map[ StageY ][ StageX ].MapNum == 2 || map[ StageY ][ StageX ].MapNum == 3 || map[ StageY ][ StageX ].MapNum == 4 || map[ StageY ][ StageX ].MapNum == 5 ) {
						map[ StageY ][ StageX ].MapNum = 1;
						if ( map[ StageY ][ StageX ].ItemType == _ITEMT_COIN ) {
							Player.CoinNum++;
						}
					}
					if ( DrawItem( StageX, StageY ) == TRUE ) {
						map[ StageY ][ StageX ].ItemFlg = _ITEM_NHAVE;
					}
				}

				//�n�e�i�u���b�N�̐F�̑J��
				if( map[StageY][StageX].MapNum==2||map[StageY][StageX].MapNum==3||map[StageY][StageX].MapNum==4||map[StageY][StageX].MapNum==5){
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
		PlaySoundMem( Sound.coin, DX_PLAYTYPE_BACK, FALSE );
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

		//�m�[�}���}���I���L�m�R����鏈��
		if ( Player.PlayerState == 1 ) {
			if ( map[ iY ][ iX ].ItemFrame++ < 1 ) {
				map[ iY ][ iX ].ItemY -= 10;
			}
			else if ( map[ iY ][ iX ].ItemFrame < 25 ) {
				map[ iY ][ iX ].ItemY -= 1;
				PlaySoundMem( Sound.kinoko, DX_PLAYTYPE_BACK, FALSE );
			}
			else if ( map[ iY ][ iX ].ItemFrame >= 28 ) {
				//���E�ɓ���
				if ( map[ iY ][ iX ].ItemDirect == _DIRECT_RIGHT ) {
					if ( HitBlockRight( map[ iY ][ iX ].ItemX + Player.MapScrollX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
						map[ iY ][ iX ].ItemX += 2;
					} else if ( HitBlockRight( map[ iY ][ iX ].ItemX + Player.MapScrollX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
						map[ iY ][ iX ].ItemDirect = _DIRECT_LEFT;
					}
				}
				else if ( map[ iY ][ iX ].ItemDirect == _DIRECT_LEFT ) {
					if ( HitBlockLeft( map[ iY ][ iX ].ItemX + Player.MapScrollX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
						map[ iY ][ iX ].ItemX -= 2;
					} else if ( HitBlockLeft( map[ iY ][ iX ].ItemX + Player.MapScrollX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_TRUE ) {
						map[ iY ][ iX ].ItemDirect = _DIRECT_RIGHT;
					}
				}
				/*****     ��������     *****/
				if ( HitBlockDown( map[ iY ][ iX ].ItemX + Player.MapScrollX, map[ iY ][ iX ].ItemY, _MASS_HALF , _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
					if ( map[ iY ][ iX ].IYSpeed < 8.0f )	map[ iY ][ iX ].IYSpeed += 0.8f;
					map[ iY ][ iX ].ItemY += ( int )map[ iY ][ iX ].IYSpeed;
				} else {
					map[ iY ][ iX ].IYSpeed = 0.0f;
				}
			}
			//�����蔻��
			if ( HitObj( Player.PlayerX, Player.PlayerY, map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
				PlaySoundMem(Sound.powerUp,DX_PLAYTYPE_BACK,FALSE);
				Player.PlayerState = 2;
				Player.PlayerAnime = 1;
				return TRUE;
			}
			//��ʊO�ɏ������ꍇ
			if ( map[ iY ][ iX ].ItemY >= ( 15 * _MASS_Y ) || map[ iY ][ iX ].ItemX >= ( 15 * _MASS_X ) || map[ iY ][ iX ].ItemX < ( 0 - _MASS_X ) ) {
				return TRUE;
			}
			//�`��
			DrawRotaGraph( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, 1.0f, 0, Pic.StageBlock[ 13 ], TRUE );
		}
		//�X�[�p�[�}���I���t�@�C���[�}���I�̎��̏���
		else if ( Player.PlayerState >= 2 ) {
			if ( map[ iY ][ iX ].ItemFrame++ < 1 ) {
				map[ iY ][ iX ].ItemY -= 10;
			}
			else if ( map[ iY ][ iX ].ItemFrame < 25 ) {
				map[ iY ][ iX ].ItemY -= 1;
			}
			//�����蔻��
			if ( HitObj( Player.PlayerX, Player.PlayerY, map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
				if ( Player.PlayerState == 2 ) {
					PlaySoundMem(Sound.powerUp,DX_PLAYTYPE_BACK,FALSE);
					Player.PlayerAnime = 1;
					Player.PlayerState = 3;
				} else if ( Player.PlayerState == 3 ) {
					Player.Point += 100;
				}
				return TRUE;
			}
			//��ʊO���Ə����鏈��
			if ( map[ iY ][ iX ].ItemY >= ( 15 * _MASS_Y ) || map[ iY ][ iX ].ItemX >= ( 15 * _MASS_X ) || map[ iY ][ iX ].ItemX < ( 0 - _MASS_X ) ) {
				return TRUE;
			}
			//�`��
			DrawRotaGraph( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, 1.0f, 0, Pic.StageBlock[ 25 ], TRUE );
		}
	}

	return FALSE;

}

//�v���C���[�`��
void DrawPlayer() {

	static int i = 0;
	static int PDrawMode = 0;	//0=�����ĂȂ��A1=�E�����Ă���A 2=�������Ă�
	static int SlideMode = 0;	//1=�E����A2=������
	static int JumpState = 1;	//�W�����v�̏�Ԃ������Ă���
	int InMode = 0;
	int InMode2 = 0;
	int WalkMove = 9 - ( int )Player.PSpeed;
	if ( ( ( opt.NowK & PAD_INPUT_M ) == 0 ) && JumpState == 1 ) {
		JumpState = 0;
	}
	i++;

	//�މ����̈ꎞ�I�Ȗ��G���
	if ( Player.dgnFlg == 1 ) {
		static int inFlg = 0;
		if ( inFlg++ > 120 ) {
			Player.dgnFlg = 0;
			inFlg = 0;
		}
	}

	//�ޗ��ɗ�������
	if ( Player.PlayerY >= ( 15 * _MASS_Y ) ) {
		Player.DeathFlg = 1;
	}

	//�����A�j���[�V����
	if ( Player.PlayerAnime == 0 || Player.DeathFlg == 0 ) {
		if( ( 0 == FR_Control.FrameCount % WalkMove ) && /*opt.NowK != NULL*/ Player.PSpeed >= 0.0f ){
			if ( Player.JumpFrame == 0 )	Player.P_i_f++;
			if(Player.P_i_f >= 3 && Player.JumpFrame == 0 )	Player.P_i_f=0;
			if ( Player.JumpFrame > 0 )	Player.P_i_f = 3;
		}
		else if( Player.PSpeed <= 0.0f ) {
			Player.P_i_f = 0;	//�L�[�������߂���
		}
	}

	//���s����
	/*****     �E�ړ�����     *****/
	if ( Player.GoalFlg == 0 && Player.PlayerX <= ( 15 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {

		/** �ʏ폈�� **/
		if ( PDrawMode == 0 || ( PDrawMode == 2 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 1;
		}
		if ( PDrawMode == 1 ) {
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				//�m�[�}���}���I����
				if ( Player.PlayerState == 1 ) {
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
				}
				//�X�[�p�[�}���I�ȏゾ��
				else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
					if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
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
				
					} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
						Player.GoalFlg = 1;
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
						Player.PJSpeed = 0;
						JumpState = 0;
						Player.JumpFrame = 0;
					}
				}
			}
			Player.P_lr_f = _DIRECT_RIGHT;					//���E���]�t���O
			if ( Player.DeathFlg == 0 ) {
				//���s���̃v���C���[�`��
				if ( Player.PlayerState == 1 ) {									//�m�[�}���}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		
				}
				else if ( Player.PlayerState == 2 ) {								//�X�[�p�[�}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkS[Player.P_i_f], TRUE, FALSE );		
				}
				else if ( Player.PlayerState == 3 ) {								//�t�@�C���[�}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkF[Player.P_i_f], TRUE, FALSE );	
				}
			}
		}
		/** �}�u���[�L�̏��� **/
		if ( PDrawMode == 2 && Player.PSpeed > 0.0f ) {
			Player.P_lr_f=0;					//���E���]�t���O
			SlideMode = 1;
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				//�m�[�}���}���I����
				if ( Player.PlayerState == 1 ) {
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
				}
				//�X�[�p�[�}���I�ȏゾ��
				else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
					if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						Player.PSpeed -= 0.4f;
						if ( ( Player.PlayerX -= Player.PSpeed ) < ( 0 * _MASS_X + _MASS_HALF ) ) {
							Player.PlayerX += Player.PSpeed;
							Player.PSpeed = 0.0f;
							Player.MapSSpeed = 0;
						}
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
						GAMESTATE = GAME_GOAL;
					}
					if ( Player.PSpeed <= 0.0f ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
						PDrawMode = 1;
					}
				}
			}
			//�v���C���[�`��
			if ( Player.DeathFlg == 0 ) {
				if ( Player.PlayerState == 1 ) {									//�m�[�}���}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, FALSE );
				}
				else if ( Player.PlayerState == 2 ) {								//�X�[�p�[�}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[ 7 ], TRUE, FALSE );
				}
				else if ( Player.PlayerState == 3 ) {								//�t�@�C���[�}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[ 7 ], TRUE, FALSE );
				}
			}
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
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				//�m�[�}���}���I����
				if ( Player.PlayerState == 1 ) {
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
				}
				//�X�[�p�[�}���I�ȏ㏈��
				else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
					if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						if ( Player.PSpeed < 6.0f ) {
							Player.PSpeed += 0.2f;			//�����x�ݒ�
						}
						Player.PlayerX -= Player.PSpeed;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
						GAMESTATE = GAME_GOAL;
					}
				}
			}
			Player.P_lr_f = _DIRECT_LEFT;					//���E���]�t���O
			if ( Player.DeathFlg == 0 ) {
				//�v���C���[�`��
				if ( Player.PlayerState == 1 ) {
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//���s���̃v���C���[�`��
				} 
				else if ( Player.PlayerState == 2 ) {
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkS[Player.P_i_f], TRUE, TRUE );			//���s���̃v���C���[�`��
				}
				else if ( Player.PlayerState == 3 ) {
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkF[Player.P_i_f], TRUE, TRUE );			//���s���̃v���C���[�`��
				}
			}
		}
		/** �}�u���[�L�̏��� **/
		if ( PDrawMode == 1 && Player.PSpeed > 0.0f ) {
			SlideMode = 2;
			InMode2 = 2;
			Player.P_lr_f=1;					//���E���]�t���O
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				//�m�[�}���}���I����
				if ( Player.PlayerState == 1 ) {
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
				}
				//�X�[�p�[�}���I�ȏ㏈��
				else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
						if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						Player.PSpeed -= 0.4f;
						if ( Player.PlayerX < ( 8 * _MASS_X + _MASS_HALF ) && ( Player.MapScrollX + Player.PlayerX < ( 200 * _MASS_X + _MASS_HALF ) ) ) {
							Player.PlayerX += Player.PSpeed;
						}
						else if ( Player.PlayerX >= ( 8 * _MASS_X + _MASS_HALF ) ) {
							Player.MapSSpeed = Player.PSpeed;
							Player.MapScrollX += Player.MapSSpeed;
						}
					} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
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
				}
			}
			if ( Player.DeathFlg == 0 ) {
				if ( Player.PlayerState == 1 ) {
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, TRUE );
				} 
				else if ( Player.PlayerState == 2 ) {
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[ 7 ], TRUE, TRUE );
				}
				else if ( Player.PlayerState == 3 ) {
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[ 7 ], TRUE, TRUE );
				}
			}
		}
	}
	/*****  ���̑�����   *****/
	else {
		if ( Player.PSpeed > 0.0f ) {
			if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
				Player.PSpeed -= 0.2f;
				/*****  �}���I���菈��   *****/
				if ( Player.PSpeed > 0.0f ) {
					//�E����
					if ( PDrawMode == 1 && ( Player.PlayerX < 15 * _MASS_X + _MASS_HALF ) ) { 
						//�m�[�}���}���I����
						if ( Player.PlayerState == 1 ) {
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
						}
						//�X�[�p�[�}���I�ȏ㏈��
						else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
							if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
								if ( Player.PlayerX < ( 8 * _MASS_X + _MASS_HALF ) && ( Player.MapScrollX + Player.PlayerX <= ( 200 * _MASS_X + _MASS_HALF ) ) ) {
									Player.PlayerX += Player.PSpeed;
								}
								else if ( Player.PlayerX >= ( 8 * _MASS_X + _MASS_HALF ) ) {
									Player.MapSSpeed = Player.PSpeed;
									Player.MapScrollX += Player.MapSSpeed;
								}
							} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
								Player.PSpeed = 0.0f;
								Player.MapSSpeed = 0;
							} else if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
								Player.GoalFlg = 1;
								Player.PSpeed = 0.0f;
								Player.MapSSpeed = 0;
								Player.PJSpeed = 0;
								JumpState = 0;
								Player.JumpFrame = 0;
							}
						}
					} 
					//������
					else if ( PDrawMode == 2 && ( Player.PlayerX > 0 * _MASS_X + _MASS_HALF ) ) {
						//�m�[�}���}���I����
						if ( Player.PlayerState == 1 ) {
							if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
								Player.PlayerX -= Player.PSpeed;
							} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
								Player.PSpeed = 0.0f;
								Player.MapSSpeed = 0;
							} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
								GAMESTATE = GAME_GOAL;
							}
						}
						//�X�[�p�[�}���I�ȏ㏈��
						else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
							if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
								Player.PlayerX -= Player.PSpeed;
							} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
								Player.PSpeed = 0.0f;
								Player.MapSSpeed = 0;
							} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
								GAMESTATE = GAME_GOAL;
							}
						}
					}
				}
			}
			if ( Player.DeathFlg == 0 ) {
				//�v���C���[�`��
				if ( Player.PlayerState == 1 ) {				//�m�[�}���}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ Player.P_i_f ], TRUE , Player.P_lr_f );
				}
				else if ( Player.PlayerState == 2 ) {			//�X�[�p�[�}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[ Player.P_i_f ], TRUE , Player.P_lr_f );
				}
				else if ( Player.PlayerState == 3 ) {			//�t�@�C���[�}���I
					DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[ Player.P_i_f ], TRUE , Player.P_lr_f );
				}
			}
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
	
	if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {
		if ( Player.JumpMode == TRUE ) {
			if ( Player.PlayerState == 1 ) {
				//������̃u���b�N�̓����蔻��
				if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {			
					//�^�񒆂Ƀq�b�g����ꍇ
					Player.PJSpeed = 0.0f;
					Player.JumpMode = FALSE;
					Player.JumpFrame = 0;
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
			}
			else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
				//������̃u���b�N�̓����蔻��
				if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {			
					//�^�񒆂Ƀq�b�g����ꍇ
					Player.PJSpeed = 0.0f;
					Player.JumpMode = FALSE;
					Player.JumpFrame = 0;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_LEFT ) {	
					//(�}���I��)�����ɓ���������
					int Difference = 0;
					Difference = map[ ( Player.PlayerY-_MASS_HALF+4 )/32][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX -
									( ( Player.PlayerX ) - _MASS_HALF + 4 );
					Player.PlayerX += Difference;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_RIGHT ) {	
					//(�}���I��)�����ɓ���������
					int Difference = 0;
					Difference = ( ( Player.PlayerX )+_MASS_HALF-4 ) - 
								map[ ( Player.PlayerY-_MASS_HALF+4 )/32 ][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX;
					Player.PlayerX -= Difference;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {	
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
			}

		} else if ( JumpState == 2 && Player.JumpMode == FALSE ) {
			JumpState = 3;
		}
	}

	/*****     ��������     *****/
	if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {

		//�m�[�}���}���I����
		if ( Player.PlayerState == 1 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				JumpState = 4;
				Player.DownFlg = 1;
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
					Player.DownFlg = 0;
				}
				if ( JumpState == 4 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
					JumpState = 0;
					Player.DownFlg = 0;
				}
			}
			else {
				Player.PYSpeed = 0.0f;
			}
		}
		//�X�[�p�[�}���I�ȏ㏈��
		else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				JumpState = 4;
				Player.DownFlg = 1;
				if ( Player.PYSpeed < 8.0f )	Player.PYSpeed += 0.8f;
				Player.PlayerY += ( int )Player.PYSpeed;
				//�߂荞�ݖh�~����
				if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
					int Difference = 0;
					Difference = ( Player.PlayerY + _MASS_Y ) - map[ ( Player.PlayerY / 32 ) + 1 ][ ( Player.MapScrollX + Player.PlayerX )/32 ].CoY;
					Player.PlayerY -= Difference;
					Player.PYSpeed = 0.0f;
				}
			} 
			else if ( Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
				if ( JumpState == 3 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
					JumpState = 0;
					Player.DownFlg = 0;
				}
				if ( JumpState == 4 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
					JumpState = 0;
					Player.DownFlg = 0;
				}
			}
			else {
				Player.PYSpeed = 0.0f;
			}
		}
	}

	//�p���[�A�b�v�A�j���[�V����
	if ( Player.PlayerAnime == 1 && Player.DeathFlg == 0 ) {
		if ( Player.PAnimeFrame++ < 1 ) {
			if ( Player.PlayerState == 2 ) {
				Player.PlayerY -= _MASS_HALF;
			}
		}
		else if ( Player.PAnimeFrame > 90 ) {
			Player.PlayerAnime = 0;
			Player.PAnimeFrame = 0;
		}
	}

	//�S�[���A�j���[�V����
	static int pg_walk;
	if ( Player.GoalFlg == 1 ) {
		//�m�[�}���}���I����
		if ( Player.PlayerState == 1 ) {
			if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) != _HIT_TRUE ) {
				PlaySoundMem( Sound.pole, DX_PLAYTYPE_BACK, FALSE );
				Player.PlayerY += 3;
			}
		}
		//�X�[�p�[�}���I�ȏ㏈��
		else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
			if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) != _HIT_TRUE ) {
				PlaySoundMem( Sound.pole, DX_PLAYTYPE_BACK, FALSE );
				Player.PlayerY += 3;
			}
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
			StopSoundMem( Sound.pole );
			PlaySoundMem( Sound.goal, DX_PLAYTYPE_BACK, FALSE );
		}
		//�m�[�}���}���I����
		if ( Player.PlayerState == 1 ) {
			if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_CASTLE ){
				Player.GoalFlg=4;
				GAMESTATE=GAME_GOAL;
			}
		}
		//�X�[�p�[�}���I�ȏ㏈��
		else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
			if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_CASTLE ){
				Player.GoalFlg=4;
				GAMESTATE=GAME_GOAL;
			}
		}
	}

	//�S�[���摜�`��
	if ( Player.PlayerState == 1 ) {			//�m�[�}���}���I
		static int i=7;
		if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
			if(i==8)i=7;
			else if(i==7)i=8;
		}
		if ( Player.GoalFlg == 1 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[i], TRUE, FALSE );			//�S�[��������
		} else if ( Player.GoalFlg == 2 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[7], TRUE, TRUE );			//�S�[������(��ԉ��ɍ~�肽)�Ƃ�(���])
		}
	}
	else if ( Player.PlayerState == 2 ) {		//�X�[�p�[�}���I
		static int i=8;
		if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
			if(i==9)i=8;
			else if(i==8)i=9;
		}
		if ( Player.GoalFlg == 1 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[i], TRUE, FALSE );			//�S�[��������
		} else if ( Player.GoalFlg == 2 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[7], TRUE, TRUE );			//�S�[������(��ԉ��ɍ~�肽)�Ƃ�(���])
		}
	}
	else if ( Player.PlayerState == 3 ) {		//�t�@�C���[�}���I
		static int i=8;
		if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
			if(i==9)i=8;
			else if(i==8)i=9;
		}
		if ( Player.GoalFlg == 1 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[i], TRUE, FALSE );			//�S�[��������
		} else if ( Player.GoalFlg == 2 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[7], TRUE, TRUE );			//�S�[������(��ԉ��ɍ~�肽)�Ƃ�(���])
		}
	}

	//���S����
	if ( Player.DeathFlg == 1 ) {
		static int waitTime = 0;
		static int moveFlg = 0;
		if ( waitTime++ < 1 ) {
			Player.Life--;
			if ( CheckSoundMem( Sound.bgm_death ) == 0 ) {
				PlaySoundMem( Sound.bgm_death, DX_PLAYTYPE_BACK );
			}
			//Player.PJSpeed = 0.0f;
			Player.PYSpeed = 0.0f;
		}
		else if ( waitTime > 180 ) {
			GAMESTATE = GAME_INIT;
			DrawBox( 0, 0, _WINDOWSIZE_X, _WINDOWSIZE_Y, 0x000000, TRUE );
			waitTime = 0;
			moveFlg = 0;
		}

		//if ( waitTime > 30 && Player.PJSpeed > 0.0f ) {
		//	Player.PJSpeed -= 0.2f;
		//	Player.PlayerY -= Player.PJSpeed;
		//}
		//if ( Player.PJSpeed <= 0.0f && Player.PYSpeed <= 0.0f ) {
		//	Player.PYSpeed += 0.2f;
		//	Player.PlayerY += Player.PYSpeed;
		//}
		//if ( Player.PYSpeed > 0.0f ) {
		//	if ( Player.PYSpeed <= 8.0f ) {
		//		Player.PYSpeed += 0.2f;
		//	}
		//	Player.PlayerY += Player.PYSpeed;
		//}

		if ( waitTime > 30 && moveFlg == 0 && Player.PYSpeed > -6.0f ) {
			Player.PYSpeed -= 0.4f;
		}
		else if ( waitTime > 30 && moveFlg == 0 && Player.PYSpeed <= 8.0f ) {
			moveFlg = 1;
		}
		if ( waitTime > 30 && moveFlg == 1 && Player.PYSpeed <= 8.0f ) {
			Player.PYSpeed += 0.4f;
		}
		Player.PlayerY += Player.PYSpeed;

	}


	//�v���C���[�`��
	if ( Player.DeathFlg == 0 ) {
		if ( Player.PlayerState == 1 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );	//�W�����v���ĂȂ��Ƃ�
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );	//�W�����v������
		}
		else if ( Player.PlayerState == 2 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[0], TRUE , Player.P_lr_f );	//�W�����v���ĂȂ��Ƃ�
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[4], TRUE , Player.P_lr_f );	//�W�����v������
		}
		else if ( Player.PlayerState == 3 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[0], TRUE , Player.P_lr_f );	//�W�����v���ĂȂ��Ƃ�
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[4], TRUE , Player.P_lr_f );	//�W�����v������
		}
	}

	//���S���̉摜�`��
	else if ( Player.DeathFlg == 1 ) {
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[5], TRUE , FALSE );	//�W�����v������
	}

#ifdef _DEBUGMODE
	SetFontSize( 12 );
	DrawFormatString( 420,  50, 0xff0000, "OldK = %d", i );				//OldK�`��
	DrawFormatString( 420,  70, 0xff0000, "NowK = %d", opt.NowK );				//NowK�`��
	DrawFormatString( 420,  90, 0xff0000, "PAnm = %d", Player.PlayerAnime );	//PDrawMode�`��
	DrawFormatString( 420, 110, 0xff0000, "Life = %d", Player.Life );			//P_lr_f�`��
	DrawFormatString( 420, 130, 0xff0000, "PlrX = %d", Player.PlayerX );		//PlayerX�`��
	DrawFormatString( 420, 150, 0xff0000, "PlrY = %d", Player.PlayerY );		//PlayerY�`��
	DrawFormatString( 420, 170, 0xff0000, "Scrl = %d", Player.MapScrollX );		//Scroll�`��
	DrawFormatString( 420, 190, 0xff0000, "DthF = %d", Player.DeathFlg );		//PSpeed�`��
	DrawFormatString( 420, 210, 0xff0000, "Pint = %d", Player.Point );			//InMode�`��
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

//�G�l�~�[�`��
void DrawEnemy() {

	if ( Player.PlayerAnime == 0 && Player.DeathFlg == 0 ) {

		for ( int i = 0; i < _ENEMY_NUM; i++ ) {

			if ( Enemy[ i ].EnemyX < ( 18 * _MASS_X ) ) {

				if ( Enemy[ i ].PressFlg == 0 ) {
					//���E�ɓ���
					if ( Enemy[ i ].EDirect == _DIRECT_RIGHT ) {
						if ( HitBlockRight( Enemy[ i ].EnemyX + Player.MapScrollX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_ENEMY, FALSE, _HITW_ENEMY ) == _HIT_FALSE ) {
							Enemy[ i ].EnemyX += 1;
						} else if ( HitBlockRight( Enemy[ i ].EnemyX + Player.MapScrollX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_ENEMY, FALSE, _HITW_ENEMY ) == _HIT_TRUE ) {
							Enemy[ i ].EDirect = _DIRECT_LEFT;
						}
					}
					else if ( Enemy[ i ].EDirect == _DIRECT_LEFT ) {
						if ( HitBlockLeft( Enemy[ i ].EnemyX + Player.MapScrollX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_ENEMY, FALSE, _HITW_ENEMY ) == _HIT_FALSE ) {
							Enemy[ i ].EnemyX -= 1;
						} else if ( HitBlockLeft( Enemy[ i ].EnemyX + Player.MapScrollX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_ENEMY, FALSE, _HITW_ENEMY ) == _HIT_TRUE ) {
							Enemy[ i ].EDirect = _DIRECT_RIGHT;
						}
					}
					/*****     ��������     *****/
					if ( HitBlockDown( Enemy[ i ].EnemyX + Player.MapScrollX, Enemy[ i ].EnemyY, _MASS_HALF , _MASS_ENEMY, FALSE, _HITW_ENEMY ) == _HIT_FALSE ) {
						if ( Enemy[ i ].EYSpeed < 8.0f )	Enemy[ i ].EYSpeed += 0.8f;
						Enemy[ i ].EnemyY += ( int )Enemy[ i ].EYSpeed;
					} else {
						Enemy[ i ].EYSpeed = 0.0f;
					}

					//�v���C���[�Ƃ̓����蔻��(�m�[�}���}���I)
					if ( Player.PlayerState == 1 && Player.dgnFlg == 0 ) {
						if ( Player.DownFlg == 0 ) {
							if ( HitObj( Player.PlayerX, Player.PlayerY, Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
								//Player.DeathFlg = 1;
								Player.PlayerState = 1;
								Player.dgnFlg = 1;
							}
						}
						else if ( Player.DownFlg == 1 ) {
							if ( HitObjDown( Player.PlayerX, Player.PlayerY, Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
								Enemy[ i ].PressFlg = 1;
								Player.JumpMode = TRUE;
								Player.JumpFrame = 0;
								Player.PJSpeed = 7.0f;
							} 
							else if ( HitObjDown( Player.PlayerX, Player.PlayerY, Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, _MASS_HALF, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_DEATH ) {
								//Player.DeathFlg = 1;
								Player.PlayerState = 1;
								Player.dgnFlg = 1;
							}
						}
					}
					//�v���C���[�Ƃ̓����蔻��(�X�[�p�[�}���I�ȏ�)
					else if ( ( Player.PlayerState == 2 || Player.PlayerState == 3 ) && Player.dgnFlg == 0 ) {
						if ( Player.DownFlg == 0 ) {
							if ( HitObj( Player.PlayerX, Player.PlayerY, Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, _MASS_Y, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
								Player.DeathFlg = 1;
							}
						}
						else if ( Player.DownFlg == 1 ) {
							if ( HitObjDown( Player.PlayerX, Player.PlayerY, Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, _MASS_Y, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
								Enemy[ i ].PressFlg = 1;
								Player.JumpMode = TRUE;
								Player.JumpFrame = 0;
								Player.PJSpeed = 7.0f;
							} 
							else if ( HitObjDown( Player.PlayerX, Player.PlayerY, Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, _MASS_Y, _MASS_HALF, _MASS_HALF, _MASS_HALF ) == _HIT_DEATH ) {
								Player.DeathFlg = 1;
							}
						}
					}
				}
				else if ( Enemy[ i ].PressFlg == 1 ) {
					static int eliminate = 0;
					if ( eliminate++ > 60 ) {
						Enemy[ i ].PressFlg = 2;
						eliminate = 0;
					}
				}
			}
			
			if ( Enemy[ i ].EnemyX < -( 4 * _MASS_X ) ) {
				Enemy[ i ].PressFlg = 2;
			}
		}
	}

	for ( int i = 0; i < _ENEMY_NUM; i++ ) {
		if ( Enemy[ i ].EnemyType == 1 ) {
			static int kuriF = 0;
			if ( 0 == FR_Control.FrameCount % 5 ) {
				if ( kuriF == 0 )	kuriF = 1;
				else if ( kuriF == 1 )	kuriF = 0;
			}
			if ( Enemy[ i ].PressFlg == 0 ) {
				DrawRotaGraph( Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, 1.0f, 0, Pic.EnemyMob[ 0 ], TRUE, kuriF );
			}
			else if ( Enemy[ i ].PressFlg == 1 ) {
				DrawRotaGraph( Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, 1.0f, 0, Pic.EnemyMob[ 1 ], TRUE, FALSE );
			}
		}
		else if ( Enemy[ i ].EnemyType == 2 ) {
			static int nokoF = 2;
			if ( 0 == FR_Control.FrameCount % 5 ) {
				if ( nokoF == 2 )	nokoF = 3;
				else if ( nokoF == 3 )	nokoF = 2;
			}
			if ( Enemy[ i ].PressFlg == 0 ) {
				DrawRotaGraph( Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, 1.0f, 0, Pic.EnemyMob[ nokoF ], TRUE, Enemy[ i ].EDirect );
			}
			else if ( Enemy[ i ].PressFlg == 1 ) {
				DrawRotaGraph( Enemy[ i ].EnemyX, Enemy[ i ].EnemyY, 1.0f, 0, Pic.EnemyMob[ 11 ], TRUE, FALSE );
			}
		}
	}

#ifdef _DEBUGMODE
	DrawFormatString( 20, 50, 0xff0000, "EnemyX = %d", Enemy[ 3 ].EnemyX );
	DrawFormatString( 20, 80, 0xff0000, "EnemyY = %d", Enemy[ 3 ].EnemyY );
#endif

}

//�摜�Ǎ�
int LoadImages() {

	//�^�C�g���C���[�W�ǂݍ���
	if ( ( Pic.TitleImg = LoadGraph( "images/TitleImage01.png" ) ) == -1 )	return -1;
	//�u���b�N�Ǎ�
	if ( LoadDivGraph( "images/backimage.png", 84, 12, 7, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//�L�����N�^�[�Ǎ�
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )			return -1;
	//�X�[�p�[�}���I�ǂݍ���
	if ( LoadDivGraph( "images/super_mario_chara.png", 15, 5, 3, 32, 64, Pic.Suplayer ) == -1 )	return -1;
	//�t�@�C���[�}���I�ǂݍ���
	if ( LoadDivGraph( "images/fire_mario_chara .png", 15, 5, 3, 32, 64, Pic.FirePlayer ) == -1 )	return -1;
	//�G�ǂݍ���
	if ( LoadDivGraph( "images/mob.png", 20, 10, 2, 32, 48, Pic.EnemyMob ) == -1 )	return -1;

	Pic.P_Walk[0]=Pic.Player[1];
	Pic.P_Walk[1]=Pic.Player[2];
	Pic.P_Walk[2]=Pic.Player[3];
	Pic.P_Walk[3]=Pic.Player[4];

	Pic.P_WalkS[ 0 ] = Pic.Suplayer[ 1 ];
	Pic.P_WalkS[ 1 ] = Pic.Suplayer[ 2 ];
	Pic.P_WalkS[ 2 ] = Pic.Suplayer[ 3 ];
	Pic.P_WalkS[ 3 ] = Pic.Suplayer[ 4 ];

	Pic.P_WalkF[ 0 ] = Pic.FirePlayer[ 1 ];
	Pic.P_WalkF[ 1 ] = Pic.FirePlayer[ 2 ];
	Pic.P_WalkF[ 2 ] = Pic.FirePlayer[ 3 ];
	Pic.P_WalkF[ 3 ] = Pic.FirePlayer[ 4 ];

	return TRUE;
}

int LoadSounds() {
	// �R�C��
	if ((Sound.coin = LoadSoundMem("BGM_SE/coin.wav")) == -1)					return -1;
	// �L�m�R
	if ((Sound.kinoko = LoadSoundMem("BGM_SE/mario_powerup.wav")) == -1)		return -1;
	// �|�[��
	if ((Sound.pole = LoadSoundMem("BGM_SE/mario_goalflag.wav")) == -1)			return -1;
	// �S�[��
	if ((Sound.goal = LoadSoundMem("BGM_SE/bgm_goal.wav")) == -1)				return -1;
	//�p���[�A�b�v
	if ((Sound.powerUp = LoadSoundMem("BGM_SE/powerup.wav")) == -1)				return -1;
	//BGM
	if ( ( Sound.bgm_ground = LoadSoundMem( "BGM_SE/bgm_tijou.wav" ) ) == -1 )	return -1;
	//���S��
	if ( ( Sound.bgm_death = LoadSoundMem( "BGM_SE/mario_death.wav" ) ) == -1 )	return -1;
	//�u���b�N�����˂�Ƃ��̉�
	if ( ( Sound.BounceBlock = LoadSoundMem( "BGM_SE/block_ataru.wav" ) ) == -1 )	return -1;
	//�u���b�N�����鉹
	if ( ( Sound.BreakBlock = LoadSoundMem( "BGM_SE/block_break.wav" ) ) == -1 )	return -1;

	return 0;
}

//�}�b�v��������
void MapInit() {
	static int i = 0;
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
			map[ StageY ][ StageX ].Block_UP = 0;
			map[ StageY ][ StageX ].BU_F = 0;
			map[ StageY ][ StageX ].IYSpeed = 0.0f;
			map[ StageY ][ StageX ].ItemX = 0;
			map[ StageY ][ StageX ].ItemY = 0;
			map[ StageY ][ StageX ].ItemFrame = 0;
			//�n�e�i�u���b�N�������ꍇ
			if ( Map[ StageY ][ StageX ] == 2 ) {
				map[ StageY ][ StageX ].ItemFlg = _ITEM_BRING;
				map[ StageY ][ StageX ].ItemType = _ITEMT_COIN;
				map[ StageY ][ StageX ].ItemX = StageX * _MASS_X + _MASS_HALF;
				map[ StageY ][ StageX ].ItemY = StageY * _MASS_Y + _MASS_HALF;
			}
			//�̂��̂�
			if ( Map[ StageY ][ StageX ] == 81 ) {
				Enemy[ i ].EnemyType = 2;
				Enemy[ i ].EnemyX = ( StageX * _MASS_X ) + _MASS_HALF;
				Enemy[ i ].EnemyY = ( StageY * _MASS_Y ) + _MASS_HALF - 8;
				Enemy[ i ].EDirect = _DIRECT_LEFT;
				Enemy[ i ].ESpeed = 0.0f;
				Enemy[ i ].EYSpeed = 0.0f;
				Enemy[ i ].PressFlg = 0;
				i++;
			}
			//�N���{�[
			else if ( Map[ StageY ][ StageX ] == 72 ) {
				Enemy[ i ].EnemyType = 1;
				Enemy[ i ].EnemyX = ( StageX * _MASS_X ) + _MASS_HALF;
				Enemy[ i ].EnemyY = ( StageY * _MASS_Y ) + _MASS_HALF - 8;
				Enemy[ i ].EDirect = _DIRECT_LEFT;
				Enemy[ i ].ESpeed = 0.0f;
				Enemy[ i ].EYSpeed = 0.0f;
				Enemy[ i ].PressFlg = 0;
				i++;
			}
		}
	}
	i = 0;

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
		if ( who == _HITW_PLAYER ) {
			if ( map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				if ( map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].ItemFlg == _ITEM_BRING ) {
					map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].ItemFlg = _ITEM_ANIME;
				}
				if ( who == _HITW_PLAYER ) {
					if ( Player.PlayerState == 1 ) {
						if ( map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 1 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 2 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 3 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 4 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 5 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 6 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 7 ) {
								if ( map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum != 1 ) {
									map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].BU_F=1;
								}
								PlaySoundMem( Sound.BounceBlock, DX_PLAYTYPE_BACK );

						}

					}

					if ( Player.PlayerState == 2 || Player.PlayerState == 3 )  {
						if ( map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 2 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 3 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 4 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 5 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 6 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 7 ) {
								 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].BU_F=1;
						}
						if ( map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 1 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 2 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 3 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 4 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 5 ) {
								 PlaySoundMem( Sound.BounceBlock, DX_PLAYTYPE_BACK );
						}
						if ( map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 6 ||
							 map[ ( ( Player.PlayerY-_MASS_Y ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 7 ) {
								 PlaySoundMem( Sound.BreakBlock, DX_PLAYTYPE_BACK );
						}
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

		if ( who == _HITW_PWRUP ) {
		}
	}
	
	return _HIT_FALSE;
}

//�u���b�N�������蔻��
int HitBlockDown( int oX, int oY, int pX, int pY, int jMode, int who ) {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( who == _HITW_PLAYER ) {
			if ( map[ ( ( oY+pY )/32 ) ][ ( ( oX-pX+6 )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( oY+pY )/32 ) ][ ( ( oX+pX-6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
		else if ( who == _HITW_PWRUP ) {
			if ( map[ ( ( oY+pY )/32 ) ][ ( ( oX-pX+6 )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( oY+pY )/32 ) ][ ( ( oX+pX-6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
		else if ( who == _HITW_FLAG ) {
			if ( map[ ( ( oY+pY )/32 ) ][ ( ( oX-pX+6 )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( oY+pY )/32 ) ][ ( ( oX+pX-6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
		else if ( who == _HITW_ENEMY ) {
			if ( map[ ( ( oY+pY )/32 ) ][ ( ( oX-pX+6 )/32 ) ].MapNum == HitBlockNum[ i ]
			|| map[ ( ( oY+pY )/32 ) ][ ( ( oX+pX-6 )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
	}
	return _HIT_FALSE;
}

//�u���b�N�E�����蔻��
int HitBlockRight( int oX, int oY, int pX, int pY, int jMode, int who ) {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( who == _HITW_PLAYER ) {
			if ( Player.PlayerState == 1 ) {
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
			}
			else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
				if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] 
				  || map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ]
				  || map[ ( ( oY ) /32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
					return _HIT_TRUE;
				} 
				else if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 34
					   || map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 35
					   || map[ ( ( oY ) /32 ) ][ ( ( oX )/32 ) ].MapNum == 34
					   || map[ ( ( oY ) /32 ) ][ ( ( oX )/32 ) ].MapNum == 35 ) {
					return _HIT_GOAL;
				}
				else if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 70
					   || map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX )/32 ) ].MapNum == 70
					   || map[ ( ( oY ) /32 ) ][ ( ( oX )/32 ) ].MapNum == 70 ) {
					return _HIT_CASTLE;
				}
			}
		} else if ( who == _HITW_PWRUP ) {
			if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] 
			|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX+pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			} 
		}
		else if ( who == _HITW_ENEMY ) {
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
		if ( who == _HITW_PLAYER ) {
			if ( Player.PlayerState == 1 ) {
				if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
					|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
					return _HIT_TRUE;
				}
			}
			else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
				if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
					|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
					|| map[ ( ( oY )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
					return _HIT_TRUE;
				}
			}
		}
		else if ( who == _HITW_PWRUP ) {
			if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
				|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
		else if ( who == _HITW_ENEMY ) {
			if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
				|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
	}
	
	return _HIT_FALSE;
}

//�I�u�W�F�N�g�̓����蔻��
int HitObj( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad ) {

	//if ( ( ( oX - widthRad ) + ( widthRad * 2 ) < ( pX - widthRad ) ) &&
	//	   ( ( pX - widthRad ) + ( widthRad * 2 ) < ( oX - widthRad ) ) &&
	//	   ( ( oY - heightRad ) + ( heightRad * 2 ) < ( pY - heightRad ) ) &&
	//	   ( ( pY - heightRad ) + ( heightRad * 2 ) < ( oY - heightRad ) ) ) {
	//		return _HIT_FALSE;
	//}
	//if ( ( ( pX - widthRad ) > ( oX - widthRad ) + ( widthRad * 2 ) ) &&
	//	 ( ( oX - widthRad ) > ( pX - widthRad ) + ( widthRad * 2 ) ) &&
	//	 ( ( pY - heightRad ) > ( oY - heightRad ) + ( heightRad * 2 ) ) &&
	//	 ( ( oY - heightRad ) > ( pY - heightRad ) + ( heightRad * 2 ) ) ) {
	//		 return _HIT_FALSE;
	//}
	//if ( ( oY - _MASS_HALF ) > ( pY + _MASS_HALF ) && ( oY + _MASS_HALF ) < ( pY - _MASS_HALF ) &&
	//	 ( oX - _MASS_HALF ) > ( pX + _MASS_HALF ) && ( oX + _MASS_HALF ) < ( pX - _MASS_HALF ) ) {
	//		 return _HIT_FALSE;
	//}

	//if ( ( oX + _MASS_HALF ) > ( pX - _MASS_HALF ) && ( oX - _MASS_HALF ) < ( pX + _MASS_HALF ) ) {
	//	if ( ( oY + _MASS_HALF ) > ( pY - _MASS_HALF ) && ( oY - _MASS_HALF ) < ( pY + _MASS_HALF ) ) {
	//		return _HIT_TRUE;
	//	}
	//}

	if ( ( oX + owidthRad ) > ( pX - pwidthRad ) && ( oX - owidthRad ) < ( pX + pwidthRad ) ) {
		if ( ( oY + oheightRad ) > ( pY - pheightRad ) && ( oY - oheightRad ) < ( pY + pheightRad ) ) {
			return _HIT_TRUE;
		}
	}

	return _HIT_FALSE;

}

//�I�u�W�F�N�g�̓����蔻�蓥�ݕt��
int HitObjDown( int oX, int oY, int pX, int pY, int oheightRad, int owidthRad, int pheightRad, int pwidthRad ){

	//���E����
	if ( ( oX + owidthRad ) > ( pX - pwidthRad ) && ( oX - owidthRad ) < ( pX + pwidthRad ) ) {
		if ( ( oY + oheightRad ) > ( pY - pheightRad ) && ( oY - oheightRad ) < ( pY + pheightRad ) ) {
			return _HIT_TRUE;
			if ( ( oY - oheightRad ) > ( pY - pheightRad ) ) {
				return _HIT_DEATH;
			}
		}
	}
	return _HIT_FALSE;
}

//�v���C���[��������
void PlayerInit() {
	Player.PlayerX = (  3 * _MASS_X ) + _MASS_HALF;
	Player.PlayerY = ( 11 * _MASS_Y ) + _MASS_HALF;
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
	Player.PlayerAnime = 0;
	Player.PlayerState = 1;
	Player.DownFlg = 0;
	if ( Player.Life == 0 ) {
		Player.Life = 5;
	}
}

//�G��������
//void EnemyInit() {
//	Enemy.EnemyX = ( 10 * _MASS_X ) + _MASS_HALF;
//	Enemy.EnemyY = ( 11 * _MASS_Y ) + _MASS_HALF - 8;
//	Enemy.EDirect = _DIRECT_RIGHT;
//	Enemy.ESpeed = 0.0f;
//	Enemy.EYSpeed = 0.0f;
//	Enemy.PressFlg = 0;
//}