//=============================================================================
//
//	敵の処理 [enemy.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY		(128)				// 敵の最大数
#define ENEMY_SIZEX		(25)				// 敵のサイズX
#define ENEMY_SIZEY		(25)				// 敵のサイズY

//*****************************************************************************
// 敵の状態
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// 通常状態
	ENEMYSTATE_DAMAGE,			// ダメージ状態
	ENEMYSTATE_WARNING,			// 危険状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*****************************************************************************
// 敵構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	int nType;				// 種類
	int nLife;				// 体力
	ENEMYSTATE state;		// 状態
	int nCounterAnim;		// アニメーションカウンター
	int nPatternAnim;		// アニメーションNO.
	int nCounterState;		// 状態カウンター
	int nCounterAttack;		// 攻撃カウンター
	bool bBlinking;			// 点滅状態
	bool bUse;				// 使用しているかどうか
}Enemy;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
void ResetEnemy(void);
void DeleteEnemy(void);
bool CheckEnemy(void);
void LoadEnemy(void);

#endif
