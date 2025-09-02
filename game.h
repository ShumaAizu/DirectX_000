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
	GAMESTATE_NORMAL,				// 通常状態
	GAMESTATE_END,					// 終了状態
	GAMESTATE_MAX
}GAMESTATE;

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
GAMESTATE GetGameState(void);
GAMEEND GetGameEnd(void);
void SetEnablePause(bool bPause);

#endif