//=============================================================================
//
//	レーダーの処理 [radar.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "radar.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RADAR_RADIUS	(256.0f)		// レーダーの半径

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRadar = NULL;						// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRadar = NULL;					// 頂点バッファへのポインタ
D3DXVECTOR3 g_radarpos = {};									// レーダーの位置
float g_fLengthRadar = NULL;
float g_fAngleRadar = NULL;

//====================================
//	レーダーの初期化処理
//====================================
void InitRadar(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntRadar = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\radar000.png",
		&g_pTextureRadar);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRadar,
		NULL);

	Player* pPlayer = GetPlayer();

	g_fLengthRadar = SQRTF(RADAR_RADIUS, RADAR_RADIUS);
	g_fAngleRadar = atan2f(RADAR_RADIUS, RADAR_RADIUS);

	g_radarpos = pPlayer->pos;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRadar->Lock(0, 0, (void * *)&pVtx, 0);

	pVtx[0].pos.x = g_radarpos.x + sinf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.y = g_radarpos.y + cosf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_radarpos.x + sinf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.y = g_radarpos.y + cosf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_radarpos.x + sinf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.y = g_radarpos.y + cosf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_radarpos.x + sinf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.y = g_radarpos.y + cosf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRadar->Unlock();
}

//=============================================================================
//	レーダーの終了処理
//=============================================================================
void UninitRadar(void)
{
	// テクスチャの破棄
	if (g_pTextureRadar != NULL)
	{
		g_pTextureRadar->Release();
		g_pTextureRadar = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRadar != NULL)
	{
		g_pVtxBuffRadar->Release();
		g_pVtxBuffRadar = NULL;
	}
}

//=============================================================================
//	レーダーの描画処理
//=============================================================================
void DrawRadar(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Player* pPlayer = GetPlayer();

	if (pPlayer->state != PLAYERSTATE_WAIT)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffRadar, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRadar);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,  0, 2);

	}
}

//=============================================================================
//	レーダーの更新処理
//=============================================================================
void UpdateRadar(void)
{
	Player* pPlayer = GetPlayer();				// プレイヤーの情報を取得

	D3DXVECTOR3* pCameraPos = GetCamera();		// カメラの情報を取得

	VERTEX_2D* pVtx;							// 頂点情報へのポインタ

	g_radarpos = pPlayer->pos;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRadar->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffRadar->Unlock();
}

