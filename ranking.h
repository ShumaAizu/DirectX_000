//=============================================================================
//
//	�����L���O��ʏ��� [ranking.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANK				(5)			// �����N��
#define MAX_NUM					(5)			// ����
#define RANKING_DIFF			(15.0f)
#define RANKING_SIZEX			(50.0f)
#define RANKING_SIZEY			(100.0f)
#define RANKING_POSX			(540.0f)
#define RANKING_POSY			(25.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);
int Sort_desc(const void* pCastScore1, const void* pCastScore2);

#endif
