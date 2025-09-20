//=============================================================================
//
//	�����L���O��ʏ��� [ranking.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RANKINGRANK_H_
#define _RANKINGRANK_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKINGRANK_DIFF			(65.0f)
#define RANKINGRANK_SIZEX			(100.0f)
#define RANKINGRANK_SIZEY			(50.0f)
#define RANKINGRANK_POSX			(420.0f)
#define RANKINGRANK_POSY			(50.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRankingRank(void);
void UninitRankingRank(void);
void UpdateRankingRank(void);
void DrawRankingRank(void);
void ResetRankingRank(void);

#endif
