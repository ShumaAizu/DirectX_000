//=============================================================================
//
//	タイトル画面処理 [title.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlebg.h"
#include "titlemenu.h"
#include "selectlevel.h"
#include "titlelogo.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TITLE_UI		(4)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_UI] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// 頂点バッファへのポインタ
TITLESTATE g_titleState = TITLESTATE_OP;							// タイトルの状態
int g_nCounterTitleState = 0;										// 状態管理カウンター

//====================================
//	タイトルの初期化処理
//====================================
void InitTitle(void)
{

	g_titleState = TITLESTATE_OP;

	// タイトル背景の初期化
	InitTitleBG();

	// エフェクトの初期化
	InitEffect();

	// パーティクルの初期化
	InitParticle();

	// タイトルロゴの初期化処理
	InitTitleLogo();

	// タイトルメニューの初期化
	InitTitleMenu();

	// 難易度選択の初期化処理
	InitSelectLevel();

	PlaySound(SOUND_LABEL_BGM_TITLE000);
}

//====================================
//	タイトルの終了処理
//====================================
void UninitTitle(void)
{
	// タイトル背景の終了処理
	UninitTitleBG();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// タイトルロゴの終了処理
	UninitTitleLogo();

	// タイトルメニューの終了処理
	UninitTitleMenu();

	// 難易度選択の終了処理
	UninitSelectLevel();

	// サウンドを止める
	StopSound();
}

//====================================
//	タイトルの描画処理
//====================================
void DrawTitle(void)
{
	// タイトル背景の描画処理
	DrawTitleBG();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// タイトルロゴの描画処理
	DrawTitleLogo();

	if (g_titleState == TITLESTATE_MENU)
	{
		// タイトルメニューの描画処理
		DrawTitleMenu();
	}
	else if (g_titleState == TITLESTATE_SELECTLEVEL)
	{
		DrawSelectLevel();
	}
}

//====================================
//	タイトルの更新処理
//====================================
void UpdateTitle(void)
{

	// タイトル背景の更新処理
	UpdateTitleBG();

	// エフェクトの更新処理
	UpdateEffect();

	// パーティクルの更新処理
	UpdateParticle();

	// タイトルロゴの更新処理
	UpdateTitleLogo();

	if (g_titleState == TITLESTATE_MENU)
	{
		// タイトルメニューの更新処理
		UpdateTitleMenu();
	}
	else if (g_titleState == TITLESTATE_SELECTLEVEL)
	{
		UpdateSelectLevel();
	}
}

//========================================
//	タイトルの状態設定
//========================================
void SetTitleState(TITLESTATE titlestate)
{
	g_titleState = titlestate;
}

//========================================
//	タイトルの状態取得
//========================================
TITLESTATE GetTitleState(void)
{
	return g_titleState;
}