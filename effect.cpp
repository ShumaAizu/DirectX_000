//=============================================================================
//
//	エフェクト処理 [effect.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "effect.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EFFECT		(12288)			// エフェクトの最大数

// エフェクト構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fRadius;			// 半径
	int nLife;				// 寿命
	EFFECTSTATE state;		// 状態
	bool bUse;				// 使用状況
}Effect;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		// 頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];							// エフェクトの情報

//====================================
//	エフェクトの初期化処理
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ
	int nCntEffect = 0;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	// 初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 30;
		g_aEffect[nCntEffect].fRadius = 25;
		g_aEffect[nCntEffect].state = EFFECTSTATE_NORMAL;
		g_aEffect[nCntEffect].bUse = false;			// 使用していない状態にする
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,		// エフェクトの数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点情報の設定
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//====================================
//	エフェクトの終了処理
//====================================
void UninitEffect(void)
{
	// テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//====================================
//	エフェクトの描画処理
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 使用判定
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{// 弾が使用されている
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================
//	エフェクトの更新処理
//====================================
void UpdateEffect(void)
{
	int nCntEffect = 0;

	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{// エフェクトが使用されている

			switch (g_aEffect[nCntEffect].state)
			{
			case EFFECTSTATE_NORMAL:
				g_aEffect[nCntEffect].fRadius -= 5.25f;	// 大きさを小さくする
				break;

			case EFFECTSTATE_COLLECT:

				Player* pPlayer = GetPlayer();
				float fAngle = atan2f(pPlayer->pos.x - g_aEffect[nCntEffect].pos.x, pPlayer->pos.y - g_aEffect[nCntEffect].pos.y);

				g_aEffect[nCntEffect].move.x = sinf(fAngle) * 10.0f;
				g_aEffect[nCntEffect].move.y = cosf(fAngle) * 10.0f;

				if (g_aEffect[nCntEffect].pos == pPlayer->pos)
				{
					g_aEffect[nCntEffect].bUse == false;
				}

				break;
			}

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{// 大きさが0以下になったら
				g_aEffect[nCntEffect].fRadius = 0;		// 0で固定
			}

			g_aEffect[nCntEffect].col.a -= 0.025f;		// 透明にしていく

			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x - g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x + g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x - g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x + g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y + g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].nLife--;				// 寿命を削る

			if (g_aEffect[nCntEffect].nLife < 0)
			{//もし寿命が尽きたら
				g_aEffect[nCntEffect].bUse = false;		// エフェクトを使用していない状態にする
				g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			
		}

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//====================================
//	エフェクトの設定
//====================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{// エフェクトを使用していない
			g_aEffect[nCntEffect].pos = pos;			// 受け取った位置を代入
			g_aEffect[nCntEffect].move = move;			// 受け取った移動量を代入
			g_aEffect[nCntEffect].col = col;			// 受け取った色を代入
			g_aEffect[nCntEffect].nLife = nLife;		// 受け取った寿命を代入
			g_aEffect[nCntEffect].fRadius = fRadius;	// 受け取った半径を代入
			g_aEffect[nCntEffect].state = EFFECTSTATE_NORMAL;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);

			g_aEffect[nCntEffect].bUse = true;			// エフェクトが使用されている状態にする
			break;		// ここでfor文を抜ける
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();

}

//====================================
//	集まるパーティクルの設定
//====================================
void SetCollectParticle(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCount = 0;
	float fMove;

	VERTEX_2D* pVtx = NULL;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++, pVtx += 4)
	{
		if (g_aEffect[nCntParticle].bUse == true)
		{
			continue;
		}

		// ランダムな位置をセット
		g_aEffect[nCntParticle].pos.x = pos.x + (float)(rand() % 250 - 100);
		g_aEffect[nCntParticle].pos.y = pos.y + (float)(rand() % 250 - 100);

		// ランダムな半径をセット
		g_aEffect[nCntParticle].fRadius = (float)(rand() % 25 + 11);

		// ランダムな寿命をセット
		g_aEffect[nCntParticle].nLife = 50;

		// ランダムな移動量をセット
		fMove = 25.0f;

		g_aEffect[nCntParticle].state = EFFECTSTATE_COLLECT;

		g_aEffect[nCntParticle].col = col;

		float fAngle = atan2f(pos.x - g_aEffect[nCntParticle].pos.x, pos.y - g_aEffect[nCntParticle].pos.y);

		g_aEffect[nCntParticle].move.x = sinf(fAngle) * fMove;
		g_aEffect[nCntParticle].move.y = cosf(fAngle) * fMove;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x - g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y - g_aEffect[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x + g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y - g_aEffect[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x - g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y + g_aEffect[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x + g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y + g_aEffect[nCntParticle].fRadius, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntParticle].col;
		pVtx[1].col = g_aEffect[nCntParticle].col;
		pVtx[2].col = g_aEffect[nCntParticle].col;
		pVtx[3].col = g_aEffect[nCntParticle].col;

		g_aEffect[nCntParticle].bUse = true;			// エフェクトが使用されている状態にする

		nCount++;
		if (nCount > 5)
		{
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}