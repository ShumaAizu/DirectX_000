//=============================================================================
//
//	スコア処理 [score.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//マクロ定義

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);

#endif
