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

//マップ全サイズ
#define _MAP_ALLSIZE_X 220
#define _MAP_ALLSIZE_Y 14

//当たり判定のあるブロックの数
#define _HITBLOCK	21

//当たり判定制御定数
#define _HIT_FALSE	0
#define _HIT_TRUE	1

//方向の定数
#define _DIRECT_RIGHT	0
#define _DIRECT_LEFT	1

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
	int PlayerX;		//プレイヤー座標X
	int PlayerY;		//プレイヤー座標Y
	float PSpeed;		//プレイヤースピード
	int JumpFrame;		//ジャンプフレーム
	int JumpTime;		//スペースキーの押された時間
	int JumpMode;		//ジャンプ状態変数
	int PJSpeed;		//ジャンプスピード
	int P_i_f;			//歩行フレーム
	int P_lr_f;			//歩行方向変数
	int JF_f;
	int MapScrollX;		//マップスクロールするのに必要なやつ
	int Scroll;			//マップスクロールの現在量
	float PYSpeed;	//落下スピード
	int PDirectMode;	//方向で処理帰るときの変数
};
//first 3,11
PLAYER Player = { ( ( 2 * _MASS_X ) + _MASS_HALF ), ( 6 * _MASS_Y + _MASS_HALF ), 0, 0 };

int Map[ _MAP_ALLSIZE_Y ][ _MAP_ALLSIZE_X ] = 
	{	
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0, 19, 20, 20, 20, 21,  0,  0,  0,  0, 31, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 19, 20, 21,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 32, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0, 31, 32, 33,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  6,  6,  6,  6,  6,  6,  0,  0,  0,  6,  6,  6,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  7,  0,  0,  0,  0,  0,  7,  2,  2,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0,  6,  0,  6,  0,  6,  0,  6,  0,  6,  0,  6,  0,  0,  0,  2,  0,  0,  0,  6,  2,  6,  2,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  7,  0,  0,  0,  0,  3,  0,  0,  3,  0,  0,  3,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6,  0,  0,  0,  0,  0,  0,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  4,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0, 18,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0,  0, 18,  0,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 77, 78,  0, 15, 16, 17,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0, 15, 16, 17,  0,  9,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0,  0, 18,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0, 15, 16, 17,  0,  0, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{ 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  2, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0, 22, 23, 23, 24,  0, 77, 78, 15, 16, 69, 16, 17,  0,  0,  0,  0, 77, 78, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 23, 23, 24,  0,  0,  0, 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9, 23, 23,  9,  9,  9,  9, 15, 16, 69, 16,  9,  9,  9,  9,  9,  0,  0,  9,  9,  9,  9, 24, 15, 16, 17, 77, 78,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  9,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0, 15, 16, 69, 16, 17,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 8},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 8}
	};

//当たり判定のあるマップナンバー
int HitBlockNum[ _HITBLOCK ] = { 1, 2, 3, 4, 5, 6, 7 , 8, 9, 37, 38, 39, 40, 41, 42, 49, 50, 51, 52, 53, 54 };

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

//マリオ処理系関数
// int ComX, int ComY 
int HitBlockUp();		//ブロック上当たり判定
int HitBlockDown();		//ブロック下当たり判定
int HitBlockRight();	//ブロック右当たり判定
int HitBlockLeft();		//ブロック左当たり判定

//読込処理関数
int LoadImages();	//画像読込

/****************************************************/
/*****											*****/
/*****				  メイン処理				*****/
/*****											*****/
/****************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	const int _WINDOWSIZE_X = 768;
	const int _WINDOWSIZE_Y = 672;

	GAMESTATE = GAME_TITLE;
	SetMainWindowText( "Super Mairo Bros" );					//ウィンドウテキスト変更
	SetGraphMode( _WINDOWSIZE_X, _WINDOWSIZE_Y, 32 );			//ウィンドウサイズ変更

	ChangeWindowMode( _WINDOWMODE );							//ウィンドウモード変更
	if ( DxLib_Init() == -1 )	return -1;		//Dxライブラリ初期化

	int offscreen_handle = MakeScreen( 512, 448, FALSE );
	SetDrawScreen( offscreen_handle );							//描画スクリーン変更
	/********************     リフレッシュレート確認     *************************/
	hdc = GetDC( GetMainWindowHandle() ) ;			// デバイスコンテキストの取得
	RefreshRate = GetDeviceCaps( hdc, VREFRESH ) ;	// リフレッシュレートの取得
	ReleaseDC( GetMainWindowHandle(), hdc ) ;		// デバイスコンテキストの解放
	/*****************************************************************************/

	if ( LoadImages() == -1 )	return -1;		//画像読込処理

	/*************************     メインループ処理     **************************/
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

//エンド画面
void DrawEnd() {
	GAMESTATE = END;
}

//メイン初期処理
void GameInit() {

	MapInit();
	Player.PYSpeed = 0.0f;
	Player.PSpeed = 0.0f;
	Player.PJSpeed = 0.0f;
	Player.JumpFrame = 0;
	Player.JumpMode = FALSE;

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
		}
	}

	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			map[ StageY ][ StageX ].CoX -= Player.MapScrollX;
		}
	}
	Player.Scroll=-map[0][0].CoX;

}


//プレイヤー描画
void DrawPlayer() {

	static int PDrawMode = 0;	//0=動いてない、1=右動いている、 2=左動いてる
	static int SlideMode = 0;	//1=右滑り、2=左滑り
	static int JumpState = 1;	//ジャンプの状態を持っておく
	int InMode = 0;
	int InMode2 = 0;

	if ( ( ( opt.NowK & PAD_INPUT_M ) == 0 ) && JumpState == 1 ) {
		JumpState = 0;
	}

	//歩くアニメーション
	if( ( 0 == FR_Control.FrameCount % 4 ) && opt.NowK != NULL ){
		if ( Player.JumpFrame == 0 )	Player.P_i_f++;
		if(Player.P_i_f >= 3 && Player.JumpFrame == 0 )	Player.P_i_f=0;
		if ( Player.JumpFrame > 0 )	Player.P_i_f = 3;
	}
	if( opt.NowK == NULL ){
		Player.P_i_f = 0;	//キー操作をやめた時
	}

	//歩行処理
	/*****     右移動処理     *****/
	if ( Player.PlayerX <= ( 15 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {

		/** 通常処理 **/
		if ( PDrawMode == 0 || ( PDrawMode == 2 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 1;
		}
		if ( PDrawMode == 1 ) {
			if ( HitBlockRight() == _HIT_FALSE ) {
				if ( Player.PSpeed < 6.0f ) {
					Player.PSpeed += 0.2f;				//加速度設定
				}
				Player.PlayerX += Player.PSpeed;
			} else if ( HitBlockRight() == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
			}

			//スクロールシステム
			//if ( Player.PlayerX >= 6  * _MASS_X + _MASS_HALF ) {
			//	Player.PlayerX -= Player.PSpeed;
			//	Player.MapScrollX = Player.PSpeed;
			//}

			Player.P_lr_f = _DIRECT_RIGHT;					//左右反転フラグ
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		//歩行時のプレイヤー描画
		}
		/** 急ブレーキの処理 **/
		if ( PDrawMode == 2 && Player.PSpeed > 0.0f ) {
			Player.P_lr_f=0;					//左右反転フラグ
			SlideMode = 1;
			if ( HitBlockLeft() == _HIT_FALSE ) {
				Player.PSpeed -= 0.4f;
				if ( ( Player.PlayerX -= Player.PSpeed ) < ( 0 * _MASS_X + _MASS_HALF ) ) {
					Player.PlayerX += Player.PSpeed;
					Player.PSpeed = 0.0f;
				}
			} else if ( HitBlockLeft() == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
			}
			if ( Player.PSpeed <= 0.0f ) {
				Player.PSpeed = 0.0f;
				PDrawMode = 1;
			}
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, FALSE );
		}
	}
	/*****     左移動処理     *****/
	else if ( Player.PlayerX > ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X ) ) {

		/** 通常処理 **/
		if ( PDrawMode == 0 || ( PDrawMode == 1 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 2;
		}
		if ( PDrawMode == 2 ) {
			InMode = 2;
			if ( HitBlockLeft() == _HIT_FALSE ) {
				if ( Player.PSpeed < 6.0f ) {
					Player.PSpeed += 0.2f;			//加速度設定
				}
				Player.PlayerX -= Player.PSpeed;
			} else if ( HitBlockLeft() == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
			}
			Player.P_lr_f = _DIRECT_LEFT;					//左右反転フラグ
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//歩行時のプレイヤー描画
		}
		/** 急ブレーキの処理 **/
		if ( PDrawMode == 1 && Player.PSpeed > 0.0f /*PDrawMode == 4*/ ) {
			SlideMode = 2;
			InMode2 = 2;
			Player.P_lr_f=1;					//左右反転フラグ
			if ( HitBlockRight() == _HIT_FALSE ) {
				Player.PSpeed -= 0.4f;
				Player.PlayerX += Player.PSpeed;
			} else if ( HitBlockRight() == _HIT_TRUE ) {
				Player.PSpeed = 0.0f;
			}
			if ( Player.PSpeed <= 0.0f )	{
				Player.PSpeed = 0.0f;
				PDrawMode = 2;
			}
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, TRUE );
		}
	}
	/*****  その他処理   *****/
	else {
		if ( Player.PSpeed > 0.0f ) {
			Player.PSpeed -= 0.2f;
			//if ( Player.PSpeed <= 0.0f && SlideMode == 2 )	Player.P_lr_f = _DIRECT_LEFT;
			//if ( Player.PSpeed <= 0.0f && SlideMode == 1 )	Player.P_lr_f = _DIRECT_RIGHT;
			/*****  マリオ滑り処理   *****/
			if ( Player.PSpeed > 0.0f ) {
				//右処理
				if ( /*Player.P_lr_f == 0*/ PDrawMode == 1 && ( Player.PlayerX < 15 * _MASS_X + _MASS_HALF ) ) { 
					if ( HitBlockRight() == _HIT_FALSE ) {
						Player.PlayerX += Player.PSpeed;
					} else if ( HitBlockRight() == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
					}
					InMode2 = 3;
				} 
				//左処理
				else if ( /*Player.P_lr_f == 1*/ PDrawMode == 2 && ( Player.PlayerX > 0 * _MASS_X + _MASS_HALF ) ) {
					if ( HitBlockLeft() == _HIT_FALSE ) {
						Player.PlayerX -= Player.PSpeed;
					} else if ( HitBlockLeft() == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
					}
				}
			}
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );
		}
		if ( Player.PSpeed <= 0.0f ) {
			PDrawMode = 0;
			Player.PSpeed = 0.0f;
		}
		InMode = 1;
		Player.MapScrollX = 0;
	}

	/*****     ジャンプ処理     *****/
	if ( JumpState == 0 && Player.JumpMode == FALSE && ( opt.NowK & PAD_INPUT_M ) ) {
		Player.JumpMode = TRUE;
		Player.JumpFrame = 0;
		Player.PJSpeed = 7.0f;
		JumpState = 2;
	}
	
	if ( Player.JumpMode == TRUE ) {

		//上方向のブロックの当たり判定
		if ( HitBlockUp() == _HIT_TRUE ) {			//ヒットする場合
			Player.PJSpeed = 0.0f;
			Player.JumpMode = FALSE;
			Player.JumpFrame = 0;
		} else if ( HitBlockUp() == _HIT_FALSE ) {	//ヒットしない場合
			if ( Player.JumpFrame++ < 6 ) {
				if ( ( opt.NowK & PAD_INPUT_M ) == 0 ) {
					JumpState = 3;
				}
			} else if ( Player.JumpFrame >= 6 && Player.JumpFrame < 20 ) {
				if ( opt.NowK & PAD_INPUT_M ) {
					if ( JumpState != 3 )	Player.PJSpeed =  6.0f;
				} else if ( ( opt.NowK & PAD_INPUT_M ) == 0 ) { 
					if ( Player.PJSpeed > 0.0f ) {
						Player.PJSpeed -= 0.3f;
					} else if ( Player.PJSpeed <= 0.0f ) {
						Player.PJSpeed = 0.0f;
						Player.JumpMode = FALSE;
						Player.JumpFrame = 0;
					}
					JumpState = 3;
				}
			} else if ( Player.JumpFrame >= 20 ) {
				if ( Player.PJSpeed > 0.0f ) {
					Player.PJSpeed -= 0.4f;
				} else if ( Player.PJSpeed <= 0.0f ) {
					Player.PJSpeed = 0.0f;
					Player.JumpMode = FALSE;
					Player.JumpFrame = 0;
				}
			} 
			Player.PlayerY -= Player.PJSpeed;
			//else {
			//	if ( Player.PJSpeed > 0.0f ) {
			//		Player.PJSpeed -= 0.3f;
			//	}
			//	Player.PlayerY -= Player.PJSpeed;
			//	if ( Player.PJSpeed <= 0.0f ) {
			//		Player.PJSpeed = 0.0f;
			//		Player.JumpMode = FALSE;
			//		Player.JumpFrame = 0;
			//	}
			//}
		}

	} else if ( JumpState == 2 && Player.JumpMode == FALSE ) {
		JumpState = 3;
	}

	//if ( Player.JumpMode == FALSE && ( opt.NowK & PAD_INPUT_M ) ) {
	//	Player.JumpMode = TRUE;
	//}

	//if ( Player.JumpFrame < 16 && ( opt.NowK & PAD_INPUT_M ) ) {
	//	Player.JumpFrame++;
	//	Player.PlayerY -= 6.0f;
	//	if ( Player.JumpFrame >= 16 ) {
	//		Player.JumpMode = FALSE;
	//	}
	//}

	/*****     落下処理     *****/
	if ( Player.JumpMode == FALSE && HitBlockDown() == _HIT_FALSE ) {
		if ( Player.PYSpeed < 6.0f )	Player.PYSpeed += 0.6f;
		Player.PlayerY += ( int )Player.PYSpeed;
		//めり込み防止処理
		if ( HitBlockDown() == _HIT_TRUE ) {
			int Difference = 0;
			Difference = ( Player.PlayerY + _MASS_HALF ) - map[ ( Player.PlayerY / 32 ) + 1 ][ Player.PlayerX/32 ].CoY;
			Player.PlayerY -= Difference;
			Player.PYSpeed = 0.0f;
		}
	} 
	else if (Player.JumpMode == FALSE && HitBlockDown() == _HIT_TRUE ) {
		if ( JumpState == 3 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
			JumpState = 0;
		}
	}
	else {
		Player.PYSpeed = 0.0f;
	}
	
	
	//ジャンプ処理
	//if( Player.JumpFrame == 0 && opt.Kflg & PAD_INPUT_M ) {
	//	Player.JumpFrame++;
	//}
	//if(opt.NowK==PAD_INPUT_M){
	//	Player.JumpTime++;
	//}
	//if ( Player.JumpFrame > 0 ) {
	//	if ( Player.JumpFrame < 12 )	Player.JF_f = 1;
	//	else if ( Player.JumpFrame < 18 )	Player.JF_f = 2;
	//	else if ( Player.JumpFrame < 24 )	Player.JF_f = 3;
	//	else if ( Player.JumpFrame < 30 )	Player.JF_f = 4;
	//	else if ( Player.JumpFrame < 36 )	Player.JF_f = 5;
	//	else if ( Player.JumpFrame < 47 )	Player.JF_f = 6;
	//	if ( Player.JumpFrame == 47 ) {
	//		Player.JumpFrame = 0;
	//	}

	//	switch ( Player.JF_f )
	//	{
	//		case 1:
	//			Player.JumpFrame += 1;
	//			Player.PlayerY -= 9;
	//			if ( map[ ( Player.PlayerY / 32 ) - 1 ][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum != 0 )	Player.JumpFrame = 36;
	//			break;
	//		case 2:
	//			Player.JumpFrame += 1;
	//			Player.PlayerY -= 5;
	//			if ( map[ ( Player.PlayerY / 32 ) - 1 ][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum != 0 )	Player.JumpFrame = 30;
	//			break;
	//		case 3:
	//			Player.JumpFrame += 1;
	//			Player.PlayerY -= 3;
	//			if ( map[ ( Player.PlayerY / 32 ) - 1 ][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum != 0 )	Player.JumpFrame = 24;
	//			break;
	//		case 4:
	//			Player.JumpFrame += 1;
	//			Player.PlayerY += 3;
	//			if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1 ][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum != 0 )	Player.JumpFrame=0;
	//			break;
	//		case 5:
	//			Player.JumpFrame += 1;
	//			Player.PlayerY += 5;
	//			if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1 ][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum != 0 )	Player.JumpFrame=0;
	//			break;
	//		case 6:
	//			Player.JumpFrame += 1;
	//			Player.PlayerY += 9;
	//			if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum != 0 )	Player.JumpFrame=0;
	//			if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1][ ( Player.PlayerX+Player.Scroll / 32 ) ].MapNum == 0 )	Player.JumpFrame=36;
	//			break;
	//		default:
	//			break;
	//	}
	//}
	//if ( 0 != Player.PlayerY % 16 && Player.JumpFrame == 0 )	Player.PlayerY = ( Player.PlayerY / 16 ) * 16;
	//if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1 ][ ( Player.PlayerX / 32 ) ].MapNum == 0 && Player.JumpFrame == 0 )	Player.JumpFrame = 24;

	//無動作時のプレイヤー描画
	//if ( ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpFrame == 0 )	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );
	//if ( ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpFrame != 0 )	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );

	if ( ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 /*JumpMode == 0*/ )	
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );
	if ( ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 /*JumpMode != 0*/ )	
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );

#ifdef _DEBUGMODE
	SetFontSize( 12 );
	DrawFormatString( 420,  50, 0xff0000, "OldK = %d", opt.OldK );			//OldK描画
	DrawFormatString( 420,  70, 0xff0000, "NowK = %d", opt.NowK );			//NowK描画
	DrawFormatString( 420,  90, 0xff0000, "PdrM = %d", PDrawMode );			//PDrawMode描画
	DrawFormatString( 420, 110, 0xff0000, "LR_F = %d", Player.P_lr_f );		//P_lr_f描画
	DrawFormatString( 420, 130, 0xff0000, "PlrX = %d", Player.PlayerX );	//PlayerX描画
	DrawFormatString( 420, 150, 0xff0000, "PlrY = %d", Player.PlayerY );	//PlayerY描画
	DrawFormatString( 420, 170, 0xff0000, "Scrl = %d", Player.Scroll );		//Scroll描画
	DrawFormatString( 420, 190, 0xff0000, "Pspd = %.2f", Player.PSpeed );	//PSpeed描画
	DrawFormatString( 420, 210, 0xff0000, "InMd = %d", InMode );			//InMode描画
	DrawFormatString( 420, 230, 0xff0000, "IMd2 = %d", InMode2 );			//InMode2描画
	DrawFormatString( 420, 250, 0xff0000, "Slid = %d", SlideMode );			//InMode2描画
	DrawFormatString( 420, 270, 0xff0000, "JmpM = %d", JumpState );			//JumpMode描画
	DrawFormatString( 420, 290, 0xff0000, "JmpM = %d", Player.JumpMode );	//InMode2描画

	DrawCircle( Player.PlayerX, Player.PlayerY, 2, 0xFF69B4, 1 );
	DrawCircle( map[ ( ( Player.PlayerY )/32 ) ][ ( ( Player.PlayerX )/32 ) ].CoX, map[ ( ( Player.PlayerY )/32 ) ][ ( ( Player.PlayerX )/32 ) ].CoY, 2, 0x0000ff, 1 );
	DrawCircle( Player.PlayerX - _MASS_HALF + 8, Player.PlayerY + _MASS_HALF, 2, 0xffff00, 1 );
	DrawCircle( map[ ( ( Player.PlayerY )/32 ) + 1 ][ ( ( Player.PlayerX - _MASS_HALF + 8 )/32 ) ].CoX, map[ ( ( Player.PlayerY )/32 ) + 1 ][ ( ( Player.PlayerX - _MASS_HALF + 8 )/32 ) ].CoY, 2, 0x000000, 1 );
#endif
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
	Pic.P_Walk[3]=Pic.Player[4];

	return TRUE;
}

//マップ初期処理
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

// int ComX, int ComY 

int HitBlockUp() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		//if ( map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) - 1 ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) ].MapNum == HitBlockNum[ i ] )	return _HIT_TRUE;
		if ( map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF+4 )/32 )  ].MapNum == HitBlockNum[ i ] 
			|| map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF-4 )/32 )  ].MapNum == HitBlockNum[ i ]  ) {
				return _HIT_TRUE;
		}
	}
	
	return _HIT_FALSE;
}

int HitBlockDown() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		//if ( Player.P_lr_f == 0 && map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) + 1 ][ ( ( Player.PlayerX-_MASS_HALF )/32 )  ].MapNum == HitBlockNum[ i ] )		return _HIT_TRUE;
		//if ( Player.P_lr_f == 1 && map[ ( ( Player.PlayerY-_MASS_HALF )/32 ) + 1 ][ ( ( Player.PlayerX-_MASS_HALF )/32 ) + 1  ].MapNum == HitBlockNum[ i ] )	return _HIT_TRUE;
	
		if ( map[ ( ( Player.PlayerY+_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF+4 )/32 )  ].MapNum == HitBlockNum[ i ] 
		|| map[ ( ( Player.PlayerY+_MASS_HALF )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF-4 )/32 )  ].MapNum == HitBlockNum[ i ]  ) {
				return _HIT_TRUE;
		}
	
	}
	return _HIT_FALSE;
}

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