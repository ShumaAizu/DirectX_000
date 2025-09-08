//=============================================================================
//
//	ウェーブ処理 [wave.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wave.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "time.h"
#include "sound.h"
#include "life.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_WARD		(256)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWave = NULL;					// 頂点バッファへのポインタ
int g_nTimeLine = 0;

//====================================
//	敵の初期化処理
//====================================
void InitWave(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTimeLine = 0;

	LoadWave();
}

//=============================================================================
//	敵の終了処理
//=============================================================================
void UninitWave(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffWave != NULL)
	{
		g_pVtxBuffWave->Release();
		g_pVtxBuffWave = NULL;
	}
}

//=============================================================================
//	敵の描画処理
//=============================================================================
void DrawWave(void)
{

}

//=============================================================================
//	敵の更新処理
//=============================================================================
void UpdateWave(void)
{
	Enemy* pEnemy = GetEnemy();
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->nTimeLine == g_nTimeLine)
		{
			pEnemy->bUse = true;
		}
	}

	g_nTimeLine++;

	int* pNumEnemy = GetNumEnemy();		// 敵の数を取得

	if ((*pNumEnemy <= 0 || GetTime() <= 0 || GetScore() <= 0) && GetGameState() != GAMESTATE_END)
	{// 敵がいなくなった/タイムがなくなった/スコアがなくなった
		SetGameState(GAMESTATE_END, 60);
	}
}

//=============================================================================
//	敵のロード処理
//=============================================================================
void LoadWave(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	int* pNumEnemy = GetNumEnemy();

	pFile = fopen("data\\wave.txt", "r");

	if (pFile != NULL)
	{// 開けたら
		//ローカル変数宣言
		char aString[MAX_WARD];		// 文字列を読み込む
		char aStrRelease[3];		// 不要な = を読み込む

		// それぞれの値を読み込む
		D3DXVECTOR3 pos = {};
		D3DXVECTOR3 move = {};
		int type = 0;
		int life = 0;
		int timeline = 0;
		int nData;

		while (true)
		{
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM_ENEMY") == 0)
			{
				fscanf(pFile, "%s", &aStrRelease[0]);

				fscanf(pFile, "%d", pNumEnemy);
			}

			if (strcmp(&aString[0], "SETENEMY") == 0)
			{// SETENEMYを読み取った
				while (true)
				{
					nData = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{// POSを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}

					if (strcmp(&aString[0], "MOVE") == 0)
					{// POSを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%f", &move.x);
						fscanf(pFile, "%f", &move.y);
						fscanf(pFile, "%f", &move.z);
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{// TYPEを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &type);

					}

					if (strcmp(&aString[0], "LIFE") == 0)
					{// LIFEを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &life);
					}

					if (strcmp(&aString[0], "TIMELINE") == 0)
					{// LIFEを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &timeline);
					}

					if (strcmp(&aString[0], "ENDSET") == 0)
					{// ENDSETを読み取った
						SetEnemy(pos, move, type, life, timeline);
						break;
					}
				}
			}

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}