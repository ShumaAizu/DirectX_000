////=============================================================================
////
////	残機表示処理 [stock.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//#include "stock.h"
//#include "player.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define STOCK_POSX		(0.0f)				// 残機の座標X
//#define STOCK_POSY		(100.0f)			// 残機の座標Y
//#define STOCK_SIZEX		(50.0f)				// 残機のサイズX
//#define STOCK_SIZEY		(50.0f)				// 残機のサイズY
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_pTextureStock = NULL;				// 残機のテクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;			// 残機の頂点バッファへのポインタ
//D3DXVECTOR3 g_posStock;									// 残機の位置
//
////=============================================================================
////	ライフの初期化処理
////=============================================================================
//void InitStock(void)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	int nCntStock = 0;
//
//	// 初期化
//	g_posStock = D3DXVECTOR3(STOCK_POSX, STOCK_POSY, 0.0f);
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock000.png", &g_pTextureStock);
//
//
//	// 頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_STOCK,		// 最大数だけ
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffStock,
//		NULL);
//
//	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
//	{
//		// 頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX), STOCK_POSY, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX) + STOCK_SIZEX, STOCK_POSY, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX), STOCK_POSY + STOCK_SIZEY, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX) + STOCK_SIZEX, STOCK_POSY + STOCK_SIZEY, 0.0f);
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
//	g_pVtxBuffStock->Unlock();
//}
//
////=============================================================================
////	ライフの終了処理
////=============================================================================
//void UninitStock(void)
//{
//	// テクスチャの破棄
//	if (g_pTextureStock != NULL)
//	{
//		g_pTextureStock->Release();
//		g_pTextureStock = NULL;
//	}
//
//	// 頂点バッファの破棄
//	if (g_pVtxBuffStock != NULL)
//	{
//		g_pVtxBuffStock->Release();
//		g_pVtxBuffStock = NULL;
//	}
//}
//
////=============================================================================
////	ライフの描画処理
////=============================================================================
//void DrawStock(void)
//{
//	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
//
//	// デバイスの取得
//	pDevice = GetDevice();
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, g_pVtxBuffStock, 0, sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, g_pTextureStock);
//
//	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
//	{
//		// ポリゴンの描画
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStock * 4, 2);
//	}
//}
//
////=============================================================================
////	ライフの更新処理
////=============================================================================
//void UpdateStock(void)
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
//	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntNumber = 0; nCntNumber < MAX_STOCK; nCntNumber++)
//	{
//		if (pPlayer->nStock >= nCntNumber + 1)
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
//	g_pVtxBuffStock->Unlock();
//}