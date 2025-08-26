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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INIT_POSX		(640.0f)			// 初期位置X
#define INIT_POSY		(500.0f)			// 初期位置Y

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;				// プレイヤーのテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		// プレイヤーの頂点バッファへのポインタ
Player g_player;										// プレイヤーの情報
float g_fLengthPlayer;									// 対角線の長さ
float g_fAnglePlayer;									// 対角線の角度
float g_fMovePlayerSize;								// 大きさの変動量
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
		"data\\TEXTURE\\player000.png",
		&g_pTexturePlayer);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	g_player.pos = D3DXVECTOR3(INIT_POSX, INIT_POSY, 0.0f);	// 位置
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向きを初期化する
	g_fMovePlayerSize = 0.0f;								// 大きさの変動量を初期化
	g_player.nLife = MAX_LIFE;								// ライフの初期化
	g_player.nStock = MAX_STOCK;							// 残機の初期化
	g_player.state = PLAYERSTATE_NORMAL;					// 状態の初期化
	g_player.bDisp = true;									// 表示状態の初期化

	// 対角線の長さを算出する
	g_fLengthPlayer = sqrtf(PLAYER_SIZEX * PLAYER_SIZEX + PLAYER_SIZEY * PLAYER_SIZEY) * 0.5f;

	// 対角線の角度を算出する
	g_fAnglePlayer = atan2f(PLAYER_SIZEX, PLAYER_SIZEY);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
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
			g_player.nLife = MAX_LIFE;					// ライフ初期化
			g_player.nCounterState = 60;				// 状態持続時間セット
			g_player.bDisp = true;						// 表示状態に
		}
		return;

	case PLAYERSTATE_NORMAL:
		// パーティクル設定
		SetParticle(g_player.pos, D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f), 45.0f, 50);
		CollisionPlayertoEnemy();
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;		// 状態持続時間を減らす
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
			// 敵のリセット
			ResetEnemy();

			float nCntX = 390.0f, nCntY = 100.0f;
			int nCnt1, nCnt2;

			for (nCnt1 = 0; nCnt1 < 6; nCnt1++, nCntX += 100.0f)
			{
				for (nCnt2 = 0, nCntY = 50.0f; nCnt2 < 5; nCnt2++, nCntY += 50.0f)
				{
					SetEnemy(D3DXVECTOR3(nCntX, nCntY, 0.0f), rand() % 4, rand() % 5 + 1);
				}
			}

			g_player.state = PLAYERSTATE_NORMAL;		// 状態の初期化
			g_player.nLife = 5;							// ライフの初期化
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

	if (GetKeyboardRepeat(DIK_SPACE) == true)
	{//SPACEキーが押された

		Option* pOption = GetOption();

		PlaySound(SOUND_LABEL_SE_SHOT);

		//弾の設定
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(0.0f, -BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);


		//弾の設定
		if (pOption->bUse == true)
		{
			SetEnemyBullet(pOption->pos, 15.0f, 50, BULLETTYPE_PLAYER, (pOption->rot.z + D3DX_PI));
		}

		//斜め
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * BULLET_MOVE, cosf(-D3DX_PI * 0.75f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * BULLET_MOVE, cosf(-D3DX_PI * 0.75f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * BULLET_MOVE, cosf(-D3DX_PI * 0.25f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * BULLET_MOVE, cosf(D3DX_PI * 0.25f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
	}

	if (GetKeyboardPress(DIK_A) == true)
	{//Aキーが押された
		if (GetKeyboardPress(DIK_W) == true)
		{//かつWキーが押された
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//かつSキーが押された
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x -= PLAYER_MOVEX;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//Dキーが押された
		if (GetKeyboardPress(DIK_W) == true)
		{//かつWキーが押された
			g_player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//かつSキーが押された
			g_player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x += PLAYER_MOVEX;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//Wキーが押された
		g_player.move.y -= PLAYER_MOVEY;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//Sキーが押された
		g_player.move.y += PLAYER_MOVEY;
	}

	if (GetJoypadRepeat(JOYKEY_A) == true)
	{//SPACEキーが押された

		Option* pOption = GetOption();

		PlaySound(SOUND_LABEL_SE_SHOT);

		//弾の設定
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(0.0f, -BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);


		//弾の設定
		if (pOption->bUse == true)
		{
			SetEnemyBullet(pOption->pos, 15.0f, 50, BULLETTYPE_PLAYER, (pOption->rot.z + D3DX_PI));
		}

		//斜め
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * BULLET_MOVE, cosf(-D3DX_PI * 0.75f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * BULLET_MOVE, cosf(-D3DX_PI * 0.75f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
		//SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * BULLET_MOVE, cosf(-D3DX_PI * 0.25f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
		//SetPlayerBullet(g_player.pos, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * BULLET_MOVE, cosf(D3DX_PI * 0.25f) * BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER);
	}

	// チャージショット
	if (GetJoypadRepeat(JOYKEY_B) == true)
	{
		static int ChargeCounter = 0;

		ChargeCounter++;

		if (ChargeCounter > 180)
		{
			SetParticle(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 75.0f, 50);
		}
		else
		{
			SetCollectParticle(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	// オプション生成
	if (GetJoypadTrigger(JOYKEY_X) == true || GetKeyboardTrigger(DIK_F1) == true)
	{
		SetOption(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, 0.0f),64.0f, 0.75f * D3DX_PI);
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
	}
	else
	{// デッドゾーン外なら
		// 初期化
		fJoypadMove = 0.0f;
		fThumbLX = 0.0f;
		fThumbLY = 0.0f;
	}

	// 移動量の更新
	g_player.move.x += fThumbLX;
	g_player.move.y -= fThumbLY;

	if (GetJoypadPress(JOYKEY_LEFT) == true)
	{//Aキーが押された
		if (GetJoypadPress(JOYKEY_UP) == true)
		{//かつWキーが押された
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetJoypadPress(JOYKEY_DOWN) == true)
		{//かつSキーが押された
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x -= PLAYER_MOVEX;
		}
	}
	else if (GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された
		if (GetJoypadPress(JOYKEY_UP) == true)
		{//かつWキーが押された
			g_player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetJoypadPress(JOYKEY_DOWN) == true)
		{//かつSキーが押された
			g_player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x += PLAYER_MOVEX;
		}
	}
	else if (GetJoypadPress(JOYKEY_UP) == true)
	{//Wキーが押された
		g_player.move.y -= PLAYER_MOVEY;
	}
	else if (GetJoypadPress(JOYKEY_DOWN) == true)
	{//Sキーが押された
		g_player.move.y += PLAYER_MOVEY;
	}

	//if (GetKeyboardPress(DIK_LEFT) == true)
	//{//左キーが押された
	//	//左に回転
	//	g_player.move.z += 0.015f;
	//}

	//if (GetKeyboardPress(DIK_RIGHT) == true)
	//{//右キーが押された
	//	//右に回転
	//	g_player.move.z += -0.015f;
	//}

	if (GetKeyboardPress(DIK_UP) == true)
	{//上キーが押された
		//拡大
		g_fMovePlayerSize += 1.5f;
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//下キーが押された
		//縮小
		g_fMovePlayerSize += -1.5f;
	}

	// テスト用敵消去
	if (GetKeyboardRepeat(DIK_DELETE) == true)
	{
		DeleteEnemy();
	}

	// テスト用スコア加算
	if (GetKeyboardPress(DIK_0) == true)
	{
		SetScore(0);
	}
	if (GetKeyboardPress(DIK_8) == true)
	{
		AddScore(1);
	}
	if (GetKeyboardPress(DIK_7) == true)
	{
		AddScore(10);
	}
	if (GetKeyboardPress(DIK_6) == true)
	{
		AddScore(100);
	}
	if (GetKeyboardPress(DIK_5) == true)
	{
		AddScore(1000);
	}
	if (GetKeyboardPress(DIK_4) == true)
	{
		AddScore(10000);
	}
	if (GetKeyboardPress(DIK_3) == true)
	{
		AddScore(100000);
	}
	if (GetKeyboardPress(DIK_2) == true)
	{
		AddScore(1000000);
	}
	if (GetKeyboardPress(DIK_1) == true)
	{
		AddScore(10000000);
	}

	// 位置を更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.rot.z += g_player.move.z;
	g_fLengthPlayer += g_fMovePlayerSize;

	// 慣性を更新
	g_player.move.x += (0.0f - g_player.move.x) * 0.1f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.1f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.1f;
	g_fMovePlayerSize += (0.0f - g_fMovePlayerSize) * 0.1f;

	if (g_fLengthPlayer < PLAYER_MIN_SIZE)
	{// もしサイズが一定以下だったら
		g_fLengthPlayer = PLAYER_MIN_SIZE;
	}

	// 端に行ったら反対に移動する
	if (g_player.pos.x + PLAYER_SIZEX < 0)
	{
		g_player.pos.x = SCREEN_WIDTH;
	}
	if (g_player.pos.x - PLAYER_SIZEX > SCREEN_WIDTH)
	{
		g_player.pos.x = 0;
	}

	if (g_player.pos.y + PLAYER_SIZEY < 0)
	{
		g_player.pos.y = SCREEN_HEIGHT;
	}
	if (g_player.pos.y - PLAYER_SIZEY > SCREEN_HEIGHT)
	{
		g_player.pos.y = 0;
	}

	// テクスチャを更新
	g_nCounterAnimPlayer++;
	if ((g_nCounterAnimPlayer % 5) == 0)
	{
		g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % 10;
	}

	// 頂点座標の更新
	VERTEX_2D* pVtx;			//頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
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
		g_player.nLife -= nDamage;
		if (g_player.nLife <= 0)
		{// プレイヤーのライフがなくなった
			SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetJoypadVibration(9000, 12000);
			if (g_player.nStock > 0)
			{// プレイヤーのストックが残っていたら
				g_player.state = PLAYERSTATE_WAIT;
				g_player.nStock--;
				g_player.nCounterState = 60;

				g_player.pos = D3DXVECTOR3(INIT_POSX, INIT_POSY, 0.0f);	// 位置の初期化
				g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量の初期化

					// 頂点座標の設定
				pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
				pVtx[3].pos.z = 0.0f;
			}
			else
			{
				g_player.state = PLAYERSTATE_DEATH;
			}

			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			g_player.bDisp = false;
		}
		else
		{// プレイヤーの体力が残っていたら
			g_player.state = PLAYERSTATE_DAMAGE;
			g_player.nCounterState = 5;
			SetJoypadVibration(6000, 9000);

			PlaySound(SOUND_LABEL_SE_HIT);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		}
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
			if (pEnemy->pos.x >= g_player.pos.x - ENEMY_SIZEX - (BULLET_SIZEX / 2) &&
				pEnemy->pos.y >= g_player.pos.y - ENEMY_SIZEY - (BULLET_SIZEY / 2) &&
				pEnemy->pos.x <= g_player.pos.x + ENEMY_SIZEX + (BULLET_SIZEX / 2) &&
				pEnemy->pos.y <= g_player.pos.y + ENEMY_SIZEY + (BULLET_SIZEY / 2) &&
				g_player.state == PLAYERSTATE_NORMAL)
			{// もし敵とプレイヤーがあたっていたら
				// ヒット処理
				HitPlayer(MAX_LIFE);
			}
		}
	}
}