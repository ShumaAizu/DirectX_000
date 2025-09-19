//=============================================================================
//
//	タイトルメニュー処理 [titlemenu.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "titlemenu.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLEMENU_POSX		(440.0f)
#define TITLEMENU_POSY		(410.0f)
#define TITLEMENU_SIZEX		(400.0f)
#define TITLEMENU_SIZEY		(100.0f)
#define TITLEFADE_TIMER		(300)		// タイマーの秒数

//*****************************************************************************
// タイトルメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}TitleMenu;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTitleMenu[TITLEMENU_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMenu = NULL;					// 頂点バッファへのポインタ
TITLEMENU g_titleMenu = TITLEMENU_GAMESTART;						// タイトルメニューの状態
TitleMenu g_atitleMenu[TITLEMENU_MAX] = {};							// タイトルメニューの情報
int g_nMenuChangeCounter = 0;										// メニュー切り替えカウンター
bool g_bUpdate_TitleMenu = true;									// タイトルメニュー操作可能か
int g_nTitleFadeCounter = 0;

//=============================================================================
//	タイトルメニューの初期化処理
//=============================================================================
void InitTitleMenu(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[TITLEMENU_MAX] =
	{
		"data\\TEXTURE\\Title_GAMESTART.png",
		"data\\TEXTURE\\Title_EXIT.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMenu,
		NULL);

	for (int nCntTitleMenuTex = 0; nCntTitleMenuTex < TITLEMENU_MAX; nCntTitleMenuTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntTitleMenuTex], &g_apTextureTitleMenu[nCntTitleMenuTex]);
	}

	// 初期化
	g_titleMenu = TITLEMENU_GAMESTART;
	g_bUpdate_TitleMenu = true;
	g_nMenuChangeCounter = 60;
	g_nTitleFadeCounter = 0;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleMenu->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		g_atitleMenu[nCntTitleMenu].pos = D3DXVECTOR3(TITLEMENU_POSX, TITLEMENU_POSY, 0.0f);
		g_atitleMenu[nCntTitleMenu].nDispCounter = 4;
		g_atitleMenu[nCntTitleMenu].bDisp = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(TITLEMENU_POSX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TITLEMENU_POSX + TITLEMENU_SIZEX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TITLEMENU_POSX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY) + TITLEMENU_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TITLEMENU_POSX + TITLEMENU_SIZEX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY) + TITLEMENU_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntTitleMenu == g_titleMenu)
		{ // 選択されていれば不透明度を戻す
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // 選択されていなければ不透明度を下げる
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleMenu->Unlock();
}

//=============================================================================
//	タイトルメニューの終了処理
//=============================================================================
void UninitTitleMenu(void)
{
	// テクスチャの破棄
	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (g_apTextureTitleMenu[nCntTitleMenu] != NULL)
		{
			g_apTextureTitleMenu[nCntTitleMenu]->Release();
			g_apTextureTitleMenu[nCntTitleMenu] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleMenu != NULL)
	{
		g_pVtxBuffTitleMenu->Release();
		g_pVtxBuffTitleMenu = NULL;
	}
}

//=============================================================================
//	タイトルメニューの描画処理
//=============================================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (g_atitleMenu[nCntTitleMenu].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitleMenu[nCntTitleMenu]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitleMenu * 4, 2);
		}
	}
	
}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateTitleMenu(void)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (nCntTitleMenu == g_titleMenu)
		{ // 選択されていれば不透明度を戻す
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // 選択されていなければ不透明度を下げる
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		pVtx += 4;
	}

	if (g_bUpdate_TitleMenu == true)
	{
		if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStick(JOYSTICK_UP) == true)
		{ // 上方向キーが押されたら
			// 現在のモードに合わせて変更
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_titleMenu)
			{
			case TITLEMENU_GAMESTART:
				g_titleMenu = TITLEMENU_EXIT;
				break;

			case 	TITLEMENU_EXIT:
				g_titleMenu = TITLEMENU_GAMESTART;
				break;
			}
		}

		if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStick(JOYSTICK_DOWN) == true)
		{ // 下方向キーが押されたら
			// 現在のモードに合わせて変更
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_titleMenu)
			{
			case TITLEMENU_GAMESTART:
				g_titleMenu = TITLEMENU_EXIT;
				break;

			case TITLEMENU_EXIT:
				g_titleMenu = TITLEMENU_GAMESTART;
				break;
			}
		}
	}

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // 決定キーが押されたら
		if (g_bUpdate_TitleMenu == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bUpdate_TitleMenu = false;
	}

	if (g_bUpdate_TitleMenu == false)
	{
		g_nMenuChangeCounter--;
		g_atitleMenu[g_titleMenu].nDispCounter++;
		if (g_atitleMenu[g_titleMenu].nDispCounter % 5 == 0 && g_nMenuChangeCounter >= 0)
		{
			g_atitleMenu[g_titleMenu].bDisp = g_atitleMenu[g_titleMenu].bDisp ? false : true;
		}

		if (g_nMenuChangeCounter <= 0)
		{
			// 現在のモードに合わせて変更
			switch (g_titleMenu)
			{
			case TITLEMENU_GAMESTART:
				SetFade(MODE_TUTORIAL, 0.025f, 0.025f);
				break;

			case TITLEMENU_EXIT:
				PostQuitMessage(0);
				break;
			}
		}
	}

	g_nTitleFadeCounter++;

	if (GetKeyboardAny() == true || GetJoypadAny() == true)
	{
		g_nTitleFadeCounter = 0;
	}

	if (g_nTitleFadeCounter >= TITLEFADE_TIMER && g_bUpdate_TitleMenu != false)
	{// カウンターが規定値を超えた
		// モード設定
		SetFade(MODE_RANKING, 0.025f, 0.025f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTitleMenu->Unlock();
}

//=============================================================================
//	タイトルメニューの設定処理
//=============================================================================
void SetTitleMenu(TITLEMENU titlemenu)
{
	g_titleMenu = titlemenu;
}