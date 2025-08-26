//=============================================================================
//
//	オプションの処理 [option.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "option.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPTION_RADIUS	(16)		// オプションの半径
#define OPTION_COOLTIME	(60)		// オプションのクールタイム(ms)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureOption = NULL;						// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOption = NULL;				// 頂点バッファへのポインタ
Option g_aOption[MAX_OPTION];									// オプションの情報

//====================================
//	オプションの初期化処理
//====================================
void InitOption(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntOption = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\option000.png",
		&g_pTextureOption);

	// 初期化
	for (nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{
		g_aOption[nCntOption].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aOption[nCntOption].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aOption[nCntOption].distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aOption[nCntOption].state = OPTIONSTATE_NORMAL;
		g_aOption[nCntOption].bUse = false;			// 使用していない状態にする
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OPTION,		// オプションの数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOption,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffOption->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f);

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

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	オプションの終了処理
//=============================================================================
void UninitOption(void)
{
	// テクスチャの破棄
	if (g_pTextureOption != NULL)
	{
		g_pTextureOption->Release();
		g_pTextureOption = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffOption != NULL)
	{
		g_pVtxBuffOption->Release();
		g_pVtxBuffOption = NULL;
	}
}

//=============================================================================
//	オプションの描画処理
//=============================================================================
void DrawOption(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Player* pPlayer = GetPlayer();

	if (pPlayer->state != PLAYERSTATE_WAIT)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffOption, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureOption);

		for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
		{
			if (g_aOption[nCntOption].bUse == true)
			{// オプションが使用されている
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOption * 4, 2);
			}
		}
	}
}

//=============================================================================
//	オプションの更新処理
//=============================================================================
void UpdateOption(void)
{
	Player* pPlayer = GetPlayer();		// プレイヤーの情報を取得

	Option* pOption = &g_aOption[0];	// オプションのポインタ

	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++, pOption++)
	{
		if (pOption->bUse == true)
		{
			switch (pOption->state)
			{
			case OPTIONSTATE_NORMAL:

				break;

			case OPTIONSTATE_COOLTIME:
				g_aOption[nCntOption].nCounterState--;
				if (g_aOption[nCntOption].nCounterState <= 0)
				{
					g_aOption[nCntOption].state = OPTIONSTATE_NORMAL;

					// 頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				break;
			}
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true || GetJoypadPress(JOYKEY_LEFT_SHOULDER))
		{// 左キーが押された
			// 左に回転
			pOption->move.z += 0.015f;
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true || GetJoypadPress(JOYKEY_RIGHT_SHOULDER))
		{// 右キーが押された
			// 右に回転
			pOption->move.z += -0.015f;
		}

		pOption->rot.z += pOption->move.z;

		pOption->move.z += (0.0f - pOption->move.z) * 0.1f;

		pOption->fAngle = atan2f(pOption->distance.x, pOption->distance.y);

		pOption->pos.x = pPlayer->pos.x + sinf(pOption->rot.z + D3DX_PI + pOption->fAngle) * pOption->distance.x;
		pOption->pos.y = pPlayer->pos.y + cosf(pOption->rot.z + D3DX_PI + pOption->fAngle) * pOption->distance.y;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pOption->pos.x - OPTION_RADIUS, pOption->pos.y - OPTION_RADIUS, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pOption->pos.x + OPTION_RADIUS, pOption->pos.y - OPTION_RADIUS, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pOption->pos.x - OPTION_RADIUS, pOption->pos.y + OPTION_RADIUS, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pOption->pos.x + OPTION_RADIUS, pOption->pos.y + OPTION_RADIUS, 0.0f);

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	オプションの取得
//=============================================================================
Option *GetOption(void)
{
	return &g_aOption[0];		// オプションの情報の先頭アドレスを返す
}

//=============================================================================
//	オプションの設定
//=============================================================================
void SetOption(D3DXVECTOR3 pos, D3DXVECTOR3 distance)
{
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{
		if (g_aOption[nCntOption].bUse == false)
		{
			g_aOption[nCntOption].pos = pos;
			g_aOption[nCntOption].bUse = true;
			g_aOption[nCntOption].distance = distance;

			g_aOption[nCntOption].pos += g_aOption[nCntOption].distance;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f)
									  + g_aOption[nCntOption].distance;
			pVtx[1].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f)
									  + g_aOption[nCntOption].distance;
			pVtx[2].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f)
									  + g_aOption[nCntOption].distance;
			pVtx[3].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f)
									  + g_aOption[nCntOption].distance;

			break;
		}

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	オプションのヒット処理
//=============================================================================
void HitOption(int nCntOption)
{
	g_aOption[nCntOption].state = OPTIONSTATE_COOLTIME;			// 状態変更
	g_aOption[nCntOption].nCounterState = OPTION_COOLTIME;		// クールタイム設定

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nCntOption * 4);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	// 頂点バッファをアンロックする
	g_pVtxBuffOption->Unlock();
}
