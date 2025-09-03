//=============================================================================
//
//	ウェーブ処理 [wave.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wave.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "life.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_ENEMY		(5)			// 敵の種類
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
}

void LoadWave(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	pFile = fopen("data\\wave.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	//ローカル変数宣言
	char aString[MAX_WARD] = {};
	char aStrRelease[3] = {};
	D3DXVECTOR3 pos = {};
	int type = 0;
	int life = 0;

	while (true)
	{
		(void)fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "SETENEMY") == 0)
		{ // SETENEMYを読み取った
			while (true)
			{
				(void)fscanf(pFile, "%s", &aString[0]);

				if (strcmp(&aString[0], "POS") == 0)
				{ // POSを読み取った
					(void)fscanf(pFile, "%s", &aStrRelease[0]);

					(void)fscanf(pFile, "%f", &pos.x);
					(void)fscanf(pFile, "%f", &pos.y);
					(void)fscanf(pFile, "%f", &pos.z);
				}

				if (strcmp(&aString[0], "TYPE") == 0)
				{
					(void)fscanf(pFile, "%s", &aStrRelease[0]);

					(void)fscanf(pFile, "%d", &type);

				}

				if (strcmp(&aString[0], "LIFE") == 0)
				{
					(void)fscanf(pFile, "%s", &aStrRelease[0]);

					(void)fscanf(pFile, "%d", &life);
				}

				if (strcmp(&aString[0], "ENDSET") == 0)
				{
					//SetEnemy(pos, type, life);
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