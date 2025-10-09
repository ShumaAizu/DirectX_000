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
	GAMESTATE_TUTORIAL,				// �`���[�g���A�����
	GAMESTATE_NORMAL,				// �ʏ���
	GAMESTATE_END,					// �I�����
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// �Q�[���̓�Փx
//*****************************************************************************
typedef enum
{
	GAMEMODE_NORMAL = 0,
	GAMEMODE_HARD,
	GAMEMODE_MAX
}GAMEMODE;

//*****************************************************************************
// �`���[�g���A���C�x���g�̏��
//*****************************************************************************
typedef enum
{
	TUTORIALEVENT_MOVE = 0,
	TUTORIALEVENT_ENEMY,
	TUTORIALEVENT_POWERUP,
	TUTORIALEVENT_END,
	TUTORIALEVENT_MAX
}TUTORIALEVENT;

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
void SetGameMode(GAMEMODE gamemode);
void SetTutorialEvent(TUTORIALEVENT tutorialevent);
GAMESTATE GetGameState(void);
GAMEMODE GetGameMode(void);
TUTORIALEVENT GetTutorialEvent(void);
GAMEEND GetGameEnd(void);
void SetEnablePause(bool bPause);

#endif