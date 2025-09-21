//=============================================================================
//
//	シューティングゲームアルファ版 [main.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MAIN_H_			// このマクロ定義がされていなかったら
#define _MAIN_H_			// 2重インクルード防止のマクロを定義する

#include <windows.h>
#include <cstdio>
#include "d3dx9.h"								// 描画処理に必要
#define DIRECTINPUT_VERSION		(0x0800)		// ビルド時の警告対処用マクロ
#include "dinput.h"								// 入力処理に必要
#include "xaudio2.h"							// サウンド処理に必要
#include "xinput.h"								// ジョイパッド処理に必要

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")				// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")				// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")				// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")				// システムの時刻取得に必要
#pragma comment(lib, "dinput8.lib")				// 入力処理に必要
#pragma comment(lib, "xinput.lib")				// ジョイパッド処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH		(1280.0f)											// ウインドウの幅
#define SCREEN_HEIGHT		(720.0f)											// ウインドウの高さ
#define SCREEN_RADIUS		(640.0f)
#define WARLD_WIDTH			(2000.0f)											// ワールド座標X
#define WARLD_HEIGHT		(2000.0f)											// ワールド座標Y
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット(2D)
#define SQRTF(X, Y)			(sqrtf(X * X + Y * Y))								// 対角線の長さ算出

//*****************************************************************************
// モードの種類
//*****************************************************************************
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_GAME,				// ゲーム画面
	MODE_TUTORIAL,			// チュートリアル画面
	MODE_RESULT,			// リザルト画面
	MODE_RANKING,			// ランキング画面
	MODE_PAUSE,				// ポーズ画面
	MODE_MAX				// 総数
}MODE;

//*****************************************************************************
// 頂点情報の構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標(x, y, z)
	float rhw;				// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;			// 頂点カラー(r, g, b, a)
	D3DXVECTOR2 tex;		// テクスチャ座標(x, y)
}VERTEX_2D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif