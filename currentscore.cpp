//=============================================================================
//
//	現在スコア処理 [currentscore.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "score.h"
#include "currentscore.h"
#include "ranking.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCurrentScore[2] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCurrentScore = NULL;			// 頂点バッファへのポインタ
D3DXVECTOR3 g_CurrentScore_pos = {};
D3DXVECTOR3 g_CurrentScoreInfo_pos = {};

//=============================================================================
//	スコアの初期化処理
//=============================================================================
void InitCurrentScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureCurrentScore[0]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM + 1,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCurrentScore,
		NULL);

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffCurrentScore->Lock(0, 0, (void**)&pVtx, 0);

		int aTexU[MAX_NUM] = {};

		int nScore = GetScore();

		g_CurrentScore_pos = D3DXVECTOR3(RANKING_POSX, RANKING_POSY + (MAX_RANK * (RANKING_SIZEY + RANKING_DIFF)), 0.0f);

		aTexU[0] = nScore % 100000 / 10000;
		aTexU[1] = nScore % 10000 / 1000;
		aTexU[2] = nScore % 1000 / 100;
		aTexU[3] = nScore % 100 / 10;
		aTexU[4] = nScore % 10 / 1;

		for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * nCntNum), g_CurrentScore_pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_CurrentScore_pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * nCntNum), g_CurrentScore_pos.y + RANKING_SIZEY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_CurrentScore_pos.y + RANKING_SIZEY, 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 1.0f);

			pVtx += 4;
		}

	// 頂点バッファをアンロックする
	g_pVtxBuffCurrentScore->Unlock();
}

//====================================
//	スコアの終了処理
//====================================
void UninitCurrentScore(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		if (g_pTextureCurrentScore[nCntTex] != NULL)
		{
			g_pTextureCurrentScore[nCntTex]->Release();
			g_pTextureCurrentScore[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffCurrentScore != NULL)
	{
		g_pVtxBuffCurrentScore->Release();
		g_pVtxBuffCurrentScore = NULL;
	}
}

//====================================
//	スコアの描画処理
//====================================
void DrawCurrentScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCurrentScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < MAX_NUM + 1; nCntNumber++)
	{
		if (nCntNumber > MAX_NUM)
		{
			pDevice->SetTexture(0, g_pTextureCurrentScore[1]);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCurrentScore[0]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, 2);
	}
}

//====================================
//	スコアの更新処理
//====================================
void UpdateCurrentScore(void)
{

}