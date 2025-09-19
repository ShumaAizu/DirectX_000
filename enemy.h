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
#define ENEMY_SIZEX		(32.0f)				// 敵のサイズX
#define ENEMY_SIZEY		(32.0f)				// 敵のサイズY

//*****************************************************************************
// 敵の状態
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// 通常状態
	ENEMYSTATE_APPEAR,			// 出現
	ENEMYSTATE_WAIT,			// 出現待ち
	ENEMYSTATE_DAMAGE,			// ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*****************************************************************************
// 敵の移動の状態
//*****************************************************************************
typedef enum
{
	ENEMY_MOVESTATE_WAIT = 0,			// 待機
	ENEMY_MOVESTATE_MOVE,				// 移動
	ENEMY_MOVESTATE_CHASE,				// 追跡
	ENEMY_MOVESTATE_MAX
}ENEMYMOVESTATE;

//*****************************************************************************
// 敵の種類
//*****************************************************************************
typedef enum
{
	ENEMYTYPE_FLAME = 0,		// 炎
	ENEMYTYPE_WATER,			// 水
	ENEMYTYPE_WIND,				// 風
	ENEMYTYPE_EARTH,			// 土
	ENEMYTYPE_BOSS,				// ボス
	ENEMYTYPE_MAX
}ENEMYTYPE;

//*****************************************************************************
// 敵の攻撃の種類
//*****************************************************************************
typedef enum
{
	ENEMY_ATTACKTYPE_AIM = 0,			// 自機狙い
	ENEMY_ATTACKTYPE_HOMING,			// ホーミング
	ENEMY_ATTACKTYPE_SCATTE,			// ばらまき
	ENEMY_ATTACKTYPE_MAX
}ENEMYATTACKTYPE;

//*****************************************************************************
// 敵構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	ENEMYSTATE state;				// 状態
	ENEMYTYPE type;					// 種類
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
}Enemy;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
void ResetEnemy(Enemy* pEnemy);
void DeleteEnemy(void);
bool CheckEnemy(void);
int* GetNumEnemy(void);

#endif