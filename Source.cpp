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

/*****		�v���C���[�n�萔		*****/
#define _PLAYER_MAXSPEED	3

//�����蔻��̂���u���b�N�̑���
#define _HITCHIP_ALL	21

//�}�b�v�S�T�C�Y
#define _MAP_ALLSIZE_X 220
#define _MAP_ALLSIZE_Y 14

//�Q�[����ԕϐ�
static int GAMESTATE;

//���t���b�V�����[�g�m�F�p�ϐ�
static int RefreshRate;
HDC hdc;

/*****      �Q�[�����[�h�񋓑�      *****/
typedef enum GAME_MODE {
	GAME_TITLE,			//�^�C�g��
	GAME_HELP,			//�w���v
	GAME_LEVEL,			//��Փx
	GAME_INIT,			//������
	GAME_MAIN,			//���C��
	GAME_WIN,			//�������
	GAME_LOSE,			//�s�k�҉��
	GAME_END,			//�G���h����
	GAME_INPUTRANKING,	//�����L���O������
	GAME_RANKING,		//�����L���O���
	END = 99			//�G���h
};

/*****      �Q�[�����[�h�񋓑�      *****/
//typedef enum BLOCK_NUMBER {
	

/*****      �摜�\����      *****/
typedef struct PICTURE {
	int Player[ 15 ];
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
	int MapNum;		//�}�b�v�i���o�[
	int DrawFlg;	//DrawGraph����Ƃ��ɂ������
	int BreakFlg;	//��ꂽ���肷����
	int CoX;		//�u���b�N��X���W
	int CoY;		//�u���b�N��Y���W
};
MAP map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ];

/*****		�v���C���[�\����		*****/
typedef struct PLAYER {
	int PlayerX;	//�v���C���[���WX
	int PlayerY;	//�v���C���[���WY
	float PSpeed;	//�v���C���[�X�s�[�h
	float P_FallSpeed;	//�v���C���[�����X�s�[�h
	int JumpFrame;	//�W�����v�t���[��
	int P_i_f;		//���s�t���[��
	int P_lr_f;		//���s�����ϐ�
	int MapScrollX;	//�}�b�v�X�N���[������̂ɕK�v�Ȃ��
	int Scroll;		//�}�b�v�X�N���[���̌��ݗ�
};
PLAYER Player = { ( ( 2 * _MASS_X ) + _MASS_HALF ), ( 11 * _MASS_Y + _MASS_HALF ), 0, 0, 0, 0, 0, 0 };

int Map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ] = 
	{	
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0,  0,  6,  6,  6,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  7,  0,  0,  0,  0,  0,  7,  2,  2,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  6,  2,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  7,  0,  0,  0,  0,  3,  0,  0,  3,  0,  0,  3,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  0,  0,  0,  0,  0,  0,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  4,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0, 18,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 77, 78,  0, 15, 16, 17,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0, 15, 16, 17,  0,  9,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0,  0, 18,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0, 15, 16, 17,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{ 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0, 22, 23, 23, 24,  0, 77, 78, 15, 16, 69, 16, 17,  0,  0,  0,  0, 77, 78, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 23, 23, 24,  0,  0,  0, 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9, 23, 23,  9,  9,  9,  9, 15, 16, 69, 16,  9,  9,  9,  9,  9,  0,  0,  9,  9,  9,  9, 24, 15, 16, 17, 77, 78,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  9,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0, 15, 16, 69, 16, 17,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 8},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 8}
	};																																																																																																																																						  

//�����蔻��̂���}�b�v�i���o�[
int HitBlockNum[ _HITCHIP_ALL ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 37, 38, 39, 40, 41, 42, 49, 50, 51, 52, 53, 54 };

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
void DrawEnd();		//�G���h��ʕ`��

//�Q�[�����C����ʕ`��n�֐�
void DrawStage();	//�X�e�[�W�`��
void DrawPlayer();	//�v���C���[�`��
void DrawBlock();	//�u���b�N�`��
void DrawItem();	//�A�C�e���`��

//�v���C���[�����n�֐�
//�����蔻��֐�( TRUE�Ȃ�q�b�g����AFALSE�Ȃ�q�b�g�Ȃ� )
int HitBlockUp( int MapX, int MapY );		//�u���b�N�㓖���蔻��
int HitBlockDown( int MapX, int MapY );		//�u���b�N�������蔻��
int HitBlockRight( int MapX, int MapY );	//�u���b�N�E�����蔻��
int HitBlockLeft( int MapX, int MapY );		//�u���b�N�������蔻��

//�Ǎ������֐�
int LoadImages();	//�摜�Ǎ�

/****************************************************/
/*****											*****/
/*****				  ���C������				*****/
/*****											*****/
/****************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

//�f�o�b�O���[�h���̃E�B���h�E�T�C�Y�ݒ�
#ifdef _DEBUGMODE
	const int _WINDOWSIZE_X = 700;
	const int _WINDOWSIZE_Y = 448;
#endif

//�f�o�b�O���[�h�Ȃ��̎��̃E�B���h�E�T�C�Y�ݒ�
#ifndef _DEBUGMODE
	const int _WINDOWSIZE_X = 512;
	const int _WINDOWSIZE_Y = 448;
#endif

	GAMESTATE = GAME_TITLE;
	SetMainWindowText( "Super Mairo Bros" );					//�E�B���h�E�e�L�X�g�ύX

	ChangeWindowMode( _WINDOWMODE );							//�E�B���h�E���[�h�ύX
	SetGraphMode( _WINDOWSIZE_X, _WINDOWSIZE_Y, 32 );			//�E�B���h�E�T�C�Y�ύX
	SetDrawScreen( DX_SCREEN_BACK );							//�`��X�N���[���ύX
	
	/********************     ���t���b�V�����[�g�m�F     *************************/
	hdc = GetDC( GetMainWindowHandle() ) ;			// �f�o�C�X�R���e�L�X�g�̎擾
	RefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;	// ���t���b�V�����[�g�̎擾
	ReleaseDC( GetMainWindowHandle(), hdc ) ;		// �f�o�C�X�R���e�L�X�g�̉��
	/*****************************************************************************/

	if ( DxLib_Init() == -1 )	return -1;		//Dx���C�u����������
	if ( LoadImages() == -1 )	return -1;		//�摜�Ǎ�����

	/*************************     ���C�����[�v����     **************************/
	while ( ProcessMessage() == 0 && ClearDrawScreen() == 0 && GAMESTATE != 99 ) {
	
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
			case GAME_END:
				DrawEnd();
				break;
		}
	
		FR_Update();
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
	int x;

	x = 320 - GetDrawStringWidth( "MARIO", 5 );
	DrawFormatString( x, 240, 0xff0000, "MARIO" );

	x = 320 - GetDrawStringWidth( "Push Space", 10 );
	DrawFormatString( x, 400, 0xffffff, "Push Space" );

	if ( opt.Kflg & PAD_INPUT_M ) {
		GAMESTATE = GAME_INIT;
	} else if ( opt.Kflg & PAD_INPUT_START ) {
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

	GAMESTATE = GAME_MAIN;

}

//�Q�[�����C������
void GameMain() {

	DrawStage();		//�X�e�[�W�`��
	DrawPlayer();		//�v���C���[�`��

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
}

//�X�e�[�W�`��
void DrawStage() {

	static int AnimationFrame = 0;

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

	//�}�b�v�`��
	/*for ( int StageY = 0; StageY < _MAP_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_X; StageX++ ) {
			DrawRotaGraph( ( ( StageX * _MASS_X ) + _MASS_HALF ), ( ( StageY * _MASS_Y ) + _MASS_HALF ), 
								1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
		}
	}*/

	//�}�b�v�`��
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			DrawRotaGraph( map[ StageY ][ StageX ].CoX + _MASS_HALF, map[ StageY ][ StageX ].CoY + _MASS_HALF,
								1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
			if ( map[ StageY ][ StageX ].MapNum == 2 || map[ StageY ][ StageX ].MapNum == 3 || 
					map[ StageY ][ StageX ].MapNum == 4 || map[ StageY ][ StageX ].MapNum == 5 ) {
				if ( AnimationFrame++ < 300 ) {
					map[ StageY ][ StageX ].MapNum = 3;
				} else if ( AnimationFrame < 400 ) {
					map[ StageY ][ StageX ].MapNum = 4;
				} else if ( AnimationFrame < 600 ) {
					map[ StageY ][ StageX ].MapNum = 5;
				} else if ( AnimationFrame < 800 ) {
					map[ StageY ][ StageX ].MapNum = 4;
				} else if ( AnimationFrame < 1000 ) {
					AnimationFrame=0;
			
		}
	}
}

	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			map[ StageY ][ StageX ].CoX -= Player.MapScrollX;
		}
	}

#ifdef _DEBUGMODE
	DrawCircle( ( Player.PlayerX / 32 ) * _MASS_X + _MASS_HALF, ( ( Player.PlayerY / 32 ) - 1 ) * _MASS_Y + _MASS_HALF, 2, 0xff00ff, TRUE );
#endif

	}
}
	

//�v���C���[�`��
void DrawPlayer() {

	int PDrawMode = 0;	//0=�����ĂȂ��A1 = �����Ă���(���E�L�[���͂������Ă�)
	static int JumpMode = 0;

	//�����A�j���[�V����
	if( ( 0 == FR_Control.FrameCount % 4 ) && opt.NowK != NULL ){
		Player.P_i_f++;
		if(Player.P_i_f==4)Player.P_i_f=0;
	}
	if( opt.NowK == NULL ){
		Player.P_i_f = 0;	//�L�[�������߂���
	}

	//���s����
	/*****     �E�ړ�����     *****/
	if ( Player.PlayerX <= ( 6 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {
		PDrawMode = 1;						//�����Ă邩���Ȃ����̏���
		//�����x�ݒ�
		if ( Player.PSpeed < 4.0f ) {
			Player.PSpeed += 0.2f;
		}
		Player.PlayerX += ( int )Player.PSpeed;	//�v���C���[�ړ�
		Player.P_lr_f=0;					//���E���]�t���O
		if ( Player.PlayerX >= 6 * _MASS_X + _MASS_HALF ) {
			Player.PlayerX -= ( int )Player.PSpeed;
			Player.MapScrollX = ( int )Player.PSpeed;
		}

		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		//���s���̃v���C���[�`��
	}
	/*****     ���ړ�����     *****/
	else if ( Player.PlayerX >= ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X )  ) {
		PDrawMode = 1;						//�����Ă邩���Ȃ����̏���
		if ( Player.PSpeed < 4.0f ) {
			Player.PSpeed -= 0.2f;			//�����x�ݒ�
		}
		Player.MapScrollX = 0;
		Player.PlayerX += ( int )Player.PSpeed;	//�v���C���[�ړ�
		
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//���s���̃v���C���[�`��
	} 
	else {		//�~�܂����Ƃ��̏���
		if ( Player.PSpeed >= 0.0f ) {
			Player.PSpeed -= 0.2f;
			Player.MapScrollX = ( int )Player.PSpeed;
			if ( Player.P_lr_f == 0 && ( Player.PlayerX < 5 * _MASS_X + _MASS_HALF ) ) { 
				Player.PlayerX += ( int )Player.PSpeed;
			} 
			else if ( Player.P_lr_f == 1 && ( Player.PlayerX > 0 * _MASS_X + _MASS_HALF ) ) {
				Player.PlayerX += ( int )Player.PSpeed;
			}
		}
		//Player.MapScrollX = 0;
	}

	//�d�͏���
	/*int Hit = 0;
	if ( JumpMode != 1 && ( HitBlockDown( Player.PlayerX, Player.PlayerY ) == FALSE ) ) {
		if ( Player.P_FallSpeed <= 4.0f ) {
			Player.P_FallSpeed += 0.4f;
		}
		Player.PlayerY += Player.P_FallSpeed;
	}*/
	

	//�W�����v����
	if( Player.JumpFrame == 0 && opt.Kflg & PAD_INPUT_M ) {
		Player.JumpFrame++;
		JumpMode = 1;
	}
	if ( Player.JumpFrame > 0 ) {
		if ( Player.JumpFrame < 64 ) {
			Player.JumpFrame += 1;
			Player.PlayerY -= 2;
		}
		if ( Player.JumpFrame >= 64 ) {
			Player.JumpFrame += 1;
			Player.PlayerY += 2;
		}

		if ( Player.JumpFrame == 127 ) {
			Player.JumpFrame = 0;

		}
	}

	//�����x�ݒ�
	//if ( opt.OldK != 0 ) {
	//	if ( Player.PSpeed <= 2.0f ) {
	//		Player.PSpeed += 0.1f;
	//	}
	//} 
	//if ( opt.OldK == 0 && Player.PSpeed >= 0.0f ) {
	//	if ( Player.PSpeed > 0.0f ) {
	//		Player.PSpeed -= 0.03f;
	//	}
	//}

#ifdef _DEBUGMODE
	DrawFormatString( 516,  50, 0xff0000, "OldK = %d", opt.OldK );		//OldK�`��
	DrawFormatString( 516,  80, 0xff0000, "NowK = %d", opt.NowK );		//NowK�`��
	DrawFormatString( 516, 110, 0xff0000, "PdrM = %d", PDrawMode );		//PDrawMode�`��
	DrawFormatString( 516, 140, 0xff0000, "LR_F = %d", Player.P_lr_f );	//P_lr_f�`��
	DrawFormatString( 516, 170, 0xff0000, "PlrX = %d", Player.PlayerX );//PlayerX�`��
	DrawFormatString( 516, 200, 0xff0000, "PlrY = %d", Player.PlayerY );	//PlayerY�`��
#endif

	//�����쎞�̃v���C���[�`��
	if ( PDrawMode == 0 )	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );

}

//�摜�Ǎ�
int LoadImages() {

	//�u���b�N�Ǎ�
	if ( LoadDivGraph( "images/backimage.png", 84, 12, 7, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//�L�����N�^�[�Ǎ�
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )	return -1;

	Pic.P_Walk[0]=Pic.Player[1];
	Pic.P_Walk[1]=Pic.Player[2];
	Pic.P_Walk[2]=Pic.Player[3];
	Pic.P_Walk[3]=Pic.Player[2];

	return TRUE;
}

//�}�b�v������
void MapInit() {
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			map[ StageY ][ StageX ].MapNum = Map[ StageY ][ StageX ];
			map[ StageY ][ StageX ].CoX = StageX * _MASS_X;
			map[ StageY ][ StageX ].CoY = StageY * _MASS_Y;
			map[ StageY ][ StageX ].DrawFlg = TRUE;
			map[ StageY ][ StageX ].BreakFlg = FALSE;
		}
	}
}

//�����蔻���u���b�N
int HitBlockUp( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( MapY / 32 ) - 1 ][ ( MapX / 32 ) ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}

//�����蔻�艺�u���b�N
int HitBlockDown( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( Player.PlayerY / 32 ) + 1 ][ ( Player.PlayerX / 32 ) ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}

//�����蔻��E�u���b�N
int HitBlockRight( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( Player.PlayerY / 32 ) ][ ( Player.PlayerX / 32 ) + 1 ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}

//�����蔻�荶�u���b�N
int HitBlockLeft( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( Player.PlayerY / 32 ) ][ ( Player.PlayerX / 32 ) - 1 ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}