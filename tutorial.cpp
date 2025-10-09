//=============================================================================
//
//	チュートリアル画面処理 [result.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "tutorial.h"
#include "tutorialmenu.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "wave.h"
#include "bullet.h"
#include "score.h"
#include "camera.h"
#include "powerup.h"
#include "option.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TUTORIAL			(2)			// リザルトテクスチャの枚数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;		// 頂点バッファへのポインタ
TUTORIAL g_tutorial[MAX_TUTORIAL] = {};
int g_nTutorialTimer = 0;

//====================================
//	リザルトの初期化処理
//====================================
void InitTutorial(void)
{
	g_nTutorialTimer = 180;
}

//====================================
//	リザルトの終了処理
//====================================
void UninitTutorial(void)
{

}

//====================================
//	リザルトの描画処理
//====================================
void DrawTutorial(void)
{

}

//====================================
//	リザルトの更新処理
//====================================
void UpdateTutorial(void)
{
	Player* pPlayer = GetPlayer();

	int* pNumEnemy = GetNumEnemy();		// 敵の数を取得

	int nUseOption = GetUseOption();

	bool bSpeedUp = GetPowerUp();

	int nScore = GetScore();

	TUTORIALEVENT tutorialevent = GetTutorialEvent();

	if ((pPlayer->move.x > 0 || pPlayer->move.y > 0) && tutorialevent == TUTORIALEVENT_MOVE)
	{
		g_nTutorialTimer--;
		if (g_nTutorialTimer <= 0)
		{
			SetTutorialEvent(TUTORIALEVENT_ENEMY);
			g_nTutorialTimer = 60;
		}
	}

	if (*pNumEnemy <= 0 && tutorialevent == TUTORIALEVENT_ENEMY)
	{
		g_nTutorialTimer--;
		if (g_nTutorialTimer <= 0)
		{
			SetTutorialEvent(TUTORIALEVENT_POWERUP);
			g_nTutorialTimer = 60;
		}
	}

	if (tutorialevent == TUTORIALEVENT_POWERUP)
	{
		if (nScore < SPEEDUP_LOWESTLINE)
		{
			AddScore(1000);
		}

		if (bSpeedUp == true)
		{
			g_nTutorialTimer--;
		}

		if (nUseOption > 0 && g_nTutorialTimer <= 0)
		{
			SetTutorialEvent(TUTORIALEVENT_END);
			g_nTutorialTimer = 300;
		}
	}

	if (tutorialevent == TUTORIALEVENT_END)
	{
		g_nTutorialTimer--;
		if (g_nTutorialTimer <= 0)
		{
			SetScore(INIT_SCORE);
			SetGameState(GAMESTATE_NORMAL, 0);
		}
	}
}