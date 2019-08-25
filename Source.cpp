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
#define _HITBLOCK	25

//当たり判定制御定数
#define _HIT_FALSE	0
#define _HIT_TRUE	1
#define _HIT_BREAK	2
#define _HIT_BRING	3

#define _HIT_RIGHT	4
#define _HIT_LEFT	5
#define _HIT_ITEM	6
#define _HIT_GOAL	7
#define _HIT_CASTLE	8

#define _HITW_PLAYER	0	//ヒット判定-プレイヤー
#define _HITW_PWRUP		1	//ヒット判定-パワーアップキノコ
#define _HITW_FBALL		2	//ヒット判定-ファイヤーボール
#define _HITW_ENEMY		3	//ヒット判定-敵
#define _HITW_FLAG		4	//ヒット判定-旗

//方向の定数
#define _DIRECT_RIGHT	0
#define _DIRECT_LEFT	1

//マリオ状態
#define _MARIO_NOMAL	0
#define _MARIO_SUPER	1
#define _MARIO_FIRE		2

//アイテム状態定数
#define _ITEM_NHAVE		0	//持ってない
#define _ITEM_BRING		1	//持ってる状態
#define _ITEM_ANIME		2	//アニメーションフラグ
#define _ITEM_MOVIN		3	//アイテムが動いてる状態(キノコなど)

#define _ITEMT_COIN		5	//コイン
#define _ITEMT_MCOIN	6	//何回か押せるコイン
#define _ITEMT_PWRUP	7	//パワーアップアイテム

//ゲーム状態変数
static int GAMESTATE;

//リフレッシュレート確認用変数
static int RefreshRate;
HDC hdc;

/*****      ゲームモード列挙体      *****/
typedef enum GAME_MODE {
	GAME_TITLE,			//タイトル
	GAME_INIT,			//初期化
	GAME_MAIN,			//メイン
	GAME_GOAL,			//ゴール画面(開発状態)
	GAME_END,			//エンド処理
	END = 99			//エンド
};

/*****      ゲームモード列挙体      *****/
//typedef enum BLOCK_NUMBER {
	

/*****      画像構造体      *****/
typedef struct PICTURE {
	int Player[ 15 ];		//ノーマルマリオ
	int Suplayer[ 15 ];		//スーパーマリオ
	int FirePlayer[ 15 ];	//ファイヤーマリオ
	int StageBlock[ 90 ];	//ステージ描画ブロック
	int P_Walk[ 4 ];		//歩行処理(ノーマル)
	int P_WalkS[ 4 ];		//歩行処理(スーパー)
	int P_WalkF[ 4 ];		//歩行処理(ファイヤー)
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
	int MapNum;			//マップナンバー
	int DrawFlg;		//DrawGraphするときにつかうやつ
	int CollisionFlg;	//衝突フラグ
	int BreakFlg;		//壊れた判定するやつ
	int CoX;			//ブロックのX座標
	int CoY;			//ブロックのY座標
	int ItemFlg;		//アイテムを持っているかいないか
	int ItemType;		//どのアイテムを持ってるか
	int ItemX;			//アイテム座標X
	int ItemY;			//アイテム座標Y
	float IYSpeed;		//落下スピード
	int ItemFrame;		//アイテム描画のフレーム
	int ItemTrans;		//アイテムの描画遷移
	int ItemDirect;		//キノコの動き(方向)制御
	int Block_UP;		//ブロックのはねる処理
	int BU_F;			//ブロックのはねる処理のフレーム
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
	int ItemDirect;		//キノコの動き(方向)制御
	int DrawMode;		//描画するかしないか
	float IYSpeed;		//アイテムの落下加速度
};
POWERUP Pup;

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
	int MapSSpeed;		//マップスクロールの現在量
	float PYSpeed;		//落下スピード
	int PDirectMode;	//方向で処理帰るときの変数
	int PlayerState;	//プレイヤーの状態( 1=Mario, 2=SuperMario, 3=FireMario )
	int PlayerAnime;	//アニメーション状態(全体が一度止まる処理)
	int PAnimeFrame;	//プレイヤーアニメーションフレーム
	int GoalFlg;		//ゴールフラグ
	int DeathFlg;		//死亡フラグ
	int CoinNum;		//コインの枚数
	int Point;			//ポイント
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
		{  0, 15, 16, 17,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 77, 78,  0, 15, 16, 17,  0,  0,  0,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0, 15, 16, 17,  0,  9,  9,  9,  9,  0,  0,  9,  9,  9,  0,  0,  0, 18,  0, 65, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 65, 66,  0,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0,  0, 15, 16, 17,  0,  0, 35,  0,  0,  0, 46, 46, 48, 46, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{ 15, 16, 69, 16, 17,  0,  0,  0, 77, 78,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0, 77, 78,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  0, 22, 23, 23, 24,  0, 77, 78, 15, 16, 69, 16, 17,  0,  0,  0,  0, 77, 78, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 22, 23, 23, 24,  0,  0,  0, 15, 16, 69, 16, 17,  0,  0,  0,  0,  0,  0, 22, 23, 23, 23, 24, 15, 16, 17,  0,  0,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9, 23, 23,  9,  9,  9,  9, 15, 16, 69, 16,  9,  9,  9,  9,  9,  0,  0,  9,  9,  9,  9, 24, 15, 16, 17, 77, 78,  0,  0, 22, 23, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78,  9,  9,  9,  9,  9,  9,  9,  9,  9,  0,  0, 15, 16, 69, 16, 17,  0,  9,  0,  0,  0, 46, 46, 70, 46, 46,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8},
		{  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8}
	};

//当たり判定のあるマップナンバー
//21
int HitBlockNum[ _HITBLOCK ] = { 1, 2, 3, 4, 5, 6, 7 , 8, 9, 37, 38, 39, 40, 41, 42, 49, 50, 51, 52, 53, 54, 65, 66, 77, 78 };

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
void DrawGoal();	//ゴール描画
void DrawEnd();		//エンド画面描画

//ゲームメイン画面描画系関数
void DrawStage();	//ステージ描画
void DrawPlayer();	//プレイヤー描画
void DrawBlock();	//ブロック描画
void DrawFlag();	//ゴール旗描画
int DrawItem( int iX, int iY /*, int iType, int iFrame*/ );	//アイテム描画

//マリオ処理系関数
// int ComX, int ComY 
int HitBlockUp();		//ブロック上当たり判定
int HitBlockDown();		//ブロック下当たり判定
int HitBlockRight();	//ブロック右当たり判定
int HitBlockLeft();		//ブロック左当たり判定

int HitBlockUp( int oX, int oY, int pX, int pY, int jMode, int who );		//ブロック上当たり判定
int HitBlockDown( int oX, int oY, int pX, int pY, int jMode, int who );		//ブロック下当たり判定
int HitBlockRight( int oX, int oY, int pX, int pY, int jMode, int who );		//ブロック右当たり判定
int HitBlockLeft( int oX, int oY, int pX, int pY,int jMode, int who );		//ブロック左当たり判定

int HitObjUp( int oX, int oY, int pX, int pY, int heightRad, int widthRad );		//敵,アイテムの当たり判定上
int HitObjDown( int oX, int oY, int pX, int pY, int heightRad, int widthRad );		//敵,アイテムの当たり判定下
int HitObjRight( int oX, int oY, int pX, int pY, int heightRad, int widthRad );		//敵,アイテムの当たり判定右
int HitObjLeft( int oX, int oY, int pX, int pY, int heightRad, int widthRad );		//敵,アイテムの当たり判定左

int HitObj( int oX, int oY, int pX, int pY, int heightRad, int widthRad );			//当たり判定全方向

int HitBlockUpBreak();	//ブロック上当たり判定、破壊判定

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
	Player.DeathFlg = 0;
	Player.GoalFlg = 0;
	Player.MapScrollX = 0;
	Player.MapSSpeed = 0;
	Player.PDirectMode = 0;
	Player.PJSpeed = 0;
	Player.PlayerState = _MARIO_NOMAL;
	Player.PlayerAnime = 0;
	Player.PlayerState = 1;

	Item.HataX = 200 * _MASS_X;
	Item.HataY = 2 * _MASS_Y + 5;

	GAMESTATE = GAME_MAIN;

}

//ゲームメイン処理
void GameMain() {

	DrawStage();		//ステージ描画
	DrawFlag();			//ゴールフラグ
	DrawPlayer();		//プレイヤー描画
	//DrawKinoko();		//キノコ描画

	if ( opt.Kflg & PAD_INPUT_START ) {
		GAMESTATE = GAME_TITLE;
	}
}

//ステージ描画
void DrawStage() {

	static int A_frame=0;

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

	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			if ( Player.PlayerAnime == 0 ) {
				map[ StageY ][ StageX ].CoX -= Player.MapSSpeed;
				if ( map[ StageY ][ StageX ].ItemFlg == _ITEM_BRING || map[ StageY ][ StageX ].ItemFlg == _ITEM_ANIME ) {
					map[ StageY ][ StageX ].ItemX -= Player.MapSSpeed;
				}
			}
		}
	}

	//マップ描画
	for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
		for ( int StageX = 0; StageX < _MAP_ALLSIZE_X; StageX++ ) {
			if ( Player.PlayerAnime == 0 ) {
				//コイン&パワーアップアイテムのアニメーション
				if ( map[ StageY ][ StageX ].ItemFlg == _ITEM_ANIME || map[ StageY ][ StageX ].ItemFlg == _ITEM_MOVIN ) {
					//初期処理(ハテナブロック変更)
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

				//ハテナブロックの色の遷移
				if(map[StageY][StageX].MapNum==2||map[StageY][StageX].MapNum==3||map[StageY][StageX].MapNum==4||map[StageY][StageX].MapNum==5){
					if(A_frame++<250)map[StageY][StageX].MapNum=2;
					else if(A_frame<350)map[StageY][StageX].MapNum=4;
					else if(A_frame<450)map[StageY][StageX].MapNum=5;
					else if(A_frame<550)map[StageY][StageX].MapNum=4;
					else if(A_frame==550)A_frame=0;
				}

				//ブロックのはねる処理
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

			//マップ描画
			DrawRotaGraph( map[ StageY ][ StageX ].CoX + _MASS_HALF, map[ StageY ][ StageX ].CoY + _MASS_HALF + map[ StageY ][ StageX ].Block_UP,
								1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
		}
	}

	//マップ描画
	//for ( int StageY = 0; StageY < _MAP_ALLSIZE_Y; StageY++ ) {
	//	for ( int StageX = ( Player.MapScrollX / _MASS_X ); StageX < ( Player.MapScrollX / _MASS_X ) * (_MAP_X + 4 ); StageX++ ) {
	//		DrawRotaGraph( map[ StageY ][ StageX ].CoX + _MASS_HALF, map[ StageY ][ StageX ].CoY + _MASS_HALF,
	//							1.0f, 0, Pic.StageBlock[ map[ StageY ][ StageX ].MapNum ], TRUE );
	//	}
	//}
	//Player.Scroll=-map[0][0].CoX;

}

//ゴール旗の描画
void DrawFlag() {

	if ( Player.GoalFlg == 1 && HitBlockDown( Item.HataX, Item.HataY, 0, _MASS_HALF, FALSE, _HITW_FLAG ) != _HIT_TRUE ) {
		Item.HataY += 3;
	} else if ( Player.GoalFlg == 1 && HitBlockDown( Item.HataX, Item.HataY, 0, _MASS_HALF, FALSE, _HITW_FLAG ) == _HIT_TRUE ) {
		Player.GoalFlg = 2;
	}
	DrawRotaGraph( Item.HataX - Player.MapScrollX, Item.HataY, 1.0f, 0, Pic.StageBlock[ 36 ], TRUE );

}

int DrawItem( int iX, int iY /*, int iType, int iFrame*/ ) {

	//コインの描画
	if ( map[ iY ][ iX ].ItemType == _ITEMT_COIN ) {
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

	//パワーアップキノコの描画
	if ( map[ iY ][ iX ].ItemType == _ITEMT_PWRUP ) {

		//ノーマルマリオがキノコを取る処理
		if ( Player.PlayerState == 1 ) {
			if ( map[ iY ][ iX ].ItemFrame++ < 1 ) {
				map[ iY ][ iX ].ItemY -= 10;
			}
			else if ( map[ iY ][ iX ].ItemFrame < 25 ) {
				map[ iY ][ iX ].ItemY -= 1;
			}
			else if ( map[ iY ][ iX ].ItemFrame >= 28 ) {
				//左右に動く
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
				/*****     落下処理     *****/
				if ( HitBlockDown( map[ iY ][ iX ].ItemX + Player.MapScrollX, map[ iY ][ iX ].ItemY, _MASS_HALF , _MASS_HALF, FALSE, _HITW_PWRUP ) == _HIT_FALSE ) {
					if ( map[ iY ][ iX ].IYSpeed < 8.0f )	map[ iY ][ iX ].IYSpeed += 0.8f;
					map[ iY ][ iX ].ItemY += ( int )map[ iY ][ iX ].IYSpeed;
				} else {
					map[ iY ][ iX ].IYSpeed = 0.0f;
				}
			}
			//当たり判定
			if ( HitObj( Player.PlayerX, Player.PlayerY, map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
				Player.PlayerState = 2;
				Player.PlayerAnime = 1;
				return TRUE;
			}
			//画面買いに消えた場合
			if ( map[ iY ][ iX ].ItemY >= ( 15 * _MASS_Y ) || map[ iY ][ iX ].ItemX >= ( 15 * _MASS_X ) || map[ iY ][ iX ].ItemX < ( 0 - _MASS_X ) ) {
				return TRUE;
			}
			//描画
			DrawRotaGraph( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, 1.0f, 0, Pic.StageBlock[ 13 ], TRUE );
		}
		//スーパーマリオかファイヤーマリオの時の処理
		else if ( Player.PlayerState >= 2 ) {
			if ( map[ iY ][ iX ].ItemFrame++ < 1 ) {
				map[ iY ][ iX ].ItemY -= 10;
			}
			else if ( map[ iY ][ iX ].ItemFrame < 25 ) {
				map[ iY ][ iX ].ItemY -= 1;
			}
			//当たり判定
			if ( HitObj( Player.PlayerX, Player.PlayerY, map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, _MASS_HALF, _MASS_HALF ) == _HIT_TRUE ) {
				if ( Player.PlayerState == 2 ) {
					Player.PlayerAnime = 1;
					Player.PlayerState = 3;
				} else if ( Player.PlayerState == 3 ) {
					Player.Point += 100;
				}
				return TRUE;
			}
			//画面外だと消える処理
			if ( map[ iY ][ iX ].ItemY >= ( 15 * _MASS_Y ) || map[ iY ][ iX ].ItemX >= ( 15 * _MASS_X ) || map[ iY ][ iX ].ItemX < ( 0 - _MASS_X ) ) {
				return TRUE;
			}
			//描画
			DrawRotaGraph( map[ iY ][ iX ].ItemX, map[ iY ][ iX ].ItemY, 1.0f, 0, Pic.StageBlock[ 25 ], TRUE );
		}
	}

	return FALSE;

}

//プレイヤー描画
void DrawPlayer() {

	static int PDrawMode = 0;	//0=動いてない、1=右動いている、 2=左動いてる
	static int SlideMode = 0;	//1=右滑り、2=左滑り
	static int JumpState = 1;	//ジャンプの状態を持っておく
	int InMode = 0;
	int InMode2 = 0;
	int WalkMove = 9 - ( int )Player.PSpeed;
	if ( ( ( opt.NowK & PAD_INPUT_M ) == 0 ) && JumpState == 1 ) {
		JumpState = 0;
	}

	//歩くアニメーション
	if ( Player.PlayerAnime == 0 ) {
		if( ( 0 == FR_Control.FrameCount % WalkMove ) && /*opt.NowK != NULL*/ Player.PSpeed >= 0.0f ){
			if ( Player.JumpFrame == 0 )	Player.P_i_f++;
			if(Player.P_i_f >= 3 && Player.JumpFrame == 0 )	Player.P_i_f=0;
			if ( Player.JumpFrame > 0 )	Player.P_i_f = 3;
		}
		else if( Player.PSpeed <= 0.0f ){
			Player.P_i_f = 0;	//キー操作をやめた時
		}
	}

	//歩行処理
	/*****     右移動処理     *****/
	if ( Player.GoalFlg == 0 && Player.PlayerX <= ( 15 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_RIGHT || opt.NowK & PAD_INPUT_Z ) ) {

		/** 通常処理 **/
		if ( PDrawMode == 0 || ( PDrawMode == 2 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 1;
		}
		if ( PDrawMode == 1 ) {
			if ( Player.PlayerAnime == 0 ) {
				//ノーマルマリオだと
				if ( Player.PlayerState == 1 ) {
					if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						if ( Player.PSpeed < 6.0f ) {
							Player.PSpeed += 0.2f;				//加速度設定
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
				//スーパーマリオ以上だと
				else if ( Player.PlayerState == 2 || Player.PlayerState ) {
					if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						if ( Player.PSpeed < 6.0f ) {
							Player.PSpeed += 0.2f;				//加速度設定
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
			Player.P_lr_f = _DIRECT_RIGHT;					//左右反転フラグ
			//歩行時のプレイヤー描画
			if ( Player.PlayerState == 1 ) {									//ノーマルマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, FALSE );		
			}
			else if ( Player.PlayerState == 2 ) {								//スーパーマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkS[Player.P_i_f], TRUE, FALSE );		
			}
			else if ( Player.PlayerState == 3 ) {								//ファイヤーマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkF[Player.P_i_f], TRUE, FALSE );	
			}
		}
		/** 急ブレーキの処理 **/
		if ( PDrawMode == 2 && Player.PSpeed > 0.0f ) {
			Player.P_lr_f=0;					//左右反転フラグ
			SlideMode = 1;
			if ( Player.PlayerAnime == 0 ) {
				//ノーマルマリオだと
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
				//スーパーマリオ以上だと
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
			//プレイヤー描画
			if ( Player.PlayerState == 1 ) {									//ノーマルマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ 6 ], TRUE, FALSE );
			}
			else if ( Player.PlayerState == 2 ) {								//スーパーマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[ 7 ], TRUE, FALSE );
			}
			else if ( Player.PlayerState == 3 ) {								//ファイヤーマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[ 7 ], TRUE, FALSE );
			}
		}
	}
	/*****     左移動処理     *****/
	else if ( Player.GoalFlg == 0 && Player.PlayerX > ( 0 * _MASS_X + _MASS_HALF ) && ( opt.NowK & PAD_INPUT_LEFT || opt.NowK & PAD_INPUT_X ) ) {

		/** 通常処理 **/
		if ( PDrawMode == 0 || ( PDrawMode == 1 && Player.PSpeed <= 0.0f ) ) {
			PDrawMode = 2;
		}
		if ( PDrawMode == 2 ) {
			InMode = 2;
			if ( Player.PlayerAnime == 0 ) {
				//ノーマルマリオ処理
				if ( Player.PlayerState == 1 ) {
					if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						if ( Player.PSpeed < 6.0f ) {
							Player.PSpeed += 0.2f;			//加速度設定
						}
						Player.PlayerX -= Player.PSpeed;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {
						Player.PSpeed = 0.0f;
						Player.MapSSpeed = 0;
					} else if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_GOAL ) {
						GAMESTATE = GAME_GOAL;
					}
				}
				//スーパーマリオ以上処理
				else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
					if ( HitBlockLeft( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
						if ( Player.PSpeed < 6.0f ) {
							Player.PSpeed += 0.2f;			//加速度設定
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
			Player.P_lr_f = _DIRECT_LEFT;					//左右反転フラグ
			//プレイヤー描画
			if ( Player.PlayerState == 1 ) {
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_Walk[Player.P_i_f], TRUE, TRUE );			//歩行時のプレイヤー描画
			} 
			else if ( Player.PlayerState == 2 ) {
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkS[Player.P_i_f], TRUE, TRUE );			//歩行時のプレイヤー描画
			}
			else if ( Player.PlayerState == 3 ) {
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.P_WalkF[Player.P_i_f], TRUE, TRUE );			//歩行時のプレイヤー描画
			}
		}
		/** 急ブレーキの処理 **/
		if ( PDrawMode == 1 && Player.PSpeed > 0.0f ) {
			SlideMode = 2;
			InMode2 = 2;
			Player.P_lr_f=1;					//左右反転フラグ
			if ( Player.PlayerAnime == 0 ) {
				//ノーマルマリオ処理
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
				//スーパーマリオ以上処理
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
	/*****  その他処理   *****/
	else {
		if ( Player.PSpeed > 0.0f ) {
			if ( Player.PlayerAnime == 0 ) {
				Player.PSpeed -= 0.2f;
				/*****  マリオ滑り処理   *****/
				if ( Player.PSpeed > 0.0f ) {
					//右処理
					if ( PDrawMode == 1 && ( Player.PlayerX < 15 * _MASS_X + _MASS_HALF ) ) { 
						//ノーマルマリオ処理
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
						//スーパーマリオ以上処理
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
					//左処理
					else if ( PDrawMode == 2 && ( Player.PlayerX > 0 * _MASS_X + _MASS_HALF ) ) {
						//ノーマルマリオ処理
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
						//スーパーマリオ以上処理
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
			//プレイヤー描画
			if ( Player.PlayerState == 1 ) {				//ノーマルマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[ Player.P_i_f ], TRUE , Player.P_lr_f );
			}
			else if ( Player.PlayerState == 2 ) {			//スーパーマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[ Player.P_i_f ], TRUE , Player.P_lr_f );
			}
			else if ( Player.PlayerState == 3 ) {			//ファイヤーマリオ
				DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[ Player.P_i_f ], TRUE , Player.P_lr_f );
			}
		}
		if ( Player.PSpeed <= 0.0f ) {
			PDrawMode = 0;
			Player.PSpeed = 0.0f;
			Player.MapSSpeed = 0;
		}
		InMode = 1;
	}

	/*****     ジャンプ処理     *****/
	if ( Player.GoalFlg == 0 && JumpState == 0 && Player.JumpMode == FALSE && ( opt.NowK & PAD_INPUT_M ) ) {
		Player.JumpMode = TRUE;
		Player.JumpFrame = 0;
		Player.PJSpeed = 7.0f;
		JumpState = 2;
	}
	
	if ( Player.PlayerAnime == 0 ) {
		if ( Player.JumpMode == TRUE ) {
			if ( Player.PlayerState == 1 ) {
				//上方向のブロックの当たり判定
				if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {			
					//真ん中にヒットする場合
					Player.PJSpeed = 0.0f;
					Player.JumpMode = FALSE;
					Player.JumpFrame = 0;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_LEFT ) {	
					//(マリオの)左側に当たった時
					int Difference = 0;
					Difference = map[ ( Player.PlayerY-_MASS_HALF+4 )/32][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX -
									( ( Player.PlayerX ) - _MASS_HALF + 4 );
					Player.PlayerX += Difference;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_RIGHT ) {	
					//(マリオの)左側に当たった時
					int Difference = 0;
					Difference = ( ( Player.PlayerX )+_MASS_HALF-4 ) - 
								map[ ( Player.PlayerY-_MASS_HALF+4 )/32 ][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX;
					Player.PlayerX -= Difference;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {	
					//ヒットしない場合
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
				//上方向のブロックの当たり判定
				if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_TRUE ) {			
					//真ん中にヒットする場合
					Player.PJSpeed = 0.0f;
					Player.JumpMode = FALSE;
					Player.JumpFrame = 0;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_LEFT ) {	
					//(マリオの)左側に当たった時
					int Difference = 0;
					Difference = map[ ( Player.PlayerY-_MASS_HALF+4 )/32][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX -
									( ( Player.PlayerX ) - _MASS_HALF + 4 );
					Player.PlayerX += Difference;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_RIGHT ) {	
					//(マリオの)左側に当たった時
					int Difference = 0;
					Difference = ( ( Player.PlayerX )+_MASS_HALF-4 ) - 
								map[ ( Player.PlayerY-_MASS_HALF+4 )/32 ][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX;
					Player.PlayerX -= Difference;
				} else if ( HitBlockUp( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {	
					//ヒットしない場合
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

	/*****     落下処理     *****/
	if ( Player.PlayerAnime == 0 ) {

		//ノーマルマリオ処理
		if ( Player.PlayerState == 1 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				JumpState = 4;
				if ( Player.PYSpeed < 8.0f )	Player.PYSpeed += 0.8f;
				Player.PlayerY += ( int )Player.PYSpeed;
				//めり込み防止処理
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
		}
		//スーパーマリオ以上処理
		else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
			if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && Player.JumpMode == FALSE && HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_FALSE ) {
				JumpState = 4;
				if ( Player.PYSpeed < 8.0f )	Player.PYSpeed += 0.8f;
				Player.PlayerY += ( int )Player.PYSpeed;
				//めり込み防止処理
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
				}
				if ( JumpState == 4 && ( opt.NowK & PAD_INPUT_M ) == 0 ) {
					JumpState = 0;
				}
			}
			else {
				Player.PYSpeed = 0.0f;
			}
		}
	}

	//パワーアップアニメーション
	if ( Player.PlayerAnime == 1 ) {
		if ( Player.PAnimeFrame++ < 1 ) {
			if ( Player.PlayerState == 2 ) {
				Player.PlayerY -= _MASS_HALF;
			}
		}
		else if ( Player.PAnimeFrame > 90 ) {
			Player.PlayerAnime = 0;
		}
	}

	//ゴールアニメーション
	static int pg_walk;
	if ( Player.GoalFlg == 1 ) {
		//ノーマルマリオ処理
		if ( Player.PlayerState == 1 ) {
			if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) != _HIT_TRUE ) {
				Player.PlayerY += 3;
			}
		}
		//スーパーマリオ以上処理
		else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
			if ( HitBlockDown( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) != _HIT_TRUE ) {
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
		}
		//ノーマルマリオ処理
		if ( Player.PlayerState == 1 ) {
			if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_HALF, FALSE, _HITW_PLAYER ) == _HIT_CASTLE ){
				Player.GoalFlg=4;
				GAMESTATE=GAME_GOAL;
			}
		}
		//スーパーマリオ以上処理
		else if ( Player.PlayerState == 2 || Player.PlayerState == 3 ) {
			if ( HitBlockRight( Player.MapScrollX + Player.PlayerX, Player.PlayerY, _MASS_HALF, _MASS_Y, FALSE, _HITW_PLAYER ) == _HIT_CASTLE ){
				Player.GoalFlg=4;
				GAMESTATE=GAME_GOAL;
			}
		}
	}

	//static int i=7;
	//if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
	//	if(i==8)i=7;
	//	else if(i==7)i=8;
	//}

	//ゴール画像描画
	if ( Player.PlayerState == 1 ) {			//ノーマルマリオ
		static int i=7;
		if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
			if(i==8)i=7;
			else if(i==7)i=8;
		}
		if ( Player.GoalFlg == 1 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[i], TRUE, FALSE );			//ゴールした時
		} else if ( Player.GoalFlg == 2 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[7], TRUE, TRUE );			//ゴールした(一番下に降りた)とき(反転)
		}
	}
	else if ( Player.PlayerState == 2 ) {		//スーパーマリオ
		static int i=8;
		if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
			if(i==9)i=8;
			else if(i==8)i=9;
		}
		if ( Player.GoalFlg == 1 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[i], TRUE, FALSE );			//ゴールした時
		} else if ( Player.GoalFlg == 2 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[7], TRUE, TRUE );			//ゴールした(一番下に降りた)とき(反転)
		}
	}
	else if ( Player.PlayerState == 3 ) {		//ファイヤーマリオ
		static int i=8;
		if(Player.GoalFlg==1 && 0==FR_Control.FrameCount%5){
			if(i==9)i=8;
			else if(i==8)i=9;
		}
		if ( Player.GoalFlg == 1 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[i], TRUE, FALSE );			//ゴールした時
		} else if ( Player.GoalFlg == 2 ) {
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[7], TRUE, TRUE );			//ゴールした(一番下に降りた)とき(反転)
		}
	}

	//プレイヤー描画
	if ( Player.PlayerState == 1 ) {
		if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[0], TRUE , Player.P_lr_f );	//ジャンプしてないとき
		if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Player[4], TRUE , Player.P_lr_f );	//ジャンプした時
	}
	else if ( Player.PlayerState == 2 ) {
		if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[0], TRUE , Player.P_lr_f );	//ジャンプしてないとき
		if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.Suplayer[4], TRUE , Player.P_lr_f );	//ジャンプした時
	}
	else if ( Player.PlayerState == 3 ) {
		if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode == 0 )	
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[0], TRUE , Player.P_lr_f );	//ジャンプしてないとき
		if ( ( Player.GoalFlg == 0 || Player.GoalFlg == 3 ) && ( PDrawMode == 3 || PDrawMode == 0 ) && Player.JumpMode != 0 )	
			DrawRotaGraph( Player.PlayerX, Player.PlayerY, 1.0f, 0, Pic.FirePlayer[4], TRUE , Player.P_lr_f );	//ジャンプした時
	}

#ifdef _DEBUGMODE
	SetFontSize( 12 );
	DrawFormatString( 420,  50, 0xff0000, "OldK = %d", opt.OldK );				//OldK描画
	DrawFormatString( 420,  70, 0xff0000, "NowK = %d", opt.NowK );				//NowK描画
	DrawFormatString( 420,  90, 0xff0000, "PdrM = %d", PDrawMode );				//PDrawMode描画
	DrawFormatString( 420, 110, 0xff0000, "LR_F = %d", Player.P_lr_f );			//P_lr_f描画
	DrawFormatString( 420, 130, 0xff0000, "PlrX = %d", Player.PlayerX );		//PlayerX描画
	DrawFormatString( 420, 150, 0xff0000, "PlrY = %d", Player.PlayerY );		//PlayerY描画
	DrawFormatString( 420, 170, 0xff0000, "Scrl = %d", Player.MapScrollX );		//Scroll描画
	DrawFormatString( 420, 190, 0xff0000, "Pspd = %.2f", Player.PSpeed );		//PSpeed描画
	DrawFormatString( 420, 210, 0xff0000, "Pint = %d", Player.Point );			//InMode描画
	DrawFormatString( 420, 230, 0xff0000, "Coin = %d", Player.CoinNum );		//コイン枚数描画
	DrawFormatString( 420, 250, 0xff0000, "PlrS = %d", Player.PlayerState );	//InMode2描画
	DrawFormatString( 420, 270, 0xff0000, "JmpM = %d", ( ( Player.PlayerX ) - _MASS_HALF + 4 ) );			//JumpMode描画
	DrawFormatString( 420, 290, 0xff0000, "CoX  = %d", map[ ( Player.PlayerY-_MASS_HALF+4 )/32][ ( ( Player.MapScrollX+Player.PlayerX )+_MASS_HALF-4 )/32 ].CoX );	//InMode2描画

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
	if ( LoadDivGraph( "images/mario_chara.png", 15, 5, 3, 32, 32, Pic.Player ) == -1 )			return -1;
	//スーパーマリオ読み込み
	if ( LoadDivGraph( "images/super_mario_chara.png", 15, 5, 3, 32, 64, Pic.Suplayer ) == -1 )	return -1;
	//ファイヤーマリオ読み込み
	if ( LoadDivGraph( "images/fire_mario_chara .png", 15, 5, 3, 32, 64, Pic.FirePlayer ) == -1 )	return -1;

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

//マップ初期処理
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
			//ハテナブロックだった場合
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
//上側判定
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

//下側判定
int HitBlockDown() {
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( map[ ( ( Player.PlayerY+_MASS_HALF )/32 ) ][ ( ( Player.PlayerX-_MASS_HALF+4 )/32 )  ].MapNum == HitBlockNum[ i ] 
		|| map[ ( ( Player.PlayerY+_MASS_HALF )/32 ) ][ ( ( Player.PlayerX+_MASS_HALF-4 )/32 )  ].MapNum == HitBlockNum[ i ]  ) {
				return _HIT_TRUE;
		}
	
	}
	return _HIT_FALSE;
}

//右側判定
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

//左側判定
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

//ブロック上当たり判定
int HitBlockUp( int oX, int oY, int pX, int pY, int jMode, int who ){
	for ( int i = 0; i < _HITBLOCK; i++ ) {
		if ( who == _HITW_PLAYER ) {
			if ( map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				if ( map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].ItemFlg == _ITEM_BRING ) {
					map[ ( ( oY-pY )/32 ) ][ ( ( oX )/32 ) ].ItemFlg = _ITEM_ANIME;
				}
				if ( who == _HITW_PLAYER ) {
					if ( Player.PlayerState == 1 ) {
						if ( map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 2 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 3 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 4 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 5 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 6 ||
							 map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].MapNum == 7 ) {
								map[ ( ( Player.PlayerY-_MASS_HALF ) /32 ) ][ ( ( Player.MapScrollX + Player.PlayerX )/32 ) ].BU_F=1;
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

//ブロック下当たり判定
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
	}
	return _HIT_FALSE;
}

//ブロック右当たり判定
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
	}
	
	return _HIT_FALSE;
}

//ブロック左当たり判定
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
		if ( who == _HITW_PWRUP ) {
			if ( map[ ( ( oY-pY+4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ]
				|| map[ ( ( oY+pY-4 )/32 ) ][ ( ( oX-pX )/32 ) ].MapNum == HitBlockNum[ i ] ) {
				return _HIT_TRUE;
			}
		}
	}
	
	return _HIT_FALSE;
}

//オブジェクトの当たり判定
int HitObj( int oX, int oY, int pX, int pY, int heightRad, int widthRad ) {

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

	if ( ( oX + _MASS_HALF ) > ( pX - _MASS_HALF ) && ( oX - _MASS_HALF ) < ( pX + _MASS_HALF ) ) {
		if ( ( oY + _MASS_HALF ) > ( pY - _MASS_HALF ) && ( oY - _MASS_HALF ) < ( pY + _MASS_HALF ) ) {
			return _HIT_TRUE;
		}
	}

	return _HIT_FALSE;

}