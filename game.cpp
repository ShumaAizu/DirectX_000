//=============================================================================
//
//	ゲーム画面処理 [game.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "game.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "option.h"
#include "time.h"
#include "life.h"
#include "stock.h"
#include "pause.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMEEND g_gameend = GAMEEND_MAX;			// 終了条件の情報
bool g_bGameend = false;					// 終了するかどうか
bool g_bPause = false;						// ポーズ中かどうか

//========================================
//	ゲーム画面の初期化処理
//========================================
void InitGame(void)
{
	// 背景の初期化処理
	InitBg();

	// プレイヤーの初期化処理
	InitPlayer();

	// 弾の初期化処理
	InitBullet();

	// 爆発の初期化処理
	InitExplosion();

	// 敵の初期化処理
	InitEnemy();

	// 敵のセット処理
	srand((unsigned)timeGetTime());

	// スコアの初期化処理
	InitScore();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// オプションの初期化処理
	InitOption();

	// 制限時間の初期化
	InitTime();

	// ライフの初期化
	InitLife();

	// 残機の初期化
	InitStock();

	// ポーズメニューの初期化
	InitPause();

	// 終了条件を初期化
	g_bGameend = false;

	// ポーズ状態の初期化
	g_bPause = false;

	// サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);
}

//========================================
//	ゲーム画面の終了処理
//========================================
void UninitGame(void)
{
	// 背景の終了処理
	UninitBg();

	// プレイヤーの終了処理
	UninitPlayer();

	// 弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	// 敵の終了処理
	UninitEnemy();

	// スコアの終了処理
	UninitScore();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// オプションの終了処理
	UninitOption();

	// 制限時間の終了処理
	UninitTime();

	// ライフの終了処理
	UninitLife();

	// 残機の終了処理
	UninitStock();

	// ポーズメニューの終了処理
	UninitPause();

	// サウンドを止める
	StopSound();
}

//========================================
//	ゲーム画面の更新処理
//========================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
	{// ポーズキーが押された
		g_bPause = g_bPause ? false : true;		// ポーズ状態を切り替える
	}

	if (g_bPause == true)
	{// ポーズ中なら
		// ポーズの更新処理
		UpdatePause();
	}
	else
	{
		// 背景の更新処理
		UpdateBg();

		// プレイヤーの更新処理
		UpdatePlayer();

		// 弾の更新処理
		UpdateBullet();

		// 爆発の更新処理
		UpdateExplosion();

		// 敵の更新処理
		UpdateEnemy();

		// スコアの更新処理
		UpdateScore();

		// エフェクトの更新処理
		UpdateEffect();

		// パーティクルの更新処理
		UpdateParticle();

		// オプションの更新処理
		UpdateOption();

		// 制限時間の更新処理
		UpdateTime();

		// ライフの更新処理
		UpdateLife();

		// 残機の更新処理
		UpdateStock();

		// プレイヤーの情報を取得
		Player* pPlayer = GetPlayer();

		static int nGameEndCounter = 0;

		if (CheckEnemy() == false)
		{
			g_bGameend = true;
			g_gameend = GAMEEND_CLEAR;
		}
		else if (pPlayer->state == PLAYERSTATE_DEATH || CheckTime() == false)
		{
			g_bGameend = true;
			g_gameend = GAMEEND_GAMEOVER;
		}

		// ゲーム終了条件
		if (g_bGameend == true)
		{
			nGameEndCounter++;
			if (nGameEndCounter % 60 == 0)
			{
				nGameEndCounter = 0;
				// モード設定(リザルト画面に移行)
				SetFade(MODE_RESULT);
			}
		}
	}
}

//========================================
//	ゲーム画面の描画処理
//========================================
void DrawGame(void)
{
	// 背景の描画処理
	DrawBg();

	// 弾の描画処理
	DrawBullet();

	// エフェクトの描画処理
	DrawEffect();

	// プレイヤーの描画処理
	DrawPlayer();

	// 爆発の描画処理
	DrawExplosion();

	// 敵の描画処理
	DrawEnemy();

	// スコアの描画処理
	DrawScore();

	// オプションの描画処理
	DrawOption();

	// 制限時間の描画処理
	DrawTime();

	// ライフの描画処理
	DrawLife();

	// 残機の描画処理
	DrawStock();

	if (g_bPause == true)
	{// ポーズ中なら
		// ポーズメニューの描画処理
		DrawPause();
	}
}

//========================================
//	ゲーム終了条件の取得
//========================================
GAMEEND GetGameEnd(void)
{
	return g_gameend;
}

//========================================
//	ポーズの有効無効設定
//========================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
