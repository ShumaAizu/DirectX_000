//=============================================================================
//
//	�����L���O��ʏ��� [ranking.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RANKINGRANK_H_
#define _RANKINGRANK_H_

#include "main.h"

// �^�C�g���\���̂̒�`

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRankingRank(void);
void UninitRankingRank(void);
void UpdateRankingRank(void);
void DrawRankingRank(void);
void ResetRankingRank(void);
void SetRankingRank(int nScore);
int Sort_desc(const void* pCastScore1, const void* pCastScore2);

#endif
