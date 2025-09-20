//=============================================================================
//
//	弾の処理 [bullet.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "option.h"
#include "particle.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET		(2048)			// 弾の最大数

//*****************************************************************************
// ホーミングターゲットの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float fVector;
	float fAngle;
}Target;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// 頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];							// 弾の情報
int g_nCounterAnimBullet;								// アニメーションカウンター
int g_nPatternAnimBullet;								// アニメーションパターンNo.

//=============================================================================
//	弾の初期化処理
//=============================================================================
void InitBullet(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntBullet = 0;		// カウント用変数

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	// 初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].target = NULL;
		g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
		g_aBullet[nCntBullet].nHomingTimer = 0;
		g_aBullet[nCntBullet].nLife = 30;
		g_aBullet[nCntBullet].bUse = false;			// 使用していない状態にする

		// 対角線の長さを算出する
		g_aBullet[nCntBullet].fLength = sqrtf(BULLET_SIZEX * BULLET_SIZEX + BULLET_SIZEY * BULLET_SIZEY) * 0.5f;

		// 対角線の角度を算出する
		g_aBullet[nCntBullet].fAngle = atan2f(BULLET_SIZEX, BULLET_SIZEY);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,		// 弾の数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点情報の設定
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZEX, g_aBullet[nCntBullet].pos.y - BULLET_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZEX, g_aBullet[nCntBullet].pos.y - BULLET_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZEX, g_aBullet[nCntBullet].pos.y + BULLET_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZEX, g_aBullet[nCntBullet].pos.y + BULLET_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	弾の終了処理
//=============================================================================
void UninitBullet(void)
{
	// テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
//	弾の描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 使用判定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// 弾が使用されている
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=============================================================================
//	弾の更新処理
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet = 0;

	Bullet* pBullet = &g_aBullet[0];

	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 使用判定
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{// 弾が使用されている

			float fRotMove = atan2f(pBullet->move.x, pBullet->move.y);

			switch (pBullet->type)
			{
			case BULLETTYPE_PLAYER:
				// エフェクトの設定
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 1.0f, 0.5f), 40.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.15f, 0.1f, 0.15f, 0.5f), 30.0f, 50);
				//SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 2500.0f, 5, fRotMove - D3DX_PI, fRotMove -D3DX_PI);
				// 敵との当たり判定
				CollisionEnemy(pBullet);
				break;

			case BULLETTYPE_FLAME:
				// エフェクトの設定
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f), 15.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f), 30.0f, 50);
				SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f), 300.0f, 3, D3DX_PI, -D3DX_PI);
				CollisionPlayer(pBullet);
				break;

			case BULLETTYPE_WATER:
				// エフェクトの設定
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f), 15.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f), 30.0f, 50);
				SetParticle(pBullet->pos, D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f), 300.0f, 3, D3DX_PI, -D3DX_PI);
				CollisionPlayer(pBullet);
				break;

			case BULLETTYPE_WIND:
				// エフェクトの設定
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.8f, 1.0f, 0.4f, 1.0f), 15.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.8f, 1.0f, 0.4f, 1.0f), 30.0f, 50);
				SetParticle(pBullet->pos, D3DXCOLOR(0.65f, 0.9f, 0.5f, 1.0f), 300.0f, 3, D3DX_PI, -D3DX_PI);
				CollisionPlayer(pBullet);
				break;

			case BULLETTYPE_EARTH:
				// エフェクトの設定
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.4f, 0.2f, 0.0f, 1.0f), 15.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.4f, 0.2f, 0.0f, 1.0f), 30.0f, 50);
				SetParticle(pBullet->pos, D3DXCOLOR(0.4f, 0.2f, 0.0f, 1.0f), 300.0f, 3, D3DX_PI, -D3DX_PI);
				CollisionPlayer(pBullet);
				break;

			case BULLETTYPE_BOSS:
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 15.0f, 50);
				SetRainbowParticle(pBullet->pos, 3000.0f, 3, D3DX_PI, -D3DX_PI);
				CollisionPlayer(pBullet);
				break;

			}

			switch (pBullet->shottype)
			{
			case SHOTTYPE_HOMING:
				float fLength = NULL;			// 対角線の長さ
				float fLengthLast = 250.0f;		// 一番短い対角線の長さ
				Enemy* pEnemy = GetEnemy();		// 敵の情報取得
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false || pEnemy->state == ENEMYSTATE_APPEAR)
					{
						continue;
					}

					// 対角線の長さ算出
					fLength = SQRTF((pEnemy->pos.x - pBullet->pos.x), (pEnemy->pos.y - pBullet->pos.y));
					if (fLength < fLengthLast)
					{// もし一番短い対角線の長さが更新されたら
						fLengthLast = fLength;
						pBullet->target = nCntEnemy;
					}
				}

				if (fLengthLast >= 300.0f)
				{
					g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
				}

				pEnemy = GetEnemy();		// 敵の情報取得

				if ((pEnemy + pBullet->target)->bUse == false)
				{
					pBullet->shottype = SHOTTYPE_NORMAL;
				}
				else
				{
					float fRotDest, fRotDiff;

					fRotMove = atan2f(pBullet->move.x, pBullet->move.y);
					fRotDest = atan2f((pEnemy + pBullet->target)->pos.x - pBullet->pos.x, (pEnemy + pBullet->target)->pos.y - pBullet->pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += D3DX_PI * 2;
					}
					else if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= D3DX_PI * 2;
					}

					if (pBullet->nHomingTimer <= 0)
					{
						fRotMove += fRotDiff * 0.5f;
					}
					else
					{
						pBullet->nHomingTimer--;
					}

					if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += D3DX_PI * 2;
					}
					else if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= D3DX_PI * 2;
					}

					pBullet->move.x = sinf(fRotMove) * BULLET_MOVE;
					pBullet->move.y = cosf(fRotMove) * BULLET_MOVE;
				}

				break;
			}

			// 位置の更新
			pBullet->pos += pBullet->move;

			// 頂点座標の設定
			pVtx[0].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z + D3DX_PI + pBullet->fAngle) * pBullet->fLength;
			pVtx[0].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z + D3DX_PI + pBullet->fAngle) * pBullet->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z - D3DX_PI - pBullet->fAngle) * pBullet->fLength;
			pVtx[1].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z - D3DX_PI - pBullet->fAngle) * pBullet->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.z = 0.0f;

			// 使用判定
			if (g_aBullet[nCntBullet].pos.x - pCameraPos->x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x - pCameraPos->x < 0 ||
				g_aBullet[nCntBullet].pos.y - pCameraPos->y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y - pCameraPos->y < 0)
			{// もし弾が画面外に出たら
				g_aBullet[nCntBullet].bUse = false;		// 弾を使用していない状態にする
			}

			pBullet->nLife--;				// 寿命を削る

			if (pBullet->nLife < 0)
			{//もし寿命が尽きたら
				SetRainbowParticle(pBullet->pos, 3000.0f, 5, D3DX_PI, -D3DX_PI);
				pBullet->bUse = false;		// 弾を使用していない状態にする
			}

			if (pBullet->bUse == false)
			{// 弾が使用していない状態になったら
				// 初期化
				pBullet->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pBullet->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pBullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pBullet->target = NULL;
				pBullet->nLife = 30;
			}
		}

		pVtx += 4;			// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	敵の弾の設定
//=============================================================================
void SetEnemyBullet(D3DXVECTOR3 pos, float fmove, int nLife, BULLETTYPE type,SHOTTYPE shottype, float fAngleE_P)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// 弾を使用していない
			g_aBullet[nCntBullet].pos = pos;

			g_aBullet[nCntBullet].shottype = shottype;

			switch (g_aBullet[nCntBullet].shottype)
			{
			case SHOTTYPE_HOMING:				// ホーミング弾
				float fLength = NULL;			// 対角線の長さ
				float fLengthLast = 250.0f;		// 一番短い対角線の長さ
				Enemy* pEnemy = GetEnemy();		// 敵の情報取得
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false || pEnemy->state == ENEMYSTATE_APPEAR)
					{
						continue;
					}

					// 対角線の長さ算出
					fLength = SQRTF((pEnemy->pos.x - g_aBullet[nCntBullet].pos.x), (pEnemy->pos.y - g_aBullet[nCntBullet].pos.y));
					if (fLength < fLengthLast)
					{// もし一番短い対角線の長さが更新されたら
						fLengthLast = fLength;
						g_aBullet[nCntBullet].target = nCntEnemy;
					}
				}

				if (fLengthLast >= 250.0f)
				{
					g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
				}
			}

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].move.x = sinf(fAngleE_P) * fmove;
			g_aBullet[nCntBullet].move.y = cosf(fAngleE_P) * fmove;

			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;		// 弾が使用されている状態にする
			break;		// ここでfor文を抜ける
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	プレイヤーの弾の設定
//=============================================================================
void SetPlayerBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, SHOTTYPE shottype)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// 弾を使用していない
			g_aBullet[nCntBullet].pos = pos;

			g_aBullet[nCntBullet].shottype = shottype;

			switch (g_aBullet[nCntBullet].shottype)
			{
			case SHOTTYPE_HOMING:				// ホーミング弾
				float fLength = NULL;			// 対角線の長さ
				float fLengthLast = 250.0f;		// 一番短い対角線の長さ
				Enemy* pEnemy = GetEnemy();		// 敵の情報取得
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false || pEnemy->state == ENEMYSTATE_APPEAR)
					{
						continue;
					}

					// 対角線の長さ算出
					fLength = SQRTF((pEnemy->pos.x - g_aBullet[nCntBullet].pos.x), (pEnemy->pos.y - g_aBullet[nCntBullet].pos.y));
					if (fLength < fLengthLast)
					{// もし一番短い対角線の長さが更新されたら
						fLengthLast = fLength;
						g_aBullet[nCntBullet].target = nCntEnemy;
					}
				}

				if (fLengthLast >= 250.0f)
				{
					g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
				}
			}

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].move = move;

			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;		// 弾が使用されている状態にする
			break;		// ここでfor文を抜ける
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

}

//=============================================================================
//	ホーミング弾の設定
//=============================================================================
void SetHomingBullet(D3DXVECTOR3 pos, BULLETTYPE type, float fmove, float fAngle, int nLife, int nHomingTimer)
{

	D3DXVECTOR3* pCameraPos = GetCamera();

	Player* pPlayer = GetPlayer();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// 弾を使用していない
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].shottype = SHOTTYPE_HOMING;

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].move.x = sinf(fAngle) * fmove + pPlayer->fSpeed;
			g_aBullet[nCntBullet].move.y = cosf(fAngle) * fmove + pPlayer->fSpeed;

			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nHomingTimer = nHomingTimer;
			g_aBullet[nCntBullet].bUse = true;		// 弾が使用されている状態にする
			break;		// ここでfor文を抜ける
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

}

//=============================================================================
//	敵との当たり判定
//=============================================================================
void CollisionEnemy(Bullet *pBullet)
{
	// 敵情報の取得
	Enemy *pEnemy = GetEnemy();

	// 使用判定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// 敵が使用されている
			if (pBullet->pos.x >= pEnemy->pos.x - pEnemy->fRadius - (BULLET_SIZEX / 2) &&
				pBullet->pos.y >= pEnemy->pos.y - pEnemy->fRadius - (BULLET_SIZEY / 2) &&
				pBullet->pos.x <= pEnemy->pos.x + pEnemy->fRadius + (BULLET_SIZEX / 2) &&
				pBullet->pos.y <= pEnemy->pos.y + pEnemy->fRadius + (BULLET_SIZEY / 2) &&
				pEnemy->state != ENEMYSTATE_APPEAR && pEnemy->state != ENEMYSTATE_WAIT)
			{// もし弾が敵にあたっていたら
				// 敵のヒット処理
				switch (pBullet->shottype)
				{
				case SHOTTYPE_HOMING:
					HitEnemy(nCntEnemy, 1);
					break;

				case SHOTTYPE_NORMAL:
					HitEnemy(nCntEnemy, 3);
					break;
				}
				SetRainbowParticle(pBullet->pos, 3000.0f, 5, D3DX_PI, -D3DX_PI);
				pBullet->bUse = false;
			}
		}
	}
}

//=============================================================================
//	プレイヤーとの当たり判定
//=============================================================================
void CollisionPlayer(Bullet* pBullet)
{
	// プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	if (pBullet->pos.x >= pPlayer->pos.x - ENEMY_SIZEX - (BULLET_SIZEX / 2) &&
		pBullet->pos.y >= pPlayer->pos.y - ENEMY_SIZEY - (BULLET_SIZEY / 2) &&
		pBullet->pos.x <= pPlayer->pos.x + ENEMY_SIZEX + (BULLET_SIZEX / 2) &&
		pBullet->pos.y <= pPlayer->pos.y + ENEMY_SIZEY + (BULLET_SIZEY / 2) &&
		pPlayer->state == PLAYERSTATE_NORMAL)
	{// もし弾がプレイヤーにあたっていたら
		// 敵のヒット処理
		HitPlayer(1000);
		pBullet->bUse = false;
	}
}

//=============================================================================
//	オプションとの当たり判定
//=============================================================================
void CollisionOption(Bullet* pBullet)
{
	// プレイヤー情報の取得
	Option* pOption = GetOption();

	// 使用判定
	for (int nCntEnemy = 0; nCntEnemy < MAX_OPTION; nCntEnemy++, pOption++)
	{
		if (pOption->bUse == true)
		{// オプションが使用されている
			if (pBullet->pos.x >= pOption->pos.x - ENEMY_SIZEX - (BULLET_SIZEX / 2) &&
				pBullet->pos.y >= pOption->pos.y - ENEMY_SIZEY - (BULLET_SIZEY / 2) &&
				pBullet->pos.x <= pOption->pos.x + ENEMY_SIZEX + (BULLET_SIZEX / 2) &&
				pBullet->pos.y <= pOption->pos.y + ENEMY_SIZEY + (BULLET_SIZEY / 2) &&
				pOption->state == SUBOPTIONSTATE_BARRIER)
			{// もし弾がオプションにあたっていたら
				// オプションのヒット処理
				HitOption(nCntEnemy);
				pBullet->bUse = false;
			}
		}
	}
}