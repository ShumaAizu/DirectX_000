//=============================================================================
//
//	パワーアップ処理 [powerup.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "powerup.h"
#include "input.h"
#include "effect.h"
#include "option.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPTION_COST				(1500)		// オプションを生成するために必要なスコア
#define SPEEDUP_COST			(5)			// 速度を上げるために必要なスコア
#define SPEED_LIMIT				(1.25f)		// 速度上限
#define POWERUP_POSX			(640.0f)
#define POWERUP_POSY			(608.0f)
#define POWERUP_SIZEX			(64.0f)
#define POWERUP_SIZEY			(64.0f)

//*****************************************************************************
// パワーアップ構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	bool bDisp;			// 表示状態
}PowerUp;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
bool g_bSpeedUp = false;
LPDIRECT3DTEXTURE9 g_pTexturePowerUp = NULL;				// パワーアップのテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPowerUp = NULL;			// パワーアップの頂点バッファへのポインタ
PowerUp g_powerup[MAX_OPTION] = {};
bool bPowerUpTutorialState = false;

//====================================
//	パワーアップの初期化処理
//====================================
void InitPowerup(void)
{
	g_bSpeedUp = false;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntPowerUp = 0;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\OptionIcon.png", &g_pTexturePowerUp);


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OPTION,		// 最大数だけ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPowerUp,
		NULL);

	bPowerUpTutorialState = false;

	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPowerUp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPowerUp = 0; nCntPowerUp < MAX_OPTION; nCntPowerUp++)
	{
		g_powerup[nCntPowerUp].bDisp = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX), POWERUP_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX) + POWERUP_SIZEX, POWERUP_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX), POWERUP_POSY + POWERUP_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX) + POWERUP_SIZEX, POWERUP_POSY + POWERUP_SIZEY, 0.0f);

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
	g_pVtxBuffPowerUp->Unlock();
}

//====================================
//	パワーアップの終了処理
//====================================
void UninitPowerup(void)
{
		// テクスチャの破棄
	if (g_pTexturePowerUp != NULL)
	{
		g_pTexturePowerUp->Release();
		g_pTexturePowerUp = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPowerUp != NULL)
	{
		g_pVtxBuffPowerUp->Release();
		g_pVtxBuffPowerUp = NULL;
	}
}

//====================================
//	パワーアップの更新処理
//====================================
void UpdatePowerup(void)
{
	D3DXVECTOR3* pOptionStandardRot = GetStandardRot();
	Player* pPlayer = GetPlayer();
	int nUseOption = GetUseOption();

	if (GetTutorialEvent() == TUTORIALEVENT_POWERUP)
	{
		bPowerUpTutorialState = true;
	}

	if (bPowerUpTutorialState == false)
	{
		return;
	}

	for (int nCntPowerUp = 0; nCntPowerUp < MAX_OPTION; nCntPowerUp++)
	{
		if (nCntPowerUp < nUseOption)
		{
			g_powerup[nCntPowerUp].bDisp = false;
		}
	}


	if (GetJoypadTrigger(JOYKEY_X) == true || GetKeyboardTrigger(DIK_LCONTROL) == true || GetKeyboardTrigger(DIK_RCONTROL) == true)
	{
		if (GetScore() >= OPTION_LOWESTLINE && nUseOption < MAX_OPTION)
		{
			if (GetGameState() == GAMESTATE_TUTORIAL && nUseOption > 0)
			{

			}
			else
			{
				SetOption(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, 0.0f), 64.0f, (pOptionStandardRot->z) * D3DX_PI);
				SubScore(OPTION_COST);
			}
		}
	}

	g_bSpeedUp = false;

	if (GetJoypadPress(JOYKEY_B) == true || GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
	{
		if (GetScore() >= SPEEDUP_LOWESTLINE)
		{
			PlaySound(SOUND_LABEL_SE_SPEEDUP000);
			if (pPlayer->fSpeed < SPEED_LIMIT)
			{
				pPlayer->fSpeed += 0.025f;
			}
			g_bSpeedUp = true;
			SubScore(SPEEDUP_COST);
		}
	}
	
	if (pPlayer->fSpeed >= PLAYER_SPEED && g_bSpeedUp == false)
	{
		pPlayer->fSpeed -= 0.25f;

		if (pPlayer->fSpeed < PLAYER_SPEED)
		{
			pPlayer->fSpeed = PLAYER_SPEED;
		}
	}
}

//====================================
//	パワーアップの描画処理
//====================================
void DrawPowerup(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPowerUp, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePowerUp);

	for (int nCntPowerUp = 0; nCntPowerUp < MAX_OPTION; nCntPowerUp++)
	{
		if (g_powerup[nCntPowerUp].bDisp == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPowerUp * 4, 2);
		}
	}
}

//====================================
//	パワーアップの情報取得
//====================================
bool GetPowerUp(void)
{
	return g_bSpeedUp;
}