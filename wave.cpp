//=============================================================================
//
//	ウェーブ処理 [wave.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wave.h"
#include "input.h"
#include "game.h"
#include "enemy.h"
#include "bossparts.h"
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
#define MAX_WARD		(256)		// 文字列の最大
#define WAVE_TIME		(900)		// ウェーブの秒数(フレーム)
#define MAX_WAVE		(4)			// ウェーブの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWave = NULL;					// 頂点バッファへのポインタ
int g_nTimeLine = 0;
int g_nWave = 0;

//====================================
//	敵の初期化処理
//====================================
void InitWave(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTimeLine = 0;

	g_nWave = 0;

	LoadWave(g_nWave);
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
	int* pNumEnemy = GetNumEnemy();		// 敵の数を取得

	Enemy* pEnemy = GetEnemy();

	BossParts* pBossParts = GetBossParts();

	if (GetGameState() == GAMESTATE_TUTORIAL && GetTutorialEvent() != TUTORIALEVENT_ENEMY)
	{
		return;
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->nTimeLine == g_nTimeLine)
		{// タイムラインが重なったら
			pEnemy->bUse = true;		// 出現
		}
	}

	for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++, pBossParts++)
	{
		if (pBossParts->nTimeLine == g_nTimeLine)
		{// タイムラインが重なっていたら出現
			PlaySound(SOUND_LABEL_SE_APPEAR000);
			pBossParts->bUse = true;
		}
	}

	if (GetGameState() == GAMESTATE_TUTORIAL)
	{
		return;
	}

	g_nTimeLine++;			// タイムラインを進める

	while (true)
	{

		if (*pNumEnemy != 0 && CheckEnemy() == false)
		{// ウェーブの敵が残っているかつ盤面に敵がいない
			pEnemy = GetEnemy();
			pBossParts = GetBossParts();
			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->nTimeLine == g_nTimeLine)
				{// タイムラインが重なっていたら出現
					PlaySound(SOUND_LABEL_SE_APPEAR000);
					pEnemy->bUse = true;
				}
			}

			for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++, pBossParts++)
			{
				if (pBossParts->nTimeLine == g_nTimeLine)
				{// タイムラインが重なっていたら出現
					PlaySound(SOUND_LABEL_SE_APPEAR000);
					pBossParts->bUse = true;
				}
			}
		}

		if (*pNumEnemy == 0 || CheckEnemy() == true)
		{// ウェーブの敵がいなくなった / 敵が出てきた
			break;
		}

		// ループ中にタイムラインを進める
		g_nTimeLine++;
	}


	if (g_nTimeLine >= WAVE_TIME || *pNumEnemy == 0)
	{
		g_nWave++;
		g_nTimeLine = 0;
		if (g_nWave < MAX_WAVE)
		{
			LoadWave(g_nWave);
		}
	}

	if (((*pNumEnemy <= 0 && g_nWave > MAX_WAVE) || GetTime() <= 0 || GetScore() <= 0) && GetGameState() != GAMESTATE_END)
	{// 敵がいなくなった / タイムがなくなった / スコアがなくなった
		SetGameState(GAMESTATE_END, 60);
	}
}

//=============================================================================
//	敵のロード処理
//=============================================================================
void LoadWave(int nWave)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	int* pNumEnemy = GetNumEnemy();

	const char* pWaveFileNameNormal[MAX_WAVE] =
	{
		"data\\WAVE\\wave_normal000.txt",
		"data\\WAVE\\wave_normal001.txt",
		"data\\WAVE\\wave_normal002.txt",
		"data\\WAVE\\wave_normal003.txt"
	};

	const char* pWaveFileNameHard[MAX_WAVE] =
	{
		"data\\WAVE\\wave_hard000.txt",
		"data\\WAVE\\wave_hard001.txt",
		"data\\WAVE\\wave_hard002.txt",
		"data\\WAVE\\wave_hard003.txt"
	};

	if (GetGameState() == GAMESTATE_TUTORIAL)
	{
		pFile = fopen("data\\WAVE\\wave_tutorial.txt", "r");
		g_nWave--;
	}
	else if (GetGameMode() == GAMEMODE_NORMAL)
	{
		pFile = fopen(pWaveFileNameNormal[nWave], "r");
	}
	else
	{
		pFile = fopen(pWaveFileNameHard[nWave], "r");
	}

	if (pFile == NULL)
	{// 開けなかったら
		return;
	}

	//ローカル変数宣言
	char aString[MAX_WARD];		// 文字列を読み込む
	char aStrRelease[3];		// 不要な = を読み込む

	// それぞれの値を読み込む
	int nNumEnemy = {};
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 rot = {};
	float fMove = 0;
	float fRadius = 0;
	int type = 0;
	int life = 0;
	int timeline = 0;
	int nScore = 0;
	int nData;

	while (true)
	{
		nData = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "NUM_ENEMY") == 0)
		{
			fscanf(pFile, "%s", &aStrRelease[0]);

			fscanf(pFile, "%d", &nNumEnemy);

			*pNumEnemy += nNumEnemy;
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

				if (strcmp(&aString[0], "ROT") == 0)
				{// ROTを読み取った
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &rot.z);
				}

				if (strcmp(&aString[0], "RADIUS") == 0)
				{// RADIUSを読み取った
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &fRadius);
				}

				if (strcmp(&aString[0], "MOVE") == 0)
				{// MOVEを読み取った
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &fMove);
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

				if (strcmp(&aString[0], "SCORE") == 0)
				{// SCOREを読み取った
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%d", &nScore);
				}

				if (strcmp(&aString[0], "ENDSET") == 0)
				{// ENDSETを読み取った
					SetEnemy(pos, rot, fRadius, fMove, type, life, timeline, nScore);
					break;
				}

				if (strcmp(&aString[0], "BOSSPARTSENDSET") == 0)
				{// ENDSETを読み取った
					SetBossParts(pos, rot, fRadius, fMove, type, life, timeline, nScore);
					break;
				}
			}
		}

		if (strcmp(&aString[0], "END_SCRIPT") == 0)
		{
			break;
		}
	}

	fclose(pFile);
}