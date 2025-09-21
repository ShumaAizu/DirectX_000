//=============================================================================
//
//	敵の処理 [bossparts.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _BOSSPARTS_H_
#define _BOSSPARTS_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BOSSPARTS		(12)				// 敵の最大数
#define BOSSPARTS_SIZEX		(32.0f)				// 敵のサイズX
#define BOSSPARTS_SIZEY		(32.0f)				// 敵のサイズY

//*****************************************************************************
// 敵の状態
//*****************************************************************************
typedef enum
{
	BOSSPARTSSTATE_NORMAL = 0,		// 通常状態
	BOSSPARTSSTATE_APPEAR,			// 出現
	BOSSPARTSSTATE_WAIT,			// 出現待ち
	BOSSPARTSSTATE_DAMAGE,			// ダメージ状態
	BOSSPARTSSTATE_MAX
}BOSSPARTSSTATE;

//*****************************************************************************
// 敵の種類
//*****************************************************************************
typedef enum
{
	BOSSPARTSTYPE_WING = 0,		// 翼
	BOSSPARTSTYPE_HAND,			// 手
	BOSSPARTSTYPE_FLARE,		// 炎
	BOSSPARTSTYPE_MAX
}BOSSPARTSTYPE;

//*****************************************************************************
// 敵構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	BOSSPARTSSTATE state;			// 状態
	BOSSPARTSTYPE type;				// 種類
	float fRadius;					// 半径
	float fMove;					// 移動量
	int nTimeLine;					// タイムライン
	int nScore;						// 撃破時獲得スコア
	int nLife;						// 体力
	int nCounterAnim;				// アニメーションカウンター
	int nPatternAnim;				// アニメーションNO.
	int nCounterState;				// 状態カウンター
	int nCounterAttack;				// 攻撃カウンター
	int nUseCounter;				// 使用不能までのカウンター
	bool bDisp;						// 表示状態
	bool bBlinking;					// 点滅状態
	bool bUse;						// 使用しているかどうか
}BossParts;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBossParts(void);
void UninitBossParts(void);
void UpdateBossParts(void);
void DrawBossParts(void);
void SetBossParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore);
BossParts* GetBossParts(void);
void HitBossParts(int nCntBossParts, int nDamage);
void ResetBossParts(BossParts* pBossParts);

#endif