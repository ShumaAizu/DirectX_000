//=============================================================================
//
//	スコア処理 [score.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "score.h"
#include "camera.h"
#include "enemy.h"
#include "powerup.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE		(50000)				// スコアの最大
#define MAX_SCORE_UI	(SCOREUI_MAX)		// スコアのUIの数
#define SCORE_POSX		(880.0f)			// スコアの座標X
#define SCORE_POSY		(672.0f)			// スコアの座標Y
#define SCORE_SIZEX		(520.0f)			// スコアのサイズX
#define SCORE_SIZEY		(48.0f)				// スコアのサイズY
#define INIT_SCORE		(500)				// スコアの初期値

//*****************************************************************************
// スコアUI構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	SCOREUI type;				// スコアUIの種類
}ScoreUI;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureScore[MAX_SCORE_UI] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;						// 頂点バッファへのポインタ
ScoreUI g_ascoreUI[MAX_SCORE_UI] = {};								// スコアUIの情報
int g_nScore;														// スコアの値

//=============================================================================
//	スコアの初期化処理
//=============================================================================
void InitScore(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore = 0;

	const char* pUIPass[MAX_SCORE_UI] = {
		"data\\TEXTURE\\Score_UI000.png",
		"data\\TEXTURE\\Score_UI001.png",
		"data\\TEXTURE\\Score_UI002.png"
	};

	// 初期化
	g_nScore = 0;

	for (int nCntScoreTex = 0; nCntScoreTex < MAX_SCORE_UI; nCntScoreTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pUIPass[nCntScoreTex], &g_pTextureScore[nCntScoreTex]);
	}


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE_UI,		// 桁の数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCORE_UI; nCntScore++)
	{
		g_ascoreUI[nCntScore].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
		g_ascoreUI[nCntScore].type = (SCOREUI)nCntScore;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCORE_POSX - SCORE_SIZEX, SCORE_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCORE_POSX - SCORE_SIZEX, SCORE_POSY + SCORE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY + SCORE_SIZEY, 0.0f);

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
	g_pVtxBuffScore->Unlock();

	SetScore(INIT_SCORE);

#ifdef _DEBUG
	//SetScore(50000);
#endif
}

//====================================
//	スコアの終了処理
//====================================
void UninitScore(void)
{

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_SCORE_UI; nCntTex++)
	{
		if (g_pTextureScore[nCntTex] != NULL)
		{
			g_pTextureScore[nCntTex]->Release();
			g_pTextureScore[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//====================================
//	スコアの描画処理
//====================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_UI; nCntNumber++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore[nCntNumber]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, 2);
	}
}

//====================================
//	スコアの更新処理
//====================================
void UpdateScore(void)
{
	float fRatio = 0;

	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	bool bPowerUp = GetPowerUp();

	CollisionScoretoEnemy();

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);



	for (int nCntScoreUI = 0; nCntScoreUI < MAX_SCORE_UI; nCntScoreUI++)
	{
		if (g_ascoreUI[nCntScoreUI].type != SCOREUI_CURTAIN )
		{
			continue;
		}

		fRatio = ((float)g_nScore / MAX_SCORE);

		pVtx += (nCntScoreUI * 4);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCORE_POSX - (SCORE_SIZEX - SCORE_SIZEX * fRatio), SCORE_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCORE_POSX - (SCORE_SIZEX - SCORE_SIZEX * fRatio), SCORE_POSY + SCORE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY + SCORE_SIZEY, 0.0f);

		if (bPowerUp == true)
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//====================================
//	スコアの設定処理
//====================================
void SetScore(int nScore)
{
	//int aTexU[NUM_PLACE];		// 各桁の数字を格納
	int nCntNumber = 0;
	g_nScore = nScore;

	//aTexU[0] = g_nScore % 100000000 / 10000000;
	//aTexU[1] = g_nScore % 10000000 / 1000000;
	//aTexU[2] = g_nScore % 1000000 / 100000;
	//aTexU[3] = g_nScore % 100000 / 10000;
	//aTexU[4] = g_nScore % 10000 / 1000;
	//aTexU[5] = g_nScore % 1000 / 100;
	//aTexU[6] = g_nScore % 100 / 10;
	//aTexU[7] = g_nScore % 10 / 1;
}

//====================================
//	スコアの加算処理
//====================================
void AddScore(int nValue)
{
	g_nScore += nValue;
}

//====================================
//	スコアの減算処理
//====================================
void SubScore(int nValue)
{
	g_nScore -= nValue;
}

//====================================
//	スコアの取得
//====================================
int GetScore(void)
{
	return g_nScore;
}

//=============================================================================
//	スコアと敵との当たり判定
//=============================================================================
void CollisionScoretoEnemy(void)
{
	bool bCheckCollision = {};

	// 敵の情報を取得
	Enemy* pEnemy = GetEnemy();

	D3DXVECTOR3* pCameraPos = GetCamera();

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->pos.x - pCameraPos->x + pEnemy->fRadius >= SCORE_POSX - SCORE_SIZEX &&
				pEnemy->pos.x - pCameraPos->x - pEnemy->fRadius <= SCORE_POSX + SCORE_SIZEX &&
				pEnemy->pos.y - pCameraPos->y + pEnemy->fRadius >= SCORE_POSY - SCORE_SIZEY &&
				pEnemy->pos.y - pCameraPos->y - pEnemy->fRadius <= SCORE_POSY + SCORE_SIZEY)
			{// もし敵とプレイヤーがあたっていたら
				// 半透明にする
				bCheckCollision = true;
				for (int nCntScore = 0; nCntScore < MAX_SCORE_UI; nCntScore++)
				{
					// 頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

					pVtx += 4;			// 頂点データのポインタを4つ分進める
				}
				break;
			}
		}
	}

	if (bCheckCollision == false)
	{
		for (int nCntScore = 0; nCntScore < MAX_SCORE_UI; nCntScore++)
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;			// 頂点データのポインタを4つ分進める
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}