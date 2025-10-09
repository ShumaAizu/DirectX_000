//=============================================================================
//
//	タイトルメニュー処理 [tutorialmenu.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "tutorial.h"
#include "tutorialmenu.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIALMENU_POSX		(640.0f)
#define TUTORIALMENU_POSY		(75.0f)
#define TUTORIALMENU_SIZEX		(300.0f)
#define TUTORIALMENU_SIZEY		(75.0f)

//*****************************************************************************
// タイトルメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}TutorialMenu;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTutorialMenu[TUTORIALEVENT_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMenu = NULL;					// 頂点バッファへのポインタ
TutorialMenu g_tutorialMenu = {};										// タイトルメニューの情報

//=============================================================================
//	タイトルメニューの初期化処理
//=============================================================================
void InitTutorialMenu(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialMenu,
		NULL);

	const char* pTutorial[TUTORIALEVENT_MAX] =
	{
		"data\\TEXTURE\\Tutorial001.png",
		"data\\TEXTURE\\Tutorial002.png",
		"data\\TEXTURE\\Tutorial003.png",
		"data\\TEXTURE\\Tutorial004.png"
	};

	for (int nCntTutorialEvent = 0; nCntTutorialEvent < TUTORIALEVENT_MAX; nCntTutorialEvent++)
	{
		D3DXCreateTextureFromFile(pDevice, pTutorial[nCntTutorialEvent], &g_apTextureTutorialMenu[nCntTutorialEvent]);
	}

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorialMenu->Lock(0, 0, (void * *)&pVtx, 0);

	g_tutorialMenu.pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(TUTORIALMENU_POSX - TUTORIALMENU_SIZEX, TUTORIALMENU_POSY - TUTORIALMENU_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIALMENU_POSX + TUTORIALMENU_SIZEX, TUTORIALMENU_POSY - TUTORIALMENU_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIALMENU_POSX - TUTORIALMENU_SIZEX, TUTORIALMENU_POSY + TUTORIALMENU_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIALMENU_POSX + TUTORIALMENU_SIZEX, TUTORIALMENU_POSY + TUTORIALMENU_SIZEY, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialMenu->Unlock();
}

//=============================================================================
//	タイトルメニューの終了処理
//=============================================================================
void UninitTutorialMenu(void)
{
	// テクスチャの破棄
	for (int nCntTutorialEvent = 0; nCntTutorialEvent < TUTORIALEVENT_MAX; nCntTutorialEvent++)
	{
		if (g_apTextureTutorialMenu[nCntTutorialEvent] != NULL)
		{
			g_apTextureTutorialMenu[nCntTutorialEvent]->Release();
			g_apTextureTutorialMenu[nCntTutorialEvent] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorialMenu != NULL)
	{
		g_pVtxBuffTutorialMenu->Release();
		g_pVtxBuffTutorialMenu = NULL;
	}
}

//=============================================================================
//	タイトルメニューの描画処理
//=============================================================================
void DrawTutorialMenu(void)
{
	TUTORIALEVENT tutorialevent = GetTutorialEvent();

	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTutorialMenu[tutorialevent]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateTutorialMenu(void)
{

}