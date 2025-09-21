//=============================================================================
//
//	敵の処理 [bossparts.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "enemy.h"
#include "bossparts.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "life.h"
#include "camera.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_BOSSPARTS		(3)			// 敵の種類
#define BOSSPARTS_SCORE		(500)		// 敵撃破時の獲得スコア

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBossParts[NUM_BOSSPARTS] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossParts = NULL;					// 頂点バッファへのポインタ
BossParts g_aBossParts[MAX_BOSSPARTS];								// 敵の情報

//====================================
//	敵の初期化処理
//====================================
void InitBossParts(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBossParts = 0;
	D3DXVECTOR3* pCameraPos = GetCamera();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bossparts000.png",
		&g_apTextureBossParts[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bossparts001.png",
		&g_apTextureBossParts[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bossparts002.png",
		&g_apTextureBossParts[2]);

	// 初期化
	for (nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		g_aBossParts[nCntBossParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBossParts[nCntBossParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBossParts[nCntBossParts].fRadius = 0.0f;
		g_aBossParts[nCntBossParts].bDisp = false;
		g_aBossParts[nCntBossParts].bUse = false;			// 使用していない状態にする
		g_aBossParts[nCntBossParts].bBlinking = false;
		g_aBossParts[nCntBossParts].fMove = 0.0f;
		g_aBossParts[nCntBossParts].nTimeLine = -1;
		g_aBossParts[nCntBossParts].nScore = 0;
		g_aBossParts[nCntBossParts].nLife = 3;
		g_aBossParts[nCntBossParts].nCounterAnim = 0;
		g_aBossParts[nCntBossParts].nPatternAnim = 0;
		g_aBossParts[nCntBossParts].nCounterState = 0;
		g_aBossParts[nCntBossParts].nCounterAttack = 60;
		g_aBossParts[nCntBossParts].nUseCounter = 600;
		g_aBossParts[nCntBossParts].state = BOSSPARTSSTATE_NORMAL;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSPARTS,		// 敵の数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossParts,
		NULL);

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		float fAngle = atan2f(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);
		float fRadius = SQRTF(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);

		// 頂点座標の設定
		pVtx[0].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
		pVtx[0].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
		pVtx[1].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
		pVtx[2].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
		pVtx[3].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
		pVtx[3].pos.z = 0.0f;

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

		if (nCntBossParts % 2 != 0)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBossParts->Unlock();
}

//=============================================================================
//	敵の終了処理
//=============================================================================
void UninitBossParts(void)
{
	int nCntTex;
	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < NUM_BOSSPARTS; nCntTex++)
	{
		if (g_apTextureBossParts[nCntTex] != NULL)
		{
			// テクスチャの破棄
			g_apTextureBossParts[nCntTex]->Release();
			g_apTextureBossParts[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBossParts != NULL)
	{
		g_pVtxBuffBossParts->Release();
		g_pVtxBuffBossParts = NULL;
	}
}

//=============================================================================
//	敵の描画処理
//=============================================================================
void DrawBossParts(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBossParts, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		if (g_aBossParts[nCntBossParts].bUse == true && g_aBossParts[nCntBossParts].bDisp == true)
		{// 敵が使用されている
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBossParts[g_aBossParts[nCntBossParts].type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBossParts * 4, 2);
		}
	}
}

//=============================================================================
//	敵の更新処理
//=============================================================================
void UpdateBossParts(void)
{
	int nCntBossParts = 0;

	BossParts* pBossParts = &g_aBossParts[0];

	Player* pPlayer = GetPlayer();		// プレイヤーの情報取得

	D3DXVECTOR3* pCameraPos = GetCamera();

	Enemy* pBossEnemy = GetBOSSEnemy();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	// 使用判定
	for (nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++, pVtx += 4, pBossParts++)
	{
		if (pBossParts->bUse == true)
		{// もし敵が使用されていたら
			Player* pPlayer = GetPlayer();		// プレイヤーの情報取得
			float fAngleE_P = atan2f(pPlayer->pos.x - pBossParts->pos.x,		// プレイヤーと敵との角度算出
				pPlayer->pos.y - pBossParts->pos.y);

			float fAngle = atan2f(pBossParts->fRadius, pBossParts->fRadius);
			float fRadius = SQRTF(pBossParts->fRadius, pBossParts->fRadius);

			// 頂点座標の設定
			pVtx[0].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z + fAngle) * fRadius;
			pVtx[0].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z + fAngle) * fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z - fAngle) * fRadius;
			pVtx[1].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z - fAngle) * fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.z = 0.0f;

			// ============== //
			// 敵の状態		  //
			// ============== //
			switch (pBossParts->state)
			{// 状態チェック
				// 出現状態
			case BOSSPARTSSTATE_APPEAR:
				SetRainbowParticle(pBossParts->pos, 2500.0f, 10, D3DX_PI, -D3DX_PI);	// パーティクルを設定
				pBossParts->nCounterState--;			// カウントを減らす
				if (pBossParts->nCounterState <= 0)
				{// カウントが0以下になったなら
					pBossParts->bDisp = true;				// 描画
					pBossParts->nCounterState = 0;			// カウンターリセット
					pBossParts->state = BOSSPARTSSTATE_NORMAL;	// 状態を通常に
				}
				continue;
				break;

				// 通常状態
			case BOSSPARTSSTATE_NORMAL:

				break;

				// ダメージ状態
			case BOSSPARTSSTATE_DAMAGE:
				pBossParts->nCounterState--;		// 状態持続時間を減らす
				if (pBossParts->nCounterState <= 0)
				{// 状態持続時間がなくなったら
					pBossParts->state = BOSSPARTSSTATE_NORMAL;
					// 頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				}
				break;

			case BOSSPARTSSTATE_WAIT:
				pBossParts->state = BOSSPARTSSTATE_APPEAR;
				pBossParts->nCounterState = 60;
				break;
			}

			switch (pBossParts->type)
			{
			case BOSSPARTSTYPE_WING:
				if (nCntBossParts % 2 == 0)
				{
					SetParticle(D3DXVECTOR3(pBossParts->pos.x - 45.0f, pBossParts->pos.y + 5.0f, 0.0f), D3DXCOLOR(0.95f, 1.0f, 0.95f, 1.0f), 2500.0f, 5, (0.15f * D3DX_PI), (-0.85f * D3DX_PI));
				}
				else
				{
					SetParticle(D3DXVECTOR3(pBossParts->pos.x + 45.0f, pBossParts->pos.y + 5.0f, 0.0f), D3DXCOLOR(0.95f, 1.0f, 0.95f, 1.0f), 2500.0f, 5, (0.85f * D3DX_PI), (-0.15f * D3DX_PI));
				}
				pBossParts->nCounterAttack--;
				if (pBossParts->nCounterAttack <= 0)
				{
					if (nCntBossParts % 2 == 0)
					{
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (-0.75f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (-0.60f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (-0.45f * D3DX_PI));
					}
					else
					{
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (0.75f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (0.60f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (0.45f * D3DX_PI));
					}
					pBossParts->nCounterAttack = 180;
				}


				pBossParts->pos.x += sinf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				pBossParts->pos.y += cosf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				break;

			case BOSSPARTSTYPE_HAND:
				SetParticle(pBossParts->pos, D3DXCOLOR(0.5f, 0.25f, 0.0f, 1.0f), 2500.0f, 5, D3DX_PI, -D3DX_PI);

				pBossParts->pos.x += sinf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				pBossParts->pos.y += cosf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;

				break;

			case BOSSPARTSTYPE_FLARE:
				SetParticle(D3DXVECTOR3(pBossParts->pos.x, pBossParts->pos.y - 25.0f, 0.0f), D3DXCOLOR(0.6f, 0.9f, 1.0f, 1.0f), 2500.0f, 5, (0.25f * D3DX_PI), (-0.25f * D3DX_PI));


				pBossParts->pos.x += sinf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				pBossParts->pos.y += cosf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				break;

			}

			if (pPlayer->state == PLAYERSTATE_DEATH)
			{// プレイヤーが死亡状態なら色を戻す
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// 使用判定
			if (pBossParts->pos.x <= 0 || pBossParts->pos.x >= WARLD_WIDTH ||
				pBossParts->pos.y <= 0 || pBossParts->pos.y >= WARLD_HEIGHT ||
				pBossParts->nUseCounter <= 0)
			{// 画面外に出た / カウンターが回り切った
				ResetBossParts(pBossParts);
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBossParts->Unlock();
}
//=============================================================================
//	敵の設定
//=============================================================================
void SetBossParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		if (g_aBossParts[nCntBossParts].bUse == false && g_aBossParts[nCntBossParts].state != BOSSPARTSSTATE_WAIT)
		{// 敵を使用していない
			g_aBossParts[nCntBossParts].pos = pos;
			g_aBossParts[nCntBossParts].rot = rot;
			g_aBossParts[nCntBossParts].type = (BOSSPARTSTYPE)nType;
			g_aBossParts[nCntBossParts].fRadius = fRadius;
			g_aBossParts[nCntBossParts].fMove = fMove;
			g_aBossParts[nCntBossParts].nLife = nLife;
			g_aBossParts[nCntBossParts].nTimeLine = nTimeLine;
			g_aBossParts[nCntBossParts].nScore = nScore;
			g_aBossParts[nCntBossParts].nCounterAttack = 60;
			g_aBossParts[nCntBossParts].state = BOSSPARTSSTATE_WAIT;

			float fAngle = atan2f(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);
			float fRadius = SQRTF(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);

			// 頂点座標の設定
			pVtx[0].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
			pVtx[0].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
			pVtx[1].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.z = 0.0f;
			break;		// ここでfor文を抜ける
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBossParts->Unlock();
}

//=============================================================================
//	敵の取得
//=============================================================================
BossParts* GetBossParts(void)
{
	return &g_aBossParts[0];		// 敵の情報の先頭アドレスを返す
}

//=============================================================================
//	敵のヒット処理
//=============================================================================
void HitBossParts(int nCntBossParts, int nDamage)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	g_aBossParts[nCntBossParts].nLife -= nDamage;		// 敵の体力を減らす
	if (g_aBossParts[nCntBossParts].nLife <= 0)
	{// 敵の体力がなくなったら
		//SetExplosion(g_aBossParts[nCntBossParts].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//SetParticle(g_aBossParts[nCntBossParts].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 50.0f, 10);
		SetRainbowParticle(g_aBossParts[nCntBossParts].pos, 3000.0f, 20, D3DX_PI, -D3DX_PI);
		AddScore(g_aBossParts[nCntBossParts].nScore);

		ResetBossParts(&g_aBossParts[nCntBossParts]);

		pVtx += (nCntBossParts * 4);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		PlaySound(SOUND_LABEL_SE_EXPLOSION001);
	}
	else
	{// 敵の体力が残っていたら
		g_aBossParts[nCntBossParts].state = BOSSPARTSSTATE_DAMAGE;		// ダメージ状態にする
		g_aBossParts[nCntBossParts].nCounterState = 12;

		PlaySound(SOUND_LABEL_SE_HIT001);

		pVtx += (nCntBossParts * 4);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBossParts->Unlock();
}

//=============================================================================
//	敵のリセット処理
//=============================================================================
void ResetBossParts(BossParts* pBossParts)
{
	// 初期化
	pBossParts->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pBossParts->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pBossParts->fRadius = 0.0f;
	pBossParts->bDisp = false;
	pBossParts->bUse = false;			// 使用していない状態にする
	pBossParts->bBlinking = false;
	pBossParts->fMove = 0.0f;
	pBossParts->nTimeLine = -1;
	pBossParts->nScore = 0;
	pBossParts->nLife = 3;
	pBossParts->nCounterAnim = 0;
	pBossParts->nPatternAnim = 0;
	pBossParts->nCounterState = 0;
	pBossParts->nCounterAttack = rand() % 300 + 150;
	pBossParts->nUseCounter = 600;
}