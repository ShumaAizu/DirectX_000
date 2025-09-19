//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : SHUMA AIZU
//          AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM_GAME000,	// �Q�[��BGM
	SOUND_LABEL_BGM_TITLE000,	// �^�C�g��BGM
	SOUND_LABEL_BGM_RANKING000,	// �����L���OBGM
	SOUND_LABEL_BGM_TUTORIAL000,// �`���[�g���A��BGM
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_SELECT000,	// �I��1
	SOUND_LABEL_SE_SELECT001,	// �I��2
	SOUND_LABEL_SE_PAUSE000,	// �|�[�Y��
	SOUND_LABEL_SE_APPEAR000,	// �o��
	SOUND_LABEL_SE_SHOT001,		// �e���ˉ�
	SOUND_LABEL_SE_HIT001,		// �q�b�g��
	SOUND_LABEL_SE_SPEEDUP000,	// ������
	SOUND_LABEL_SE_EXPLOSION001,// ������
	SOUND_LABEL_SE_TITLESET000,	// �^�C�g���Z�b�g
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
