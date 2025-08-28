//=============================================================================
//
//	パーティクル処理 [particle.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "particle.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTICLE		(128)		// パーティクルの最大数

//*****************************************************************************
// パーティクル構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fRadius;			// 半径
	int nLife;				// 寿命
	bool bUse;				// 使用状況
}Particle;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Particle g_aParticle[MAX_PARTICLE];			// パーティクルの情報

void InitParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = {};
		g_aParticle[nCntParticle].col = {};
		g_aParticle[nCntParticle].fRadius = {};
		g_aParticle[nCntParticle].nLife = {};
		g_aParticle[nCntParticle].bUse = false;
	}
}

void UninitParticle(void)
{

}

void UpdateParticle(void)
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// パーティクルの移動量
	D3DXCOLOR col;			// 色
	float fMove;			// 移動量
	float fRadius;			// 半径
	int nLife;				// 寿命

	float fAngle;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			for (int nCntAppear = 0; nCntAppear < 5; nCntAppear++)
			{
				pos = g_aParticle[nCntParticle].pos;

				fAngle = (float)(rand() % 629 - 314) / 100.0f;
				fMove = (float)(rand() % 500) / 100.0f + 0.1;

				move.x = sinf(fAngle) * fMove;
				move.y = cosf(fAngle) * fMove;

				col = g_aParticle[nCntParticle].col;
				fRadius = g_aParticle[nCntParticle].fRadius;
				nLife = g_aParticle[nCntParticle].nLife;

				SetEffect(pos, move, col, fRadius, nLife);
			}

			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}

}

void DrawParticle(void)
{

}

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{// エフェクトを使用していない
			g_aParticle[nCntParticle].pos = pos;			// 受け取った位置を代入
			g_aParticle[nCntParticle].col = col;			// 受け取った色を代入
			g_aParticle[nCntParticle].nLife = nLife;		// 受け取った寿命を代入
			g_aParticle[nCntParticle].fRadius = fRadius;	// 受け取った半径を代入

			g_aParticle[nCntParticle].bUse = true;			// エフェクトが使用されている状態にする
			break;		// ここでfor文を抜ける
		}
	}
}