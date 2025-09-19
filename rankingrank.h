//=============================================================================
//
//	ランキング画面処理 [ranking.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RANKINGRANK_H_
#define _RANKINGRANK_H_

#include "main.h"

// タイトル構造体の定義

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRankingRank(void);
void UninitRankingRank(void);
void UpdateRankingRank(void);
void DrawRankingRank(void);
void ResetRankingRank(void);
void SetRankingRank(int nScore);
int Sort_desc(const void* pCastScore1, const void* pCastScore2);

#endif
