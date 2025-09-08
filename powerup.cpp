//=============================================================================
//
//	パワーアップ処理 [powerup.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "powerup.h"
#include "input.h"
#include "effect.h"
#include "option.h"
#include "player.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POWERUP_LOWESTLINE		(2500)		// パワーアップするために必要なスコアの下限
#define OPTION_COST				(1500)		// オプションを生成するために必要なスコア
#define SPEEDUP_COST			(1000)		// 速度を上げるために必要なスコア

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//====================================
//	パワーアップの初期化処理
//====================================
void InitPowerup(void)
{

}

//====================================
//	パワーアップの終了処理
//====================================
void UninitPowerup(void)
{

}

//====================================
//	パワーアップの更新処理
//====================================
void UpdatePowerup(void)
{
	D3DXVECTOR3* pOptionStandardRot = GetStandardRot();
	Player* pPlayer = GetPlayer();
	int nUseOption = GetUseOption();

	if (GetJoypadTrigger(JOYKEY_X) == true || GetKeyboardTrigger(DIK_O) == true)
	{
		if (GetScore() >= POWERUP_LOWESTLINE && nUseOption < MAX_OPTION)
		{
			SetOption(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, 0.0f), 64.0f, (pOptionStandardRot->z) * D3DX_PI);
			SubScore(OPTION_COST);
		}
	}

	if (GetJoypadTrigger(JOYKEY_B) == true || GetKeyboardTrigger(DIK_L) == true)
	{
		if (GetScore() >= POWERUP_LOWESTLINE)
		{
			pPlayer->fSpeed += 0.1f;
			SubScore(SPEEDUP_COST);
		}
	}

}

//====================================
//	パワーアップの描画処理
//====================================
void DrawPowerup(void)
{

}