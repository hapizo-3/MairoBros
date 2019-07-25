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
#define _MASS_ENEMY_HALF	24

/*****		マップの高さ		*****/
#define _MAP_X		16
#define _MAP_Y		14

//敵のY軸の大きさ
#define _MAP_ENEMY	48

//プレイヤー系定数
#define _PLAYER_MAXSPEED	3

//当たり判定のあるブロックの総数
#define _HITCHIP_ALL	21

//マップ全サイズ
#define _MAP_ALLSIZE_X 220
#define _MAP_ALLSIZE_Y 14

//文字サイズ
#define _SIZE_STR	16

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
	int TitleImage[ 1 ];
	int StrImage[ 60 ];
	int TimeImage[ 10 ];
	int Enemy[ 20 ];
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
	int JF_f;
	int MapScrollX;	//マップスクロールするのに必要なやつ
	int Scroll;		//マップスクロールの現在量
};
PLAYER Player = { ( ( 2 * _MASS_X ) + _MASS_HALF ), ( 11 * _MASS_Y + _MASS_HALF ), 0, 0 };

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
int HitBlockNum[ 20 ] = { 1, 2, 3, 4, 5, 6, 7 , 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//エネミー構造体
typedef struct ENEMY {
	int EnemyX;
	int EnemyY;
	float ESpeed;
};
ENEMY Enemy = { ( Player.PlayerX + ( 15 * _MASS_X ) + _MASS_HALF ), ( 12 * _MASS_Y - _MASS_ENEMY_HALF ), 0 };

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
void DrawUI();		//UI表示

//マリオ処理系関数
int HitBlock();		//ブロック当たり判定

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
	const int _WINDOWSIZE_X = 900;
	const int _WINDOWSIZE_Y = 648;
#endif

//デバッグモードなしの時のウィンドウサイズ設定
#ifndef _DEBUGMODE
	const int _WINDOWSIZE_X = 712;
	const int _WINDOWSIZE_Y = 648;
#endif

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
#ifdef _DEBUGMODE
			FR_Draw();
#endif
		SetDrawScreen( DX_SCREEN_BACK );
		DrawExtendGraph( 0, 0, _WINDOWSIZE_X - 188, _WINDOWSIZE_Y, offscreen_handle, FALSE );
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

	DrawStage();

	//マリオ
	DrawRotaGraph( 3*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 22 ] , TRUE); 
	DrawRotaGraph( 4*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 10 ] , TRUE);  
	DrawRotaGraph( 5*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 27 ] , TRUE); 
	DrawRotaGraph( 6*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 18 ] , TRUE); 
	DrawRotaGraph( 7*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 24 ] , TRUE);

	//得点
	DrawRotaGraph( 3*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 4*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 5*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 6*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 7*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 8*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);

	//コインの枚数
	DrawRotaGraph( 11*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 50 ] , TRUE);
	DrawRotaGraph( 12*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 37 ] , TRUE);
	DrawRotaGraph( 13*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 14*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);

	//ワールド
	DrawRotaGraph( 18*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 32 ] , TRUE);
	DrawRotaGraph( 19*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 24 ] , TRUE);
	DrawRotaGraph( 20*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 27 ] , TRUE);
	DrawRotaGraph( 21*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 21 ] , TRUE);
	DrawRotaGraph( 22*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 13 ] , TRUE);

	//ステージ
	DrawRotaGraph( 19*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 1 ] , TRUE);
	DrawRotaGraph( 20*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 36 ] , TRUE);
	DrawRotaGraph( 21*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 1 ] , TRUE);

	//タイム
	DrawRotaGraph( 26*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 29 ] , TRUE);
	DrawRotaGraph( 27*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 18 ] , TRUE);
	DrawRotaGraph( 28*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 22 ] , TRUE);
	DrawRotaGraph( 29*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 14 ] , TRUE);

	//タイム数字
	DrawRotaGraph( 27*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 4 ] , TRUE);
	DrawRotaGraph( 28*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 29*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);

	//タイトル画面
	DrawRotaGraph( 256, 134, 2.1f, 0, Pic.TitleImage[ 0 ] , TRUE );

	/*int x;

	x = 320 - GetDrawStringWidth( "MARIO", 5 );
	DrawFormatString( x, 240, 0xff0000, "MARIO" );

	x = 320 - GetDrawStringWidth( "Push Space", 10 );
	DrawFormatString( x, 400, 0xffffff, "Push Space" );*/

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

	static int AnimationFram = 0;

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

}

//プレイヤー描画
void DrawPlayer() {

	int PDrawMode = 0;	//0=動いてない、1 = 動いている(左右キー入力が入ってる)
	static int JumpMode = 0;

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
	if ( Player.PlayerX <= ( 6 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {
		PDrawMode = 1;						//動いてるかいないかの処理
		//加速度設定
		if ( Player.PSpeed < 4.0f ) {
			Player.PSpeed += 0.2f;
		}
		Player.PlayerX += Player.PSpeed;	//プレイヤー移動
		Player.P_lr_f=0;					//左右反転フラグ
		if ( Player.PlayerX >= 6  * _MASS_X + _MASS_HALF ) {
			Player.PlayerX -= Player.PSpeed;
			Player.MapScrollX = Player.PSpeed;
		}

		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		//歩行時のプレイヤー描画
	}
	/*****     左移動処理     *****/
	else if ( Player.PlayerX >= ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X ) ) {
		PDrawMode = 1;						//動いてるかいないかの処理
		if ( Player.PSpeed < 4.0f ) {
			Player.PSpeed += 0.2f;			//加速度設定
		}
		Player.MapScrollX = 0;
		Player.PlayerX -= Player.PSpeed;	//プレイヤー移動
		Player.P_lr_f=1;					//左右反転フラグ
		DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//歩行時のプレイヤー描画
	}
	else {
		if ( Player.PSpeed >= 0.0f ) {
			Player.PSpeed -= 0.4f;
			if ( Player.P_lr_f == 0 && ( Player.PlayerX < 5 * _MASS_X + _MASS_HALF ) ) { 
				Player.PlayerX += Player.PSpeed;
			} 
			else if ( Player.P_lr_f == 1 && ( Player.PlayerX > 0 * _MASS_X + _MASS_HALF ) ) {
				Player.PlayerX -= Player.PSpeed;
			}
		}
		Player.MapScrollX = 0;
	}

	//ジャンプ処理
	if( Player.JumpFrame == 0 && opt.Kflg & PAD_INPUT_M ) {
		Player.JumpFrame++;
	}
	if ( Player.JumpFrame > 0 ) {
		if ( Player.JumpFrame < 12 )	Player.JF_f = 1;
		else if ( Player.JumpFrame < 18 )	Player.JF_f = 2;
		else if ( Player.JumpFrame < 24 )	Player.JF_f = 3;
		else if ( Player.JumpFrame < 30 )	Player.JF_f = 4;
		else if ( Player.JumpFrame < 36 )	Player.JF_f = 5;
		else if ( Player.JumpFrame < 47 )	Player.JF_f = 6;
		if ( Player.JumpFrame == 47 ) {
			Player.JumpFrame = 0;
		}

		switch ( Player.JF_f )
		{
			case 1:
				Player.JumpFrame += 1;
				Player.PlayerY -= 9;
				if ( map[ ( Player.PlayerY / 32 ) - 1 ][ ( Player.PlayerX / 32 ) ].MapNum != 0 )	Player.JumpFrame = 36;
				break;
			case 2:
				Player.JumpFrame += 1;
				Player.PlayerY -= 5;
				if ( map[ ( Player.PlayerY / 32 ) - 1 ][ ( Player.PlayerX / 32 ) ].MapNum != 0 )	Player.JumpFrame = 30;
				break;
			case 3:
				Player.JumpFrame += 1;
				Player.PlayerY -= 3;
				if ( map[ ( Player.PlayerY / 32 ) - 1 ][ ( Player.PlayerX / 32 ) ].MapNum != 0 )	Player.JumpFrame = 24;
				break;
			case 4:
				Player.JumpFrame += 1;
				Player.PlayerY += 3;
				if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1 ][ ( Player.PlayerX / 32 ) ].MapNum != 0 )	Player.JumpFrame=0;
				break;
			case 5:
				Player.JumpFrame += 1;
				Player.PlayerY += 5;
				if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1 ][ ( Player.PlayerX / 32 ) ].MapNum != 0 )	Player.JumpFrame=0;
				break;
			case 6:
				Player.JumpFrame += 1;
				Player.PlayerY += 9;
				if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1][ ( Player.PlayerX / 32 ) ].MapNum != 0 )	Player.JumpFrame=0;
				if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1][ ( Player.PlayerX / 32 ) ].MapNum == 0 )	Player.JumpFrame=36;
				break;
			default:
				break;
		}
	}
	if ( 0 != Player.PlayerY % 16 && Player.JumpFrame == 0 )	Player.PlayerY = ( Player.PlayerY / 16 ) * 16;
	if ( map[ ( ( Player.PlayerY - 16 ) / 32 ) + 1 ][ ( Player.PlayerX / 32 ) ].MapNum == 0 && Player.JumpFrame == 0 )	Player.JumpFrame = 24;

#ifdef _DEBUGMODE
	DrawFormatString( 516,  50, 0xff0000, "OldK = %d", opt.OldK );		//OldK描画
	DrawFormatString( 516,  80, 0xff0000, "NowK = %d", opt.NowK );		//NowK描画
	DrawFormatString( 516, 110, 0xff0000, "PdrM = %d", PDrawMode );		//PDrawMode描画
	DrawFormatString( 516, 140, 0xff0000, "LR_F = %d", Player.P_lr_f );	//P_lr_f描画
	DrawFormatString( 516, 170, 0xff0000, "PlrX = %d", Player.PlayerX );//PlayerX描画
	DrawFormatString( 516, 200, 0xff0000, "PlrY = %d", Player.PlayerY );	//PlayerY描画
#endif

	//無動作時のプレイヤー描画
	if ( PDrawMode == 0 && Player.JumpFrame == 0 )	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );
	if ( PDrawMode == 0 && Player.JumpFrame != 0 )	DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );
}

//UI描画処理
void DrawUI(){

	//wは退避領域
	static int Flame = 0 , Time = 400 , w;
	Flame ++;

	//マリオ
	DrawRotaGraph( 3*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 22 ] , TRUE); 
	DrawRotaGraph( 4*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 10 ] , TRUE);  
	DrawRotaGraph( 5*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 27 ] , TRUE); 
	DrawRotaGraph( 6*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 18 ] , TRUE); 
	DrawRotaGraph( 7*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 24 ] , TRUE);

	//得点
	DrawRotaGraph( 3*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 4*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 5*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 6*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 7*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 8*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);

	//コインの枚数
	DrawRotaGraph( 11*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 50 ] , TRUE);
	DrawRotaGraph( 12*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 37 ] , TRUE);
	DrawRotaGraph( 13*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);
	DrawRotaGraph( 14*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 0 ] , TRUE);

	//ワールド
	DrawRotaGraph( 18*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 32 ] , TRUE);
	DrawRotaGraph( 19*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 24 ] , TRUE);
	DrawRotaGraph( 20*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 27 ] , TRUE);
	DrawRotaGraph( 21*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 21 ] , TRUE);
	DrawRotaGraph( 22*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 13 ] , TRUE);

	//ステージ
	DrawRotaGraph( 19*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 1 ] , TRUE);
	DrawRotaGraph( 20*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 36 ] , TRUE);
	DrawRotaGraph( 21*_SIZE_STR , 32 , 1.0f , 0 , Pic.StrImage[ 1 ] , TRUE);

	//タイム
	DrawRotaGraph( 26*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 29 ] , TRUE);
	DrawRotaGraph( 27*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 18 ] , TRUE);
	DrawRotaGraph( 28*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 22 ] , TRUE);
	DrawRotaGraph( 29*_SIZE_STR , 16 , 1.0f , 0 , Pic.StrImage[ 14 ] , TRUE);
	
	//タイム数字
	DrawRotaGraph( 27*_SIZE_STR , 32 , 1.0f , 0 , Pic.TimeImage[ Time / 100 ] , TRUE);
	DrawRotaGraph( 28*_SIZE_STR , 32 , 1.0f , 0 , Pic.TimeImage[ w / 10 ] , TRUE);
	DrawRotaGraph( 29*_SIZE_STR , 32 , 1.0f , 0 , Pic.TimeImage[ Time % 10 ] , TRUE);
	
	w = Time % 100;

	if( Flame == 30){
		Flame = 0;
		Time --;
	}
}

//画像読込
int LoadImages() {

	//ブロック読込
	if ( LoadDivGraph( "images/backimage.png", 84, 12, 7, 32, 32, Pic.StageBlock + 1 ) == -1 )	return -1;
	//キャラクター読込
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )	return -1;
	//タイトル読込
	if ( ( Pic.TitleImage[0] = LoadGraph( "images/TitleImage01.png" ) ) == -1 ) return -1;
	//タイトル文字読込
	if ( LoadDivGraph( "images/font.png", 60, 10, 6, 16, 16, Pic.StrImage ) == -1 )	return -1;
	//タイム文字読み込み
	if ( LoadDivGraph( "images/font.png", 10, 10, 1, 16, 16, Pic.TimeImage ) == -1 )	return -1;
	//エネミー読込
	if ( LoadDivGraph( "images/mob.png", 20, 10, 2, 32, 48, Pic.Enemy ) == -1 )	return -1;


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