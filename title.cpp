//=============================================================================
//
//	タイトル画面処理 [title.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlemenu.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TITLE_UI		(4)
#define TITLEFADE_TIMER		(300)		// タイマーの秒数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_UI] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// 頂点バッファへのポインタ
TITLESTATE g_titleState = TITLESTATE_OP;							// タイトルの状態
int g_nCounterTitleState = 0;										// 状態管理カウンター
int g_nTitleFadeCounter = 0;

//====================================
//	タイトルの初期化処理
//====================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	const char* pTitle_UI[MAX_TITLE_UI] =
	{
		"data\\TEXTURE\\bg000.jpg",
		"data\\TEXTURE\\Title.png",
		"data\\TEXTURE\\Title_GAMESTART.png",
		"data\\TEXTURE\\Title_EXIT.png",
	};

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_UI; nCntTitle++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			pTitle_UI[nCntTitle],
			&g_pTextureTitle[nCntTitle]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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
	g_pVtxBuffTitle->Unlock();

	g_nTitleFadeCounter = 0;

	// タイトルメニューの初期化
	InitTitleMenu();

	PlaySound(SOUND_LABEL_BGM000);
}

//====================================
//	タイトルの終了処理
//====================================
void UninitTitle(void)
{
	// タイトルメニューの終了処理
	UninitTitleMenu();

	// サウンドを止める
	StopSound();

	// テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_UI; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//====================================
//	タイトルの描画処理
//====================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_UI; nCntTitle++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}

	// タイトルメニューの描画処理
	DrawTitleMenu();
}

//====================================
//	タイトルの更新処理
//====================================
void UpdateTitle(void)
{
	g_nTitleFadeCounter++;

	//if (GetJoypadTrigger(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	//{// 決定キーが押された
	//	// モード設定
	//	SetFade(MODE_TUTORIAL);
	//}

	if (g_nTitleFadeCounter >= TITLEFADE_TIMER)
	{// カウンターが規定値を超えた
		// モード設定
		SetFade(MODE_RANKING);
	}

	// タイトルメニューの更新処理
	UpdateTitleMenu();
}

//========================================
//	タイトルの状態設定
//========================================
void SetTitleState(TITLESTATE titlestate, int nCounter)
{
	g_titleState = titlestate;

	g_nCounterTitleState = nCounter;
}