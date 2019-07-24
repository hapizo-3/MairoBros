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

/*****		プレイヤー系定数		*****/
#define _PLAYER_MAXSPEED	3

//当たり判定のあるブロックの総数
#define _HITCHIP_ALL	21

//マップ全サイズ
#define _MAP_ALLSIZE_X 220
#define _MAP_ALLSIZE_Y 14

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
	int StageBlock[ 90 ];
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
	int MapNum;		//マップナンバー
	int DrawFlg;	//DrawGraphするときにつかうやつ
	int BreakFlg;	//壊れた判定するやつ
	int CoX;		//ブロックのX座標
	int CoY;		//ブロックのY座標
};
MAP map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ];

/*****		プレイヤー構造体		*****/
typedef struct PLAYER {
	int PlayerX;	//プレイヤー座標X
	int PlayerY;	//プレイヤー座標Y
	float PSpeed;	//プレイヤースピード
	float P_FallSpeed;	//プレイヤー落下スピード
	int JumpFrame;	//ジャンプフレーム
	int P_i_f;		//歩行フレーム
	int P_lr_f;		//歩行方向変数
	int MapScrollX;	//マップスクロールするのに必要なやつ
	int Scroll;		//マップスクロールの現在量
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

//当たり判定のあるマップナンバー
int HitBlockNum[ _HITCHIP_ALL ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 37, 38, 39, 40, 41, 42, 49, 50, 51, 52, 53, 54 };

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
void MapInit();		//マップ初期処理

//ゲームステート描画関数
void DrawTitle();	//タイトル描画
void DrawEnd();		//エンド画面描画

//ゲームメイン画面描画系関数
void DrawStage();	//ステージ描画
void DrawPlayer();	//プレイヤー描画
void DrawBlock();	//ブロック描画
void DrawItem();	//アイテム描画

//プレイヤー処理系関数
//当たり判定関数( TRUEならヒットあり、FALSEならヒットなし )
int HitBlockUp( int MapX, int MapY );		//ブロック上当たり判定
int HitBlockDown( int MapX, int MapY );		//ブロック下当たり判定
int HitBlockRight( int MapX, int MapY );	//ブロック右当たり判定
int HitBlockLeft( int MapX, int MapY );		//ブロック左当たり判定

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
	DrawFormatString( 570, 0, 0xff0000, "%.1f", FR_Control.mFps );

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

	MapInit();

	GAMESTATE = GAME_MAIN;

}

//ゲームメイン処理
void GameMain() {

	DrawStage();		//ステージ描画
	DrawPlayer();		//プレイヤー描画

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
}

//ステージ描画
void DrawStage() {

	static int AnimationFrame = 0;

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

	//マップ描画
	/*for ( int StageY = 0; StageY < _MAP_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_X; StageX++ ) {
			DrawRotaGraph( ( ( StageX * _MASS_X ) + _MASS_HALF ), ( ( StageY * _MASS_Y ) + _MASS_HALF ), 
								1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
		}
	}*/

	//マップ描画
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
	

//プレイヤー描画
void DrawPlayer() {

	int PDrawMode = 0;	//0=動いてない、1 = 動いている(左右キー入力が入ってる)
	static int JumpMode = 0;

	//歩くアニメーション
	if( ( 0 == FR_Control.FrameCount % 4 ) && opt.NowK != NULL ){
		Player.P_i_f++;
		if(Player.P_i_f==4)Player.P_i_f=0;
	}
	if( opt.NowK == NULL ){
		Player.P_i_f = 0;	//キー操作をやめた時
	}

	//歩行処理
	/*****     右移動処理     *****/
	if ( Player.PlayerX <= ( 6 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {
		PDrawMode = 1;						//動いてるかいないかの処理
		//加速度設定
		if ( Player.PSpeed < 4.0f ) {
			Player.PSpeed += 0.2f;
		}
		Player.PlayerX += ( int )Player.PSpeed;	//プレイヤー移動
		Player.P_lr_f=0;					//左右反転フラグ
		if ( Player.PlayerX >= 6 * _MASS_X + _MASS_HALF ) {
			Player.PlayerX -= ( int )Player.PSpeed;
			Player.MapScrollX = ( int )Player.PSpeed;
		}

		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		//歩行時のプレイヤー描画
	}
	/*****     左移動処理     *****/
	else if ( Player.PlayerX >= ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X )  ) {
		PDrawMode = 1;						//動いてるかいないかの処理
		if ( Player.PSpeed < 4.0f ) {
			Player.PSpeed -= 0.2f;			//加速度設定
		}
		Player.MapScrollX = 0;
		Player.PlayerX += ( int )Player.PSpeed;	//プレイヤー移動
		
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//歩行時のプレイヤー描画
	} 
	else {		//止まったときの処理
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

	//重力処理
	/*int Hit = 0;
	if ( JumpMode != 1 && ( HitBlockDown( Player.PlayerX, Player.PlayerY ) == FALSE ) ) {
		if ( Player.P_FallSpeed <= 4.0f ) {
			Player.P_FallSpeed += 0.4f;
		}
		Player.PlayerY += Player.P_FallSpeed;
	}*/
	

	//ジャンプ処理
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
	DrawFormatString( 516, 170, 0xff0000, "PlrX = %d", Player.PlayerX );//PlayerX描画
	DrawFormatString( 516, 200, 0xff0000, "PlrY = %d", Player.PlayerY );	//PlayerY描画
#endif

	//無動作時のプレイヤー描画
	if ( PDrawMode == 0 )	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );

}

//画像読込
int LoadImages() {

	//ブロック読込
	if ( LoadDivGraph( "images/backimage.png", 84, 12, 7, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//キャラクター読込
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )	return -1;

	Pic.P_Walk[0]=Pic.Player[1];
	Pic.P_Walk[1]=Pic.Player[2];
	Pic.P_Walk[2]=Pic.Player[3];
	Pic.P_Walk[3]=Pic.Player[2];

	return TRUE;
}

//マップ初期化
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

//当たり判定上ブロック
int HitBlockUp( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( MapY / 32 ) - 1 ][ ( MapX / 32 ) ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}

//当たり判定下ブロック
int HitBlockDown( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( Player.PlayerY / 32 ) + 1 ][ ( Player.PlayerX / 32 ) ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}

//当たり判定右ブロック
int HitBlockRight( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( Player.PlayerY / 32 ) ][ ( Player.PlayerX / 32 ) + 1 ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}

//当たり判定左ブロック
int HitBlockLeft( int MapX, int MapY ) {

	for ( int HitBlock = 0; HitBlock < _HITCHIP_ALL; HitBlock++ ) {
		if ( map[ ( Player.PlayerY / 32 ) ][ ( Player.PlayerX / 32 ) - 1 ].MapNum == HitBlockNum[ HitBlock ] ) {
			return TRUE;
		}
	}

	return FALSE;

}