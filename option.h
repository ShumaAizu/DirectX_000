//=============================================================================
//
//	オプションの処理 [option.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OPTION_H_
#define _OPTION_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OPTION			(1)

//*****************************************************************************
// オプションの状態
//*****************************************************************************
typedef enum
{
	OPTIONSTATE_NORMAL = 0,				// 通常
	OPTIONSTATE_WAIT,					// 出現待ち
	OPTIONSTATE_COOLTIME,				// 機能不能
	OPTIONSTATE_STAY,					// 待機
	OPTIONSTATE_MAX
}OPTIONSTATE;
//*****************************************************************************
// オプション構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 向き
	float fDistance;		// プレイヤーとの距離
	float fAngle;			// プレイヤーとの角度
	OPTIONSTATE state;		// 状態
	int nCounterState;		// 状態カウンター
	bool bUse;				// 使用しているかどうか
	bool bDisp;				// 表示状態
}Option;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetOption(D3DXVECTOR3 pos, float fDistance, float fAngle);
Option *GetOption(void);
void HitOption(int nCntOption);

#endif
