//=============================================================================
//
//	カメラ座標処理 [camera.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "camera.h"
#include "player.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3 g_camerapos = {};		// カメラ座標

void InitCamera(void)
{
	g_camerapos = {};
}

void UninitCamera(void)
{

}

void UpdateCamera(void)
{
	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// カメラの位置(プレイヤーの位置)からスクリーンの原点を算出
	g_camerapos.x = pPlayer->pos.x - SCREEN_WIDTH / 2;
	g_camerapos.y = pPlayer->pos.y - SCREEN_HEIGHT / 2;
}

void DrawCamera(void)
{

}

D3DXVECTOR3* GetCamera(void)
{
	return &g_camerapos;
}