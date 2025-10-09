//=============================================================================
//
//	�X�R�A���� [score.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INIT_SCORE		(5000)				// �X�R�A�̏����l

//*****************************************************************************
// �X�R�AUI�̎��
//*****************************************************************************
typedef enum
{
	SCOREUI_GAGE = 0,		// �X�R�A����
	SCOREUI_CURTAIN,		// ���݃X�R�A
	SCOREUI_SCALE,			// ���[�^�[
	SCOREUI_MAX
}SCOREUI;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
void SubScore(int nValue);
int GetScore(void);
void CollisionScoretoEnemy(void);

#endif
