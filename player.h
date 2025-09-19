//=============================================================================
//
//	プレイヤー処理 [player.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_SIZE			(75.0f)		// プレイヤーのサイズ
#define PLAYER_SPEED		(0.5f)		// プレイヤーの移動量

//*****************************************************************************0
// プレイヤーの状態
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,				// 出現
	PLAYERSTATE_NORMAL,					// 通常
	PLAYERSTATE_DAMAGE,					// ダメージ
	PLAYERSTATE_WAIT,					// 出現待ち
	PLAYERSTATE_DEATH,					// 死亡
	PLAYERSTATE_MAX						// 総数
}PLAYERSTATE;

//*****************************************************************************
// プレイヤー構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 move;					// 移動量
	PLAYERSTATE state;					// 状態
	float fRadius;						// 半径
	float fSpeed;						// 速さ
	int nCounterState;					// 状態カウンター
	bool bDisp;							// 表示状態
}Player;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);
void CollisionPlayertoEnemy(void);

#endif
