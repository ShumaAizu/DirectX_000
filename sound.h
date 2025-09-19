//=============================================================================
//
// サウンド処理 [sound.h]
// Author : SHUMA AIZU
//          AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM_GAME000,	// ゲームBGM
	SOUND_LABEL_BGM_TITLE000,	// タイトルBGM
	SOUND_LABEL_BGM_RANKING000,	// ランキングBGM
	SOUND_LABEL_BGM_TUTORIAL000,// チュートリアルBGM
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_SELECT000,	// 選択1
	SOUND_LABEL_SE_SELECT001,	// 選択2
	SOUND_LABEL_SE_PAUSE000,	// ポーズ音
	SOUND_LABEL_SE_APPEAR000,	// 出現
	SOUND_LABEL_SE_SHOT001,		// 弾発射音
	SOUND_LABEL_SE_HIT001,		// ヒット音
	SOUND_LABEL_SE_SPEEDUP000,	// 加速音
	SOUND_LABEL_SE_EXPLOSION001,// 爆発音
	SOUND_LABEL_SE_TITLESET000,	// タイトルセット
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
