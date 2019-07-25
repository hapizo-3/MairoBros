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
	int StageBlock[ 10 ];
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
	int DrawFlg;
	int BreakFlg;
};
MAP map;

/*****		�v���C���[�\����		*****/
typedef struct PLAYER {
	int PlayerX;	//�v���C���[���WX
	int PlayerY;	//�v���C���[���WY
	float PSpeed;	//�v���C���[�X�s�[�h
	int JumpFrame;	//�W�����v�t���[��
	int P_i_f;		//���s�t���[��
	int P_lr_f;		//���s�����ϐ�
	int JF_f;
};
PLAYER Player = { ( ( 2 * _MASS_X ) + _MASS_HALF ), ( 11 * _MASS_Y + _MASS_HALF ), 0, 0 };

int Map[ _MAP_Y ][ _MAP_X ] = 
	{	{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  6,  2,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 },
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 }		};

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

//�Q�[���X�e�[�g�`��֐�
void DrawTitle();	//�^�C�g���`��
void DrawEnd();		//�G���h��ʕ`��

//�Q�[�����C����ʕ`��n�֐�
void DrawStage();	//�X�e�[�W�`��
void DrawPlayer();	//�v���C���[�`��
void DrawBlock();	//�u���b�N�`��
void DrawItem();	//�A�C�e���`��

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

	GAMESTATE = GAME_MAIN;

}

//�Q�[�����C������
void GameMain() {

	DrawFormatString( 0, 0, 0xffffff, "MAIN" );

	DrawStage();		//�X�e�[�W�`��
	DrawPlayer();		//�v���C���[�`��

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
}

//�X�e�[�W�`��
void DrawStage() {

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

	
	for ( int StageY = 0; StageY < _MAP_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_X; StageX++ ) {
			DrawRotaGraph( ( ( StageX * _MASS_X ) + _MASS_HALF ), ( ( StageY * _MASS_Y ) + _MASS_HALF ), 1.0f, 0, Pic.StageBlock[ Map[ StageY ][ StageX ] ], TRUE );
		}
	}

}

void DrawPlayer() {

	int PDrawMode = 0;

	//�����A�j���[�V����
	if( ( 0 == FR_Control.FrameCount % 4 ) && opt.NowK != NULL ){
		if(Player.JumpFrame==0)Player.P_i_f++;
		if(Player.P_i_f>=3 &&Player.JumpFrame==0)Player.P_i_f=0;
		if(Player.JumpFrame>0)Player.P_i_f=3;
	}
	if( opt.NowK == NULL ){
		Player.P_i_f = 0;	//�L�[�������߂���
	}

	//���s����
	//�E
	if ( Player.PlayerX <= ( 15 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {
		PDrawMode = 1;
		if(Map[Player.PlayerY/32][((Player.PlayerX-16)/32)+1]==0)Player.PlayerX += ( 3 + Player.PSpeed );
		if(Player.JumpFrame==0)Player.P_lr_f=0;
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, Player.P_lr_f );		//���s���̃v���C���[�`��
		
	}//��
	else if ( Player.PlayerX >= ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X ) ) {
		PDrawMode = 1;
		if(Map[Player.PlayerY/32][((Player.PlayerX+16)/32)-1]==0)Player.PlayerX -= ( 3 + Player.PSpeed );
		if(Player.JumpFrame==0)Player.P_lr_f=1;
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, Player.P_lr_f );	//���s���̃v���C���[�`��
	}

	//�d�͏���


	//�W�����v����
	if( Player.JumpFrame == 0 && opt.Kflg & PAD_INPUT_M ) {
		Player.JumpFrame++;
	}
	if ( Player.JumpFrame > 0 ) {
		if ( Player.JumpFrame < 12 )Player.JF_f=1;
		else if ( Player.JumpFrame < 18 )Player.JF_f=2;
		else if ( Player.JumpFrame < 24 )Player.JF_f=3;
		else if ( Player.JumpFrame < 30 )Player.JF_f=4;
		else if ( Player.JumpFrame < 36 )Player.JF_f=5;
		else if ( Player.JumpFrame < 47 )Player.JF_f=6;
		if ( Player.JumpFrame == 47 ) {
			Player.JumpFrame = 0;
		}
		switch (Player.JF_f)
		{
		case 1:
			Player.JumpFrame += 1;
			Player.PlayerY -= 9;
			if(Map[((Player.PlayerY+16)/32)-1][(Player.PlayerX/32)]!=0)Player.JumpFrame=36;
			break;
		case 2:
			Player.JumpFrame += 1;
			Player.PlayerY -= 5;
			if(Map[(Player.PlayerY+16)/32-1][(Player.PlayerX/32)]!=0)Player.JumpFrame=30;
			break;
		case 3:
			Player.JumpFrame += 1;
			Player.PlayerY -= 3;
			if(Map[(Player.PlayerY+16)/32-1][(Player.PlayerX/32)]!=0)Player.JumpFrame=24;
			break;
		case 4:
			Player.JumpFrame += 1;
			Player.PlayerY += 3;
			if(Map[((Player.PlayerY-16)/32)+1][(Player.PlayerX/32)]!=0)Player.JumpFrame=0;
			break;
		case 5:
			Player.JumpFrame += 1;
			Player.PlayerY += 5;
			if(Map[((Player.PlayerY-16)/32)+1][(Player.PlayerX/32)]!=0)Player.JumpFrame=0;
			break;
		case 6:
			Player.JumpFrame += 1;
			Player.PlayerY += 9;
			if(Map[((Player.PlayerY-16)/32)+1][(Player.PlayerX/32)]!=0)Player.JumpFrame=0;
			if(Map[((Player.PlayerY-16)/32)+1][(Player.PlayerX/32)]==0)Player.JumpFrame=36;
			break;
		default:
			break;
		}
	}
	if(0!=Player.PlayerY%16 && Player.JumpFrame==0)Player.PlayerY=(Player.PlayerY/16)*16;
	if(Map[((Player.PlayerY-16)/32)+1][(Player.PlayerX/32)]==0 && Player.JumpFrame==0)Player.JumpFrame=24;
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
	DrawFormatString( 516, 170, 0xff0000, "P_i_f = %d", Player.P_i_f );	//P_lr_f�`��
	DrawFormatString( 516, 200, 0xff0000, "PlayerX = %d", Player.PlayerX );	//P_lr_f�`��
	DrawFormatString( 516, 230, 0xff0000, "PlayerY = %d", Player.PlayerY );	//P_lr_f�`��
	DrawCircle(Player.PlayerX,Player.PlayerY,3,0x0000ff);


#endif

	//�����쎞�̃v���C���[�`��
	if ( PDrawMode == 0  && Player.JumpFrame==0)	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );
	if ( PDrawMode == 0  && Player.JumpFrame!=0)	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );

}

//�摜�Ǎ�
int LoadImages() {

	//�u���b�N�Ǎ�
	if ( LoadDivGraph( "images/Block.png", 9, 9, 1, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//�L�����N�^�[�Ǎ�
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )	return -1;

	Pic.P_Walk[0]=Pic.Player[1];
	Pic.P_Walk[1]=Pic.Player[2];
	Pic.P_Walk[2]=Pic.Player[3];
	Pic.P_Walk[3]=Pic.Player[4];

	return TRUE;
}