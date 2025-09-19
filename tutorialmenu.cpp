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
#define TUTORIALMENU_POSX		(780.0f)
#define TUTORIALMENU_POSY		(500.0f)
#define TUTORIALMENU_SIZEX		(500.0f)
#define TUTORIALMENU_SIZEY		(200.0f)
#define TUTORIALFADE_TIMER		(300)		// タイマーの秒数

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
LPDIRECT3DTEXTURE9 g_apTextureTutorialMenu = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMenu = NULL;				// 頂点バッファへのポインタ
TutorialMenu g_tutorialMenu = {};									// タイトルメニューの情報
int g_nTutorialChangeCounter = 0;									// メニュー切り替えカウンター
bool g_bUpdate_TutorialMenu = true;									// タイトルメニュー操作可能か
bool g_bTutorialFade = false;

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


		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\PRESS.png", &g_apTextureTutorialMenu);

	// 初期化
	g_tutorialMenu.nDispCounter = 0;
	g_bUpdate_TutorialMenu = true;
	g_nTutorialChangeCounter = 60;
	g_bTutorialFade = false;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorialMenu->Lock(0, 0, (void * *)&pVtx, 0);

	g_tutorialMenu.pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY, 0.0f);
	g_tutorialMenu.nDispCounter = 4;
	g_tutorialMenu.bDisp = true;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIALMENU_POSX + TUTORIALMENU_SIZEX, TUTORIALMENU_POSY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY + TUTORIALMENU_SIZEY, 0.0f);
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
	if (g_apTextureTutorialMenu != NULL)
	{
		g_apTextureTutorialMenu->Release();
		g_apTextureTutorialMenu = NULL;
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
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	if (g_tutorialMenu.bDisp == true)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTutorialMenu);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	
}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateTutorialMenu(void)
{
	g_tutorialMenu.nDispCounter++;

	if (g_tutorialMenu.nDispCounter % 60 == 0 && g_bTutorialFade != true)
	{
		g_tutorialMenu.bDisp = g_tutorialMenu.bDisp ? false : true;
		if (g_tutorialMenu.bDisp == false)
		{
			g_tutorialMenu.nDispCounter = 54;
		}
		else
		{
			g_tutorialMenu.nDispCounter = 0;
		}
	}

	if (GetJoypadTrigger(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{// 決定キーが押された
		// モード設定
		if (g_bTutorialFade != true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bTutorialFade = true;
	}

	if (g_bTutorialFade == true)
	{
		g_nTutorialChangeCounter--;
		g_tutorialMenu.nDispCounter++;
		if (g_tutorialMenu.nDispCounter % 5 == 0 && g_nTutorialChangeCounter >= 0)
		{
			g_tutorialMenu.bDisp = g_tutorialMenu.bDisp ? false : true;
		}

		if (g_nTutorialChangeCounter <= 0)
		{
			SetFade(MODE_GAME, 0.025f, 0.025f);
		}
	}
}