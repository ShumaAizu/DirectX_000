//=============================================================================
//
//	�L�[�{�[�h���͏��� [input.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_KEY_MAX			(256)		//�L�[�̍ő吔
#define NUM_JOYSTROKE_MAX	(38)
#define JOYKEYSTROKE_START	(0x5800)

//*****************************************************************************
// �W���C�p�b�h�̃L�[�̎��
//*****************************************************************************
typedef enum
{
	JOYKEY_UP = 0,				// �\���L�[��
	JOYKEY_DOWN,				// �\���L�[��
	JOYKEY_LEFT,				// �\���L�[��
	JOYKEY_RIGHT,				// �\���L�[�E
	JOYKEY_START,				// �X�^�[�g�{�^��
	JOYKEY_BACK,				// �o�b�N�{�^��
	JOYKEY_LEFT_THUMB,			// ���X�e�B�b�N��������
	JOYKEY_RIGHT_THUMB,			// �E�X�e�B�b�N��������
	JOYKEY_LEFT_SHOULDER,		// L�{�^��
	JOYKEY_RIGHT_SHOULDER,		// R�{�^��
	JOYKEY_NULL1,				// ����`
	JOYKEY_NULL2,				// ����`
	JOYKEY_A,					// A�{�^��
	JOYKEY_B,					// B�{�^��
	JOYKEY_X,					// X�{�^��
	JOYKEY_Y,					// Y�{�^��
	JOYKEY_MAX					// ����
}JOYKEY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey); 
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
bool GetJoypadRepeat(JOYKEY key);
void SetJoypadVibration(int nLVibration, int nRVibration, int nVibCounter);
XINPUT_STATE *GetJoypadState(void);
bool GetJoypadStroke(WORD key);
XINPUT_KEYSTROKE* GetJoypadStroke(void);

#endif
