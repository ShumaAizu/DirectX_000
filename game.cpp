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
#include "camera.h"
#include "frame.h"
#include "powerup.h"
#include "wave.h"
#include "marker.h"
#include "radar.h"
#include "ranking.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
GAMEEND g_gameend = GAMEEND_MAX;			// 終了条件の情報
int g_nCounterGameState = 0;				// 状態管理カウンター
bool g_bGameend = false;					// 終了するかどうか
bool g_bPause = false;						// ポーズ中かどうか

//========================================
//	ゲーム画面の初期化処理
//========================================
void InitGame(void)
{
	// カメラの初期化処理
	InitCamera();

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

	// ウェーブの初期化処理
	InitWave();

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

	// マーカーの初期化
	InitMarker();

	// フレームの初期化
	InitFrame();

	// ポーズメニューの初期化
	InitPause();

	// パワーアップの初期化
	InitPowerup();

	// レーダーの初期化
	InitRadar();

	g_gameState = GAMESTATE_NORMAL;

	// 終了条件を初期化
	g_bGameend = false;

	// ポーズ状態の初期化
	g_bPause = false;

	// サウンドの再生
	PlaySound(SOUND_LABEL_BGM_GAME000);
}

//========================================
//	ゲーム画面の終了処理
//========================================
void UninitGame(void)
{
	// ウェーブの終了処理
	UninitWave();

	// カメラの終了処理
	UninitCamera();

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

	UninitPowerup();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// オプションの終了処理
	UninitOption();

	// 制限時間の終了処理
	UninitTime();

	// マーカーの終了処理
	UninitMarker();

	// レーダーの終了処理
	UninitRadar();

	// フレームの終了処理
	UninitFrame();

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

	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && GetFade() != FADE_OUT)
	{// ポーズキーが押された
		PlaySound(SOUND_LABEL_SE_PAUSE000);
		g_bPause = g_bPause ? false : true;		// ポーズ状態を切り替える
		SetPauseMenu(PAUSE_MENU_CONTINUE);
	}

	if (g_bPause == true)
	{// ポーズ中なら
		// ポーズの更新処理
		UpdatePause();
	}
	else if(GetFade() != FADE_OUT)
	{

		// スコアの更新処理
		UpdateScore();

		// ウェーブの更新処理
		UpdateWave();

		// カメラの更新処理
		UpdateCamera();

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

		// エフェクトの更新処理
		UpdateEffect();

		// パーティクルの更新処理
		UpdateParticle();

		// オプションの更新処理
		UpdateOption();

		// 制限時間の更新処理
		UpdateTime();

		// パワーアップの更新処理
		UpdatePowerup();

		// マーカーの更新処理
		UpdateMarker();

		// レーダーの更新処理
		UpdateRadar();

		// フレームの更新処理
		UpdateFrame();

		switch (g_gameState)
		{
		case GAMESTATE_END:
			g_nCounterGameState--;
			if (g_nCounterGameState <= 0)
			{
				// モード設定(リザルト画面に移行)
				g_gameend = GAMEEND_CLEAR;
				if (GetScore() > 0)
				{
					AddScore((GetTime() * 100));
				}
				SetRanking(GetScore());
				SetFade(MODE_RANKING, 0.025f, 0.025f);
			}
			break;
		}

	}
}

//========================================
//	ゲーム画面の描画処理
//========================================
void DrawGame(void)
{
	// ウェーブの描画処理
	DrawWave();

	// カメラの描画処理
	DrawCamera();

	// 背景の描画処理
	DrawBg();

	// 弾の描画処理
	DrawBullet();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// プレイヤーの描画処理
	DrawPlayer();

	// 爆発の描画処理
	DrawExplosion();

	// 敵の描画処理
	DrawEnemy();

	// オプションの描画処理
	DrawOption();

	// フレームの描画処理
	DrawFrame();

	// スコアの描画処理
	DrawScore();

	DrawPowerup();

	// 制限時間の描画処理
	DrawTime();

	// マーカーの描画処理
	DrawMarker();

	//// レーダーの描画処理
	//DrawRadar();

	if (g_bPause == true)
	{// ポーズ中なら
		// ポーズメニューの描画処理
		DrawPause();
	}
}

//========================================
//	ゲームの状態設定
//========================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

//========================================
//	ゲームの状態の取得
//========================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
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
