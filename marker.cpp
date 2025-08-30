//=============================================================================
//
//	マーカー処理 [marker.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "marker.h"
#include "enemy.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MARKER		(MAX_ENEMY)							// マーカーの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureMarker = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMarker = NULL;	// 頂点バッファへのポインタ
Marker g_amarker[MAX_MARKER] = {};

//=============================================================================
//	マーカーの初期化処理
//=============================================================================
void InitMarker(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\option000.png",
		&g_apTextureMarker);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MARKER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMarker,
		NULL);

	// 初期化

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffMarker->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		g_amarker[nCntMarker].pos = D3DXVECTOR3(0.0f, 50.0f * nCntMarker, 0.0f);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.x + SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.x + SCREEN_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMarker->Unlock();
}

//=============================================================================
//	マーカーの終了処理
//=============================================================================
void UninitMarker(void)
{
	// テクスチャの破棄
	if (g_apTextureMarker != NULL)
	{
		g_apTextureMarker->Release();
		g_apTextureMarker = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMarker != NULL)
	{
		g_pVtxBuffMarker->Release();
		g_pVtxBuffMarker = NULL;
	}
}

//=============================================================================
//	マーカーの描画処理
//=============================================================================
void DrawMarker(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMarker, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMarker);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMarker * 4, 2);
	}
}

//=============================================================================
//	マーカーの更新処理
//=============================================================================
void UpdateMarker(void)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	Enemy* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			continue;
		}


	}

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		if (g_amarker[nCntMarker].bUse == true)
		{

		}

		pVtx += 4;
	}

	//for (nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	//{
	//	// 頂点座標の設定
	//	pVtx[0].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.y, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.y, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.y + SCREEN_HEIGHT, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.y + SCREEN_HEIGHT, 0.0f);

	//	pVtx += 4;
	//}


	// 頂点バッファをアンロックする
	g_pVtxBuffMarker->Unlock();
}