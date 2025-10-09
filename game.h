//=============================================================================
//
//	ゲーム画面処理 [game.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// ゲームの状態
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,				// 何もしていない状態
	GAMESTATE_TUTORIAL,				// チュートリアル状態
	GAMESTATE_NORMAL,				// 通常状態
	GAMESTATE_END,					// 終了状態
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// ゲームの難易度
//*****************************************************************************
typedef enum
{
	GAMEMODE_NORMAL = 0,
	GAMEMODE_HARD,
	GAMEMODE_MAX
}GAMEMODE;

//*****************************************************************************
// チュートリアルイベントの状態
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
// ゲーム終了条件
//*****************************************************************************
typedef enum
{
	GAMEEND_CLEAR = 0,		// 勝利
	GAMEEND_GAMEOVER,		// 敗北
	GAMEEND_MAX
}GAMEEND;

//*****************************************************************************
// プロトタイプ宣言
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