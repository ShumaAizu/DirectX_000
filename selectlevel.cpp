//=============================================================================
//
//	タイトルメニュー処理 [selectlevel.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "titlemenu.h"
#include "selectlevel.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SELECTLEVEL_POSX		(440.0f)
#define SELECTLEVEL_POSY		(410.0f)
#define SELECTLEVEL_SIZEX		(400.0f)
#define SELECTLEVEL_SIZEY		(100.0f)
#define TITLEFADE_TIMER		(300)		// タイマーの秒数

//*****************************************************************************
// タイトルメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}SelectLevel;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureSelectLevel[SELECTLEVEL_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectLevel = NULL;					// 頂点バッファへのポインタ
SELECTLEVEL g_SelectLevel = SELECTLEVEL_NORMAL;						// タイトルメニューの状態
SelectLevel g_aSelectLevel[SELECTLEVEL_MAX] = {};							// タイトルメニューの情報
int g_nLevelMenuChangeCounter = 0;										// メニュー切り替えカウンター
bool g_bUpdate_SelectLevel = true;									// タイトルメニュー操作可能か

//=============================================================================
//	タイトルメニューの初期化処理
//=============================================================================
void InitSelectLevel(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[SELECTLEVEL_MAX] =
	{
		"data\\TEXTURE\\Select_NORMAL.png",
		"data\\TEXTURE\\Select_HARD.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SELECTLEVEL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectLevel,
		NULL);

	for (int nCntSelectLevelTex = 0; nCntSelectLevelTex < SELECTLEVEL_MAX; nCntSelectLevelTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntSelectLevelTex], &g_apTextureSelectLevel[nCntSelectLevelTex]);
	}

	// 初期化
	g_SelectLevel = SELECTLEVEL_NORMAL;
	g_bUpdate_SelectLevel = true;
	g_nLevelMenuChangeCounter = 60;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffSelectLevel->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		g_aSelectLevel[nCntSelectLevel].pos = D3DXVECTOR3(SELECTLEVEL_POSX, SELECTLEVEL_POSY, 0.0f);
		g_aSelectLevel[nCntSelectLevel].nDispCounter = 4;
		g_aSelectLevel[nCntSelectLevel].bDisp = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SELECTLEVEL_POSX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SELECTLEVEL_POSX + SELECTLEVEL_SIZEX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SELECTLEVEL_POSX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY) + SELECTLEVEL_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SELECTLEVEL_POSX + SELECTLEVEL_SIZEX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY) + SELECTLEVEL_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntSelectLevel == g_SelectLevel)
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
	g_pVtxBuffSelectLevel->Unlock();
}

//=============================================================================
//	タイトルメニューの終了処理
//=============================================================================
void UninitSelectLevel(void)
{
	// テクスチャの破棄
	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		if (g_apTextureSelectLevel[nCntSelectLevel] != NULL)
		{
			g_apTextureSelectLevel[nCntSelectLevel]->Release();
			g_apTextureSelectLevel[nCntSelectLevel] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffSelectLevel != NULL)
	{
		g_pVtxBuffSelectLevel->Release();
		g_pVtxBuffSelectLevel = NULL;
	}
}

//=============================================================================
//	タイトルメニューの描画処理
//=============================================================================
void DrawSelectLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectLevel, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		if (g_aSelectLevel[nCntSelectLevel].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSelectLevel[nCntSelectLevel]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelectLevel * 4, 2);
		}
	}
	
}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateSelectLevel(void)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffSelectLevel->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		if (nCntSelectLevel == g_SelectLevel)
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

	if (g_bUpdate_SelectLevel == true)
	{
		if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStick(JOYSTICK_UP) == true)
		{ // 上方向キーが押されたら
			// 現在のモードに合わせて変更
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_SelectLevel)
			{
			case SELECTLEVEL_NORMAL:
				g_SelectLevel = SELECTLEVEL_HARD;
				break;

			case 	SELECTLEVEL_HARD:
				g_SelectLevel = SELECTLEVEL_NORMAL;
				break;
			}
		}

		if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStick(JOYSTICK_DOWN) == true)
		{ // 下方向キーが押されたら
			// 現在のモードに合わせて変更
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_SelectLevel)
			{
			case SELECTLEVEL_NORMAL:
				g_SelectLevel = SELECTLEVEL_HARD;
				break;

			case SELECTLEVEL_HARD:
				g_SelectLevel = SELECTLEVEL_NORMAL;
				break;
			}
		}
	}

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // 決定キーが押されたら
		if (g_bUpdate_SelectLevel == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bUpdate_SelectLevel = false;
	}

	if (GetKeyboardTrigger(DIK_Q) == true || GetJoypadTrigger(JOYKEY_B) == true)
	{
		SetTitleState(TITLESTATE_MENU);
		g_SelectLevel = SELECTLEVEL_NORMAL;
		UninitTitleMenu();
		InitTitleMenu();
	}

	if (g_bUpdate_SelectLevel == false)
	{
		g_nLevelMenuChangeCounter--;
		g_aSelectLevel[g_SelectLevel].nDispCounter++;
		if (g_aSelectLevel[g_SelectLevel].nDispCounter % 5 == 0 && g_nLevelMenuChangeCounter >= 0)
		{
			g_aSelectLevel[g_SelectLevel].bDisp = g_aSelectLevel[g_SelectLevel].bDisp ? false : true;
		}

		if (g_nLevelMenuChangeCounter <= 0)
		{
			// 現在のモードに合わせて変更
			switch (g_SelectLevel)
			{
			case SELECTLEVEL_NORMAL:
				SetGameMode(GAMEMODE_NORMAL);
				break;

			case SELECTLEVEL_HARD:
				SetGameMode(GAMEMODE_HARD);
				break;
			}

			SetFade(MODE_GAME, 0.025f, 0.025f);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffSelectLevel->Unlock();
}

//=============================================================================
//	タイトルメニューの設定処理
//=============================================================================
void SetSelectLevel(SELECTLEVEL selectlevel)
{
	g_SelectLevel = selectlevel;
}