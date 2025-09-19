//=============================================================================
//
//	背景処理 [bg.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "camera.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BG		(3)								// 多重背景の最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBg[MAX_BG] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;		// 頂点バッファへのポインタ
Bg g_abg[MAX_BG];

//=============================================================================
//	背景の初期化処理
//=============================================================================
void InitBg(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG000.jpg",
		&g_apTextureBg[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_apTextureBg[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_apTextureBg[2]);


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	// 初期化

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		g_abg[nCntBg].pos = D3DXVECTOR3(0.0f, 50.0f * nCntBg, 0.0f);
		g_abg[nCntBg].move = D3DXVECTOR3(0.0f, 1.0f + nCntBg, 0.0f);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x, g_abg[nCntBg].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x + WARLD_WIDTH, g_abg[nCntBg].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x, g_abg[nCntBg].pos.x + WARLD_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x + WARLD_WIDTH, g_abg[nCntBg].pos.x + WARLD_HEIGHT, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
//	背景の終了処理
//=============================================================================
void UninitBg(void)
{
	// テクスチャの破棄
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (g_apTextureBg[nCntBg] != NULL)
		{
			g_apTextureBg[nCntBg]->Release();
			g_apTextureBg[nCntBg] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
//	背景の描画処理
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBg = 0; nCntBg < 1; nCntBg++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBg[nCntBg]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//=============================================================================
//	背景の更新処理
//=============================================================================
void UpdateBg(void)
{
	int nCntBg;
	
	//for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	//{
	//	g_abg[nCntBg].pos += g_abg[nCntBg].move;

	//	if (g_abg[nCntBg].pos.y >= SCREEN_HEIGHT)
	//	{
	//		g_abg[nCntBg].pos.y = 0.0f - 960;
	//	}
	//}

	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x, g_abg[nCntBg].pos.y - pCameraPos->y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x + WARLD_WIDTH, g_abg[nCntBg].pos.y - pCameraPos->y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x, g_abg[nCntBg].pos.y - pCameraPos->y + WARLD_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x + WARLD_WIDTH, g_abg[nCntBg].pos.y - pCameraPos->y + WARLD_HEIGHT, 0.0f);

		pVtx += 4;
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}