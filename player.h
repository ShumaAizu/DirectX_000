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
#define PLAYER_SIZEX		(30)		// プレイヤーのサイズX
#define PLAYER_SIZEY		(30)		// プレイヤーのサイズY
#define PLAYER_MIN_SIZE		(30)		// プレイヤーの最小サイズ
#define PLAYER_MOVEX		(1.5f)		// プレイヤーの移動量X
#define PLAYER_MOVEY		(1.5f)		// プレイヤーの移動量Y


//*****************************************************************************
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
	int nLife;							// 体力
	int nStock;							// 残機
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
