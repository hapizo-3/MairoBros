#include "DxLib.h"

//デバッグモード
#define _DEBUGMODE

//ウィンドウモード変更
//(基本はウィンドウモード)
//0が全画面モード、1がウィンドウモード
#define _WINDOWMODE	1

//フレームレート定数( 60 )
#define	_FRAMERATE_60	60

/*****      フォントサイズ定数      *****/
#define _FONTSIZE_SS		10	//フォントSSサイズ
#define _FONTSIZE_S			20	//フォントSサイズ
#define _FONTSIZE_M			32	//フォントMサイズ
#define _FONTSIZE_L			48	//フォントLサイズ
#define _FONTSIZE_LL		64	//フォントLLサイズ

/*****		マスピクセル数		*****/
#define _MASS_X		32
#define _MASS_Y		32
#define _MASS_HALF	16

/*****		マップの高さ		*****/
#define _MAP_X		16
#define _MAP_Y		14

//ゲーム状態変数
static int GAMESTATE;

//リフレッシュレート確認用変数
static int RefreshRate;
HDC hdc;

/*****      ゲームモード列挙体      *****/
typedef enum GAME_MODE {
	GAME_TITLE,			//タイトル
	GAME_HELP,			//ヘルプ
	GAME_LEVEL,			//難易度
	GAME_INIT,			//初期化
	GAME_MAIN,			//メイン
	GAME_WIN,			//勝利画面
	GAME_LOSE,			//敗北者画面
	GAME_END,			//エンド処理
	GAME_INPUTRANKING,	//ランキング入り画面
	GAME_RANKING,		//ランキング画面
	END = 99			//エンド
};

/*****      ゲームモード列挙体      *****/
//typedef enum BLOCK_NUMBER {
	

/*****      画像構造体      *****/
typedef struct PICTURE {
	int Player[ 15 ];
	int StageBlock[ 10 ];
	int P_Walk[ 4 ];
};
PICTURE Pic;	//画像構造体宣言

/*****      サウンド構造体      *****/
typedef struct SOUND {
	int Sound;
};
SOUND Sound;		//音楽構造体宣言

/*****      フレームレート構造体      *****/
typedef struct FRAMERATE_CONTROL {
	int mStartTime;
	int mCount;
	int FrameCount;
	float mFps;
	int ReFreshRateState;
};
FRAMERATE_CONTROL FR_Control = { 0, 0, 0.0, 0 };	//フレームレート制御構造体宣言

/*****      キー操作構造体      *****/
typedef struct OPERATE {
	//KeyInput
	int OldK;
	int NowK;
	int Kflg;
} OPERATE;

OPERATE opt;

/*****		  マップ構造体		*****/
typedef struct MAP {
	int DrawFlg;
	int BreakFlg;
};
MAP map;

/*****		プレイヤー構造体		*****/
typedef struct PLAYER {
	int PlayerX;	//プレイヤー座標X
	int PlayerY;	//プレイヤー座標Y
	float PSpeed;	//プレイヤースピード
	int JumpFrame;	//ジャンプフレーム
	int P_i_f;		//歩行フレーム
	int P_lr_f;		//歩行方向変数
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
/*****			  関数プロトタイプ宣言			*****/
/*****											*****/
/****************************************************/

//フレームレート制御関数
static bool FR_Update( );	//フレームレート測定関数
static void FR_Draw( );		//フレームレート描画関数
static void FR_Wait( );		//待機処理関数

//ゲームメイン処理系関数
void GameInit();	//初期処理
void GameMain();	//メイン処理

//ゲームステート描画関数
void DrawTitle();	//タイトル描画
void DrawEnd();		//エンド画面描画

//ゲームメイン画面描画系関数
void DrawStage();	//ステージ描画
void DrawPlayer();	//プレイヤー描画
void DrawBlock();	//ブロック描画
void DrawItem();	//アイテム描画

//読込処理関数
int LoadImages();	//画像読込

/****************************************************/
/*****											*****/
/*****				  メイン処理				*****/
/*****											*****/
/****************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

//デバッグモード時のウィンドウサイズ設定
#ifdef _DEBUGMODE
	const int _WINDOWSIZE_X = 700;
	const int _WINDOWSIZE_Y = 448;
#endif

//デバッグモードなしの時のウィンドウサイズ設定
#ifndef _DEBUGMODE
	const int _WINDOWSIZE_X = 512;
	const int _WINDOWSIZE_Y = 448;
#endif

	GAMESTATE = GAME_TITLE;
	SetMainWindowText( "Super Mairo Bros" );					//ウィンドウテキスト変更

	ChangeWindowMode( _WINDOWMODE );							//ウィンドウモード変更
	SetGraphMode( _WINDOWSIZE_X, _WINDOWSIZE_Y, 32 );			//ウィンドウサイズ変更
	SetDrawScreen( DX_SCREEN_BACK );							//描画スクリーン変更
	
	/********************     リフレッシュレート確認     *************************/
	hdc = GetDC( GetMainWindowHandle() ) ;			// デバイスコンテキストの取得
	RefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;	// リフレッシュレートの取得
	ReleaseDC( GetMainWindowHandle(), hdc ) ;		// デバイスコンテキストの解放
	/*****************************************************************************/

	if ( DxLib_Init() == -1 )	return -1;		//Dxライブラリ初期化
	if ( LoadImages() == -1 )	return -1;		//画像読込処理

	/*************************     メインループ処理     **************************/
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

/******　　　　　　　フレーム数制御関数         ******/
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

/******　　　　　　　フレームレート描画         ******/
static void FR_Draw( ) {
	
	SetFontSize( _FONTSIZE_S );
	DrawFormatString( 516, 0, 0xff0000, "%d", FR_Control.FrameCount );
	DrawFormatString( 516, 20, 0xff0000, "%d", RefreshRate );

}

/******　　　　　　　 処理待ち関数          ******/
static void FR_Wait( ) {
	int tookTime = GetNowCount() - FR_Control.mStartTime;
	int waitTime = FR_Control.mCount * 1000 / _FRAMERATE_60 - tookTime;

	if ( waitTime > 0 ) {
		Sleep( waitTime );
	}
}

//タイトル描画
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

//エンド画面
void DrawEnd() {
	GAMESTATE = END;
}

//メイン初期処理
void GameInit() {

	GAMESTATE = GAME_MAIN;

}

//ゲームメイン処理
void GameMain() {

	DrawFormatString( 0, 0, 0xffffff, "MAIN" );

	DrawStage();		//ステージ描画
	DrawPlayer();		//プレイヤー描画

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
}

//ステージ描画
void DrawStage() {

	//背景描画
	DrawBox( 0, 0, 512, 448, 0x5080f8, TRUE );
	
	//ライン描画
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

	//歩くアニメーション
	if( ( 0 == FR_Control.FrameCount % 4 ) && opt.NowK != NULL ){
		if(Player.JumpFrame==0)Player.P_i_f++;
		if(Player.P_i_f>=3 &&Player.JumpFrame==0)Player.P_i_f=0;
		if(Player.JumpFrame>0)Player.P_i_f=3;
	}
	if( opt.NowK == NULL ){
		Player.P_i_f = 0;	//キー操作をやめた時
	}

	//歩行処理
	//右
	if ( Player.PlayerX <= ( 15 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {
		PDrawMode = 1;
		if(Map[Player.PlayerY/32][((Player.PlayerX-16)/32)+1]==0)Player.PlayerX += ( 3 + Player.PSpeed );
		if(Player.JumpFrame==0)Player.P_lr_f=0;
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, Player.P_lr_f );		//歩行時のプレイヤー描画
		
	}//左
	else if ( Player.PlayerX >= ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X ) ) {
		PDrawMode = 1;
		if(Map[Player.PlayerY/32][((Player.PlayerX+16)/32)-1]==0)Player.PlayerX -= ( 3 + Player.PSpeed );
		if(Player.JumpFrame==0)Player.P_lr_f=1;
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, Player.P_lr_f );	//歩行時のプレイヤー描画
	}

	//重力処理


	//ジャンプ処理
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
	//加速度設定
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
	DrawFormatString( 516,  50, 0xff0000, "OldK = %d", opt.OldK );		//OldK描画
	DrawFormatString( 516,  80, 0xff0000, "NowK = %d", opt.NowK );		//NowK描画
	DrawFormatString( 516, 110, 0xff0000, "PdrM = %d", PDrawMode );		//PDrawMode描画
	DrawFormatString( 516, 140, 0xff0000, "LR_F = %d", Player.P_lr_f );	//P_lr_f描画
	DrawFormatString( 516, 170, 0xff0000, "P_i_f = %d", Player.P_i_f );	//P_lr_f描画
	DrawFormatString( 516, 200, 0xff0000, "PlayerX = %d", Player.PlayerX );	//P_lr_f描画
	DrawFormatString( 516, 230, 0xff0000, "PlayerY = %d", Player.PlayerY );	//P_lr_f描画
	DrawCircle(Player.PlayerX,Player.PlayerY,3,0x0000ff);


#endif

	//無動作時のプレイヤー描画
	if ( PDrawMode == 0  && Player.JumpFrame==0)	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );
	if ( PDrawMode == 0  && Player.JumpFrame!=0)	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );

}

//画像読込
int LoadImages() {

	//ブロック読込
	if ( LoadDivGraph( "images/Block.png", 9, 9, 1, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//キャラクター読込
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )	return -1;

	Pic.P_Walk[0]=Pic.Player[1];
	Pic.P_Walk[1]=Pic.Player[2];
	Pic.P_Walk[2]=Pic.Player[3];
	Pic.P_Walk[3]=Pic.Player[4];

	return TRUE;
}