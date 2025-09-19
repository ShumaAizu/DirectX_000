//=============================================================================
//
//	タイトル背景処理 [titlelogo.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlelogo.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLELOGO_DEFAULTPOSX	(640.0f)
#define TITLELOGO_DEFAULTPOSY	(180.0f)
#define TITLELOGO_SIZEX			(500.0f)
#define TITLELOGO_SIZEY			(250.0f)
#define TITLELOGO_INITPOSX		(1280.0f + TITLELOGO_SIZEX)
#define TITLELOGO_INITPOSY		(TITLELOGO_DEFAULTPOSY)
#define TITLELOGO_SPEED			(10.0f)

////*****************************************************************************
//// タイトルロゴ構造体
////*****************************************************************************
//typedef struct
//{
//	D3DXVECTOR3 pos;		// 位置
//	bool bDisp;				// 表示状態
//}TitleLogo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;			// 頂点バッファへのポインタ
D3DXVECTOR3 g_titleLogo_pos = {};							// タイトルロゴの位置
int g_nStateChangeCounter = 0;

//====================================
//	タイトルの初期化処理
//====================================
void InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\Title_logo.png",
			&g_pTextureTitleLogo);
	

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	g_titleLogo_pos = { TITLELOGO_INITPOSX , TITLELOGO_INITPOSY , 0.0f };
	g_nStateChangeCounter = 0;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(TITLELOGO_INITPOSX - TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLELOGO_INITPOSX + TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLELOGO_INITPOSX - TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLELOGO_INITPOSX + TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//====================================
//	タイトルの終了処理
//====================================
void UninitTitleLogo(void)
{

	// テクスチャの破棄
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//====================================
//	タイトルの描画処理
//====================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//====================================
//	タイトルの更新処理
//====================================
void UpdateTitleLogo(void)
{
	TITLESTATE TitleState = GetTitleState();

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	switch (TitleState)
	{
	case TITLESTATE_OP:
		g_titleLogo_pos.x -= TITLELOGO_SPEED;

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{
			g_titleLogo_pos = { TITLELOGO_DEFAULTPOSX , TITLELOGO_DEFAULTPOSY , 0.0f };
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_titleLogo_pos.x - TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_titleLogo_pos.x + TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_titleLogo_pos.x - TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_titleLogo_pos.x + TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);


		if (g_titleLogo_pos.x <= TITLELOGO_DEFAULTPOSX)
		{
			SetRainbowParticle(g_titleLogo_pos, 5000.0f, 30, D3DX_PI, -D3DX_PI);
			SetRainbowParticle(g_titleLogo_pos, 5000.0f, 30, D3DX_PI, -D3DX_PI);
			PlaySound(SOUND_LABEL_SE_TITLESET000);
			g_nStateChangeCounter = 60;
			SetTitleState(TITLESTATE_WAIT);
		}
		break;

	case TITLESTATE_WAIT:
		g_nStateChangeCounter--;
		if (g_nStateChangeCounter <= 0)
		{
			SetTitleState(TITLESTATE_MENU);
		}
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}