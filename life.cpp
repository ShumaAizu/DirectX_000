////=============================================================================
////
////	ライフ表示処理 [life.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//#include "life.h"
//#include "player.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define LIFE_POSX		(0.0f)				// ライフの座標X
//#define LIFE_POSY		(0.0f)				// ライフの座標Y
//#define LIFE_SIZEX		(100.0f)			// ライフのサイズX
//#define LIFE_SIZEY		(100.0f)			// ライフのサイズY
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;				// ライフのテクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;			// ライフの頂点バッファへのポインタ
//D3DXVECTOR3 g_posLife;									// ライフの位置
//
////=============================================================================
////	ライフの初期化処理
////=============================================================================
//void InitLife(void)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	int nCntLife = 0;
//
//	// 初期化
//	g_posLife = D3DXVECTOR3(LIFE_POSX, LIFE_POSY, 0.0f);
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Life000.png", &g_pTextureLife);
//
//
//	// 頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,		// 最大数だけ
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffLife,
//		NULL);
//
//	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
//	{
//		// 頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX), LIFE_POSY, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX) + LIFE_SIZEX, LIFE_POSY, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX), LIFE_POSY + LIFE_SIZEY, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX) + LIFE_SIZEX, LIFE_POSY + LIFE_SIZEY, 0.0f);
//
//		// rhwの設定
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//
//		// 頂点カラーの設定
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
//
//		pVtx += 4;			// 頂点データのポインタを4つ分進める
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffLife->Unlock();
//}
//
////=============================================================================
////	ライフの終了処理
////=============================================================================
//void UninitLife(void)
//{
//	// テクスチャの破棄
//	if (g_pTextureLife != NULL)
//	{
//		g_pTextureLife->Release();
//		g_pTextureLife = NULL;
//	}
//
//	// 頂点バッファの破棄
//	if (g_pVtxBuffLife != NULL)
//	{
//		g_pVtxBuffLife->Release();
//		g_pVtxBuffLife = NULL;
//	}
//}
//
////=============================================================================
////	ライフの描画処理
////=============================================================================
//void DrawLife(void)
//{
	//LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ

	//// デバイスの取得
	//pDevice = GetDevice();

	//// 頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureLife);

	//for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	//{
	//	// ポリゴンの描画
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
	//}
//}
//
////=============================================================================
////	ライフの更新処理
////=============================================================================
//void UpdateLife(void)
//{
//	int nCntNumber = 0;
//
//	// プレイヤーの情報を取得
//	Player* pPlayer = GetPlayer();
//
//	// テクスチャ座標の設定
//	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntNumber = 0; nCntNumber < MAX_LIFE; nCntNumber++)
//	{
//		if (pPlayer->nLife >= nCntNumber + 1)
//		{
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
//		}
//		else
//		{
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		pVtx += 4;
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffLife->Unlock();
//}