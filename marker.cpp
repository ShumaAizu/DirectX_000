//=============================================================================
//
//	マーカー処理 [marker.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "marker.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MARKER		(MAX_ENEMY)							// マーカーの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMarker = NULL;			// テクスチャへのポインタ
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
		"data\\TEXTURE\\Marker000.png",
		&g_pTextureMarker);

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
		g_amarker[nCntMarker].pos = {0.0f, 0.0f, 0.0f};
		g_amarker[nCntMarker].bUse = false;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x - MARKER_SIZEX, g_amarker[nCntMarker].pos.y - MARKER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + MARKER_SIZEX, g_amarker[nCntMarker].pos.y - MARKER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x - MARKER_SIZEX, g_amarker[nCntMarker].pos.y + MARKER_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + MARKER_SIZEX, g_amarker[nCntMarker].pos.y + MARKER_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

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
	if (g_pTextureMarker != NULL)
	{
		g_pTextureMarker->Release();
		g_pTextureMarker = NULL;
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
		if (g_amarker[nCntMarker].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMarker);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMarker * 4, 2);
		}
	}
}

//=============================================================================
//	マーカーの更新処理
//=============================================================================
void UpdateMarker(void)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	Player* pPlayer = GetPlayer();

	Enemy* pEnemy = GetEnemy();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			if (g_amarker[nCntEnemy].bUse == true)
			{
				g_amarker[nCntEnemy].bUse = false;
			}
			continue;
		}

		float fLength = NULL;			// 対角線の長さ

		fLength = SQRTF((pPlayer->pos.x - pEnemy->pos.x), (pPlayer->pos.y - pEnemy->pos.y));


		if (fLength > MARKER_DIFF)
		{
			float fAngle = NULL;

			fAngle = atan2f(pEnemy->pos.x - pPlayer->pos.x, pEnemy->pos.y - pPlayer->pos.y);

			g_amarker[nCntEnemy].pos.x = (SCREEN_WIDTH / 2) + sinf(fAngle) * MARKER_POS;
			g_amarker[nCntEnemy].pos.y = (SCREEN_HEIGHT / 2 ) + cosf(fAngle) * MARKER_POS;

			if (g_amarker[nCntEnemy].pos.y >= SCREEN_HEIGHT)
			{
				g_amarker[nCntEnemy].pos.y = SCREEN_HEIGHT;
			}

			if (g_amarker[nCntEnemy].pos.y <= 0)
			{
				g_amarker[nCntEnemy].pos.y = 0;
			}


			// 頂点座標の設定
			(pVtx + (nCntEnemy * 4))[0].pos = D3DXVECTOR3(g_amarker[nCntEnemy].pos.x - MARKER_SIZEX, g_amarker[nCntEnemy].pos.y - MARKER_SIZEY, 0.0f);
			(pVtx + (nCntEnemy * 4))[1].pos = D3DXVECTOR3(g_amarker[nCntEnemy].pos.x + MARKER_SIZEX, g_amarker[nCntEnemy].pos.y - MARKER_SIZEY, 0.0f);
			(pVtx + (nCntEnemy * 4))[2].pos = D3DXVECTOR3(g_amarker[nCntEnemy].pos.x - MARKER_SIZEX, g_amarker[nCntEnemy].pos.y + MARKER_SIZEY, 0.0f);
			(pVtx + (nCntEnemy * 4))[3].pos = D3DXVECTOR3(g_amarker[nCntEnemy].pos.x + MARKER_SIZEX, g_amarker[nCntEnemy].pos.y + MARKER_SIZEY, 0.0f);

			if (pEnemy->state == ENEMYSTATE_APPEAR)
			{
				(pVtx + (nCntEnemy * 4))[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				(pVtx + (nCntEnemy * 4))[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				(pVtx + (nCntEnemy * 4))[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				(pVtx + (nCntEnemy * 4))[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			}
			else if (fLength > MARKER_DIFF_LONG)
			{
				(pVtx + (nCntEnemy * 4))[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				(pVtx + (nCntEnemy * 4))[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				(pVtx + (nCntEnemy * 4))[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				(pVtx + (nCntEnemy * 4))[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (fLength > MARKER_DIFF_SHORT)
			{
				(pVtx + (nCntEnemy * 4))[0].col = D3DXCOLOR(1.0f, 0.5f, 0.15f, 1.0f);
				(pVtx + (nCntEnemy * 4))[1].col = D3DXCOLOR(1.0f, 0.5f, 0.15f, 1.0f);
				(pVtx + (nCntEnemy * 4))[2].col = D3DXCOLOR(1.0f, 0.5f, 0.15f, 1.0f);
				(pVtx + (nCntEnemy * 4))[3].col = D3DXCOLOR(1.0f, 0.5f, 0.15f, 1.0f);
			}
			else if (fLength > MARKER_DIFF)
			{
				(pVtx + (nCntEnemy * 4))[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				(pVtx + (nCntEnemy * 4))[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				(pVtx + (nCntEnemy * 4))[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				(pVtx + (nCntEnemy * 4))[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}

			g_amarker[nCntEnemy].bUse = true;
		}
		else
		{
			if (g_amarker[nCntEnemy].bUse == true)
			{
				g_amarker[nCntEnemy].bUse = false;
			}
		}
	}



	// 頂点バッファをアンロックする
	g_pVtxBuffMarker->Unlock();
}