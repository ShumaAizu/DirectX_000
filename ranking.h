//=============================================================================
//
//	ランキング画面処理 [ranking.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

// タイトル構造体の定義

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);

#endif
