//=============================================================================
//
//	�Q�[����ʏ��� [game.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// �Q�[���̏��
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,				// �������Ă��Ȃ����
	GAMESTATE_NORMAL,				// �ʏ���
	GAMESTATE_END,					// �I�����
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// �Q�[���I������
//*****************************************************************************
typedef enum
{
	GAMEEND_CLEAR = 0,		// ����
	GAMEEND_GAMEOVER,		// �s�k
	GAMEEND_MAX
}GAMEEND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
GAMEEND GetGameEnd(void);
void SetEnablePause(bool bPause);

#endif