//=============================================================================
//
//	敵の処理 [enemy.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "enemy.h"
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
#define NUM_ENEMY		(5)			// 敵の種類
#define MAX_WARD		(256)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// 頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY];										// 敵の情報
int g_nCounterEnemy;

//====================================
//	敵の初期化処理
//====================================
void InitEnemy(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy004.png",
		&g_apTextureEnemy[4]);

	// 初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;			// 使用していない状態にする
		g_aEnemy[nCntEnemy].bBlinking = false;
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
	}

	g_nCounterEnemy = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,		// 敵の数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);

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
	g_pVtxBuffEnemy->Unlock();

	LoadEnemy();
}

//=============================================================================
//	敵の終了処理
//=============================================================================
void UninitEnemy(void)
{
	int nCntTex;
	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < NUM_ENEMY; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			// テクスチャの破棄
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
//	敵の描画処理
//=============================================================================
void DrawEnemy(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 敵が使用されている
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=============================================================================
//	敵の更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy = 0;
	static int nFrameCounter = 90;

	nFrameCounter++;

	Player* pPlayer = GetPlayer();		// プレイヤーの情報取得

	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 使用判定
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// もし敵が使用されていたら
			Player* pPlayer = GetPlayer();		// プレイヤーの情報取得
			float fAngleE_P = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x,		// プレイヤーと敵との角度算出
									 pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

			// 座標を更新
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
			if (nFrameCounter % 180 == 0)
			{
				g_aEnemy[nCntEnemy].move.x *= -1.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY - pCameraPos->y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY - pCameraPos->y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY - pCameraPos->y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY - pCameraPos->y, 0.0f);

			switch (g_aEnemy[nCntEnemy].state)
			{// 状態チェック
				// 通常状態
			case ENEMYSTATE_NORMAL:
				g_aEnemy[nCntEnemy].nCounterAttack--;
				if (g_aEnemy[nCntEnemy].nCounterAttack <= 0)
				{
					if (pPlayer->state != PLAYERSTATE_DEATH)
					{// プレイヤーが生存していれば
						// 弾発射とクールタイム設定
						SetEnemyBullet(g_aEnemy[nCntEnemy].pos, 5.0f, 150, BULLETTYPE_ENEMY,SHOTTYPE_AIM, fAngleE_P);
						g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;
					}
				}
				break;

				// ダメージ状態
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;		// 状態持続時間を減らす
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// 状態持続時間がなくなったら
					if (g_aEnemy[nCntEnemy].nLife <= 1)
					{// 体力が1以下なら危険状態に
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_WARNING;

						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_aEnemy[nCntEnemy].nCounterAttack = 0;			// すぐ攻撃させる
					}
					else
					{// 体力が十分ならノーマル状態に
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
						// 頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
				break;

			case ENEMYSTATE_WARNING:
				g_aEnemy[nCntEnemy].nCounterAttack--;
				if (g_aEnemy[nCntEnemy].nCounterAttack <= 0)
				{
					if (pPlayer->state != PLAYERSTATE_DEATH)
					{// プレイヤーが生存していたら
						// ランダムな角度を生成
						float fAngle = (float)((rand() % 200 + 1 - 100) * 0.01f);

						// 生成した角度に弾を設定
						SetEnemyBullet(g_aEnemy[nCntEnemy].pos, 10.0f, 150, BULLETTYPE_ENEMY, SHOTTYPE_NORMAL, fAngle * D3DX_PI);
						g_aEnemy[nCntEnemy].nCounterAttack = 6;			// クールタイムを設ける

						g_aEnemy[nCntEnemy].nCounterState--;		// 状態持続時間を減らす
						if (g_aEnemy[nCntEnemy].nCounterState <= 0)
						{// 持続時間がなくなったら
							// 状態切り替えと再セット
							g_aEnemy[nCntEnemy].bBlinking  = g_aEnemy[nCntEnemy].bBlinking ? false : true;
							g_aEnemy[nCntEnemy].nCounterState = 3;
						}

						// 頂点座標の設定
						pVtx[0].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) - ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) - ENEMY_SIZEY - pCameraPos->y, 0.0f);
						pVtx[1].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) + ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) - ENEMY_SIZEY - pCameraPos->y, 0.0f);
						pVtx[2].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) - ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) + ENEMY_SIZEY - pCameraPos->y, 0.0f);
						pVtx[3].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) + ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) + ENEMY_SIZEY - pCameraPos->y, 0.0f);
					}
				}
				break;
			}

			// テクスチャを更新
			g_aEnemy[nCntEnemy].nCounterAnim++;
			if ((g_aEnemy[nCntEnemy].nCounterAnim % 18) == 0)
			{
				g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % 2;
				g_aEnemy[nCntEnemy].nCounterAnim = 0;
			}

			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
			{
				// テクスチャ座標の設定
				//pVtx[0].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.5f);
				//pVtx[1].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 0.5f);
				//pVtx[2].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				//pVtx[3].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 1.0f);
			}
			else
			{
				// テクスチャ座標の設定
				//pVtx[0].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				//pVtx[1].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 0.0f);
				//pVtx[2].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.5f);
				//pVtx[3].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 0.5f);
			}

			if (pPlayer->state == PLAYERSTATE_DEATH)
			{// プレイヤーが死亡状態なら色を戻す
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
//=============================================================================
//	敵の設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{// 敵を使用していない
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].type = (ENEMYTYPE)nType;
			g_aEnemy[nCntEnemy].nLife = nLife;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);

			g_nCounterEnemy++;
			g_aEnemy[nCntEnemy].bUse = true;		// 敵が使用されている状態にする
			break;		// ここでfor文を抜ける
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
//	敵の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		// 敵の情報の先頭アドレスを返す
}

//=============================================================================
//	敵のヒット処理
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;		// 敵の体力を減らす
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{// 敵の体力がなくなったら
		//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 50.0f, 10);
		SetRainbowParticle(g_aEnemy[nCntEnemy].pos, 3000.0f, 20);
		AddScore(100);

		g_nCounterEnemy--;

		pVtx += (nCntEnemy * 4);

		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;			// 使用していない状態にする
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}
	else
	{// 敵の体力が残っていたら
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;		// ダメージ状態にする
		g_aEnemy[nCntEnemy].nCounterState = 12;

		PlaySound(SOUND_LABEL_SE_HIT);

		pVtx += (nCntEnemy * 4);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
//	敵のリセット処理
//=============================================================================
void ResetEnemy(void)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		// 初期化
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
//	敵の消去処理
//=============================================================================
void DeleteEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].bUse = false;			// 使用していない状態にする
	}
}

//=============================================================================
//	敵の生存確認処理
//=============================================================================
bool CheckEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	敵のロード処理
//=============================================================================
void LoadEnemy(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	pFile = fopen("data\\wave.txt", "r");

	if (pFile != NULL)
	{// 開けたら
		//ローカル変数宣言
		char aString[MAX_WARD];		// 文字列を読み込む
		char aStrRelease[3];		// 不要な = を読み込む

		// それぞれの値を読み込む
		D3DXVECTOR3 pos = {};
		int type = 0;
		int life = 0;
		int nData;

		while (true)
		{
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SETENEMY") == 0)
			{// SETENEMYを読み取った
				while (true)
				{
					nData = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{// POSを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);
					
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{// TYPEを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &type);

					}

					if (strcmp(&aString[0], "LIFE") == 0)
					{// LIFEを読み取った
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &life);
					}

					if (strcmp(&aString[0], "ENDSET") == 0)
					{// ENDSETを読み取った
						SetEnemy(pos, type, life);
						break;
					}
				}
			}

			

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}