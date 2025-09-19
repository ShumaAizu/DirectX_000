//=============================================================================
//
//	プレイヤー処理 [player.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "option.h"
#include "life.h"
#include "stock.h"
#include <stdlib.h>
#include "particle.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INIT_POSX		(WARLD_WIDTH / 2)			// 初期位置X
#define INIT_POSY		(WARLD_HEIGHT / 2)			// 初期位置Y

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;				// プレイヤーのテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		// プレイヤーの頂点バッファへのポインタ
float g_fLengthPlayer = NULL;
float g_fAnglePlayer = NULL;
Player g_player;										// プレイヤーの情報
int g_nCounterAnimPlayer;								// アニメーションカウンター
int g_nPatternAnimPlayer;								// アニメーションパターンNo.

//=============================================================================
//	プレイヤー初期化処理
//=============================================================================
void InitPlayer(void)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\player0001.png",
		&g_pTexturePlayer);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	g_fLengthPlayer = SQRTF(PLAYER_SIZE, PLAYER_SIZE);
	g_fAnglePlayer = atan2f(PLAYER_SIZE, PLAYER_SIZE);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	g_player.pos = D3DXVECTOR3(INIT_POSX, INIT_POSY, 0.0f);	// 位置
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 3.14f);			// 向きを初期化する
	g_player.fSpeed = PLAYER_SPEED;							// 速度を初期化
	g_player.fRadius = PLAYER_SIZE;							// プレイヤーの大きさの初期化
	g_player.state = PLAYERSTATE_NORMAL;					// 状態の初期化
	g_player.bDisp = true;									// 表示状態の初期化

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x - g_player.fRadius;
	pVtx[0].pos.y = g_player.pos.y - g_player.fRadius;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + g_player.fRadius;
	pVtx[1].pos.y = g_player.pos.y - g_player.fRadius;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - g_player.fRadius;
	pVtx[2].pos.y = g_player.pos.y + g_player.fRadius;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + g_player.fRadius;
	pVtx[3].pos.y = g_player.pos.y + g_player.fRadius;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
//	プレイヤーの終了処理
//=============================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================
//	プレイヤーの描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	if (g_player.bDisp == true)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
							   0,						//描画するプリミティブ数
							   2);						//描画する最初の頂点インデックス
	}
}

//=============================================================================
//	プレイヤーの更新処理
//=============================================================================
void UpdatePlayer(void)
{
	float fRot = g_player.rot.z - D3DX_PI;	// プレイヤーの向きの反対

	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.nCounterState--;		// 状態持続時間を減らす

		if (g_player.nCounterState % 3 == 0)
		{
			g_player.bDisp = g_player.bDisp ? false : true;
		}

		if (g_player.nCounterState <= 0 && g_player.bDisp == true)
		{
			g_player.state = PLAYERSTATE_NORMAL;		// 状態をノーマルに
		}
		break;

	case PLAYERSTATE_WAIT:
		g_player.nCounterState--;		// 状態持続時間を減らす
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_APPEAR;		// 状態を出現に
			g_player.nCounterState = 60;				// 状態持続時間セット
			g_player.bDisp = true;						// 表示状態に
		}
		return;

	case PLAYERSTATE_NORMAL:
		// パーティクル設定
		if (fRot < -D3DX_PI)
		{
			fRot += D3DX_PI * 2;
		}
		else if (fRot > D3DX_PI)
		{
			fRot -= D3DX_PI * 2;
		}

		//SetRainbowParticle(g_player.pos, 3000.0f, 15);
		SetParticle(g_player.pos, D3DXCOLOR(0.25f, 0.1f, 0.25f, 1.0f), 3000.0f, 30, fRot, fRot);
		CollisionPlayertoEnemy();
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;		// 状態持続時間を減らす
		SetRainbowParticle(g_player.pos, 5000.0f, 5, D3DX_PI, -D3DX_PI);
		if (g_player.nCounterState <= 0)
		{// 状態持続時間がなくなったら
			g_player.state = PLAYERSTATE_APPEAR;		// 初期化
			g_player.nCounterState = 30;				// 状態持続時間セット
			// 頂点座標の更新
			VERTEX_2D* pVtx;			// 頂点情報へのポインタ

			// 頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロックする
			g_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_DEATH:
#ifdef _DEBUG

		// テスト用セットエネミー
		if (GetKeyboardTrigger(DIK_L) == true)
		{

			g_player.state = PLAYERSTATE_NORMAL;		// 状態の初期化
			g_player.bDisp = true;						// 表示状態の初期化

			// 頂点座標の更新
			VERTEX_2D* pVtx;			// 頂点情報へのポインタ

			// 頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロックする
			g_pVtxBuffPlayer->Unlock();
		}
#endif
		return;
	}

	// オプションの標準向き取得
	D3DXVECTOR3* pOptionStandardRot = GetStandardRot();

	if (GetKeyboardRepeat(DIK_SPACE) == true || GetJoypadRepeat(JOYKEY_A) == true)
	{//SPACEキーが押された

		Option* pOption = GetOption();

		PlaySound(SOUND_LABEL_SE_SHOT001);

		//弾の設定
		SetHomingBullet(g_player.pos - D3DXVECTOR3(20.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 15.0f, g_player.rot.z + (0.15f * D3DX_PI), 250, 15);
		SetHomingBullet(g_player.pos + D3DXVECTOR3(20.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 15.0f, g_player.rot.z - (0.15f * D3DX_PI), 250, 15);

		//弾の設定
		for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
		{
			if ((pOption + nCntOption)->bUse == true)
			{
				//SetEnemyBullet((pOption + nCntOption)->pos, 15.0f, 50, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL, (((pOption + nCntOption)->rot.z + pOptionStandardRot->z)* D3DX_PI));
				SetEnemyBullet((pOption + nCntOption)->pos, 25.0f, 50, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL, ((pOption + nCntOption)->rot.z* D3DX_PI + g_player.rot.z));

			}
		}
	}

	float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

	float fMoveKeyboard;

	D3DXVECTOR3 KeyboardMove = {0.0f, 0.0f, 0.0f};

	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//Aキーが押された
		KeyboardMove.x -= 1;
	}

	if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された
		KeyboardMove.x += 1;
	}

	if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{//Wキーが押された
		KeyboardMove.y -= 1;
	}

	if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{//Sキーが押された
		KeyboardMove.y += 1;
	}

	fMoveKeyboard = SQRTF(KeyboardMove.x, KeyboardMove.y);

	if (fMoveKeyboard != 0)
	{
		fRotMove = g_player.rot.z;								// 今の向き
		fRotDest = atan2f(KeyboardMove.x, KeyboardMove.y);		// 目的地への向き
		fRotDiff = fRotDest - fRotMove;							// 差分

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		fRotMove += fRotDiff * 0.65f;

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		// 移動量の更新
		g_player.move.x += sinf(fRotMove) * g_player.fSpeed;
		g_player.move.y += cosf(fRotMove) * g_player.fSpeed;
		g_player.rot.z = atan2f(g_player.move.x, g_player.move.y);
	}

	// ジョイパッドの状態を取得
	XINPUT_STATE* pJoyState = GetJoypadState();

	// スティック操作
	float fThumbLX = pJoyState->Gamepad.sThumbLX;	// LスティックのXの値
	float fThumbLY = pJoyState->Gamepad.sThumbLY;	// LスティックのYの値

	float fJoypadMove;		// ベクトル

	// ベクトルの長さを算出
	fJoypadMove = sqrtf(fThumbLX * fThumbLX + fThumbLY * fThumbLY);

	if (fJoypadMove > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// もしデッドゾーン以内なら
		// 正規化
		fThumbLX = (fThumbLX) / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		fThumbLY = (fThumbLY) / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		fRotMove = g_player.rot.z;					// 今の向き
		fRotDest = atan2f(fThumbLX, -fThumbLY);		// 目的地への向き
		fRotDiff = fRotDest - fRotMove;				// 差分

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		fRotMove += fRotDiff * 0.65f;

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		// 移動量の更新
		g_player.move.x += sinf(fRotMove) * g_player.fSpeed;
		g_player.move.y += cosf(fRotMove) * g_player.fSpeed;
		g_player.rot.z = atan2f(g_player.move.x, g_player.move.y);
	}
	else
	{// デッドゾーン外なら
		// 初期化
		fJoypadMove = 0.0f;
		fThumbLX = 0.0f;
		fThumbLY = 0.0f;
	}

	g_player.pos += g_player.move;

	 //慣性を更新
	g_player.move.x += (0.0f - g_player.move.x) * 0.05f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.05f;

	// 端に行ったら反対に移動する
	if (g_player.pos.x < 0)
	{
		g_player.pos.x = 0;
	}
	if (g_player.pos.x > WARLD_WIDTH)
	{
		g_player.pos.x = WARLD_WIDTH;
	}

	if (g_player.pos.y < 0)
	{
		g_player.pos.y = 0;
	}
	if (g_player.pos.y > WARLD_HEIGHT)
	{
		g_player.pos.y = WARLD_HEIGHT;
	}

	D3DXVECTOR3* pCameraPos = GetCamera();

	// 頂点座標の更新
	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

}

//=============================================================================
//	プレイヤー情報の取得
//=============================================================================
Player* GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
//	プレイヤーのヒット処理
//=============================================================================
void HitPlayer(int nDamage)
{
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_player.state != PLAYERSTATE_DEATH)
	{
		SubScore(nDamage);
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;
		SetJoypadVibration(6000, 9000, 30);

		PlaySound(SOUND_LABEL_SE_HIT);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
//	プレイヤーと敵との当たり判定
//=============================================================================
void CollisionPlayertoEnemy(void)
{
	// 敵の情報を取得
	Enemy* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->pos.x >= g_player.pos.x - pEnemy->fRadius - (PLAYER_SIZE / 2) &&
				pEnemy->pos.y >= g_player.pos.y - pEnemy->fRadius - (PLAYER_SIZE / 2) &&
				pEnemy->pos.x <= g_player.pos.x + pEnemy->fRadius + (PLAYER_SIZE / 2) &&
				pEnemy->pos.y <= g_player.pos.y + pEnemy->fRadius + (PLAYER_SIZE / 2) &&
				g_player.state == PLAYERSTATE_NORMAL && pEnemy->state != ENEMYSTATE_APPEAR)
			{// もし敵とプレイヤーがあたっていたら
				// ヒット処理
				HitPlayer(1500);
			}
		}
	}
}