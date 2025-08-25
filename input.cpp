//=============================================================================
//
//	�L�[�{�[�h���͏��� [input.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "input.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			// �L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			// �L�[�{�[�h�̃����[�X���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];			// �L�[�{�[�h�̃��s�[�g���
BYTE g_aKeyCount[NUM_KEY_MAX];					// �L�[�{�[�h�̃J�E���g
XINPUT_STATE g_joykeyState;						// �W���C�p�b�h�̃v���X���
XINPUT_STATE g_joykeyStateTrigger;				// �W���C�p�b�h�̃g���K�[���
XINPUT_STATE g_joykeyStateRelease;				// �W���C�p�b�h�̃����[�X���
XINPUT_STATE g_joykeyStateRepeat;				// �W���C�p�b�h�̃��s�[�g���
XINPUT_VIBRATION g_joypadVibration;				// �W���C�p�b�h�̐U�����

//=============================================================================
//	�L�[�{�[�h����������
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �f�o�C�X�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
//	�W���C�p�b�h�̏���������
//=============================================================================
HRESULT InitJoypad(void)
{
	// �������̃N���A
	memset(&g_joykeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joypadVibration, 0, sizeof(XINPUT_VIBRATION));

	// XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

//=============================================================================
//	�L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
//	�W���C�p�b�h�̏I������
//=============================================================================
void UninitJoypad(void)
{
	// Xinput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//=============================================================================
//	�L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��
	int nCntKey;
	int nCntPressKey = 0;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		// �g���K�[
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	// �����[�X
			g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] & aKeyState[nCntKey]);							// ���s�[�g
			g_aKeyState[nCntKey] = aKeyState[nCntKey];															// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
//	�W���C�p�b�h�̍X�V����
//=============================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		// �W���C�p�b�h�̓��͏��
	static int nVibrationCounter = 0;

	// �W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyStateTrigger.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
		g_joykeyStateRelease.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & g_joykeyStateRelease.Gamepad.wButtons;
		g_joykeyStateRepeat.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons);

		g_joykeyState = joykeyState;		// �W���C�p�b�h�̃v���X����ۑ�
	}

	if (g_joypadVibration.wLeftMotorSpeed != NULL || g_joypadVibration.wRightMotorSpeed != NULL)
	{
		nVibrationCounter++;
		if (nVibrationCounter % 6 == 0)
		{
			SetJoypadVibration(0, 0);
			nVibrationCounter = 0;
		}
	}
}

//=============================================================================
//	�L�[�{�[�h�̃v���X�����擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	�L�[�{�[�h�̃g���K�[�����擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	�L�[�{�[�h�̃����[�X�����擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	�L�[�{�[�h�̃��s�[�g�����擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	// �t���[���J�E���^�[��錾
	int static nFrameCounter = 0;

	if (g_aKeyStateTrigger[nKey] & 0x80)
	{// �ŏ��̓g���K�[
		nFrameCounter = 0;		// �t���[���J�E���^�[�����Z�b�g
		return true;			// true��Ԃ��ďI��
	}

	if (g_aKeyStateRepeat[nKey] & 0x80)
	{// ���s�[�g���Ă�Ȃ����
   		nFrameCounter++;		// �t���[���J�E���^�[�𑝂₷
		if (nFrameCounter >= 30)
		{// �J�E���g�����ȏ�Ȃ�
			if (nFrameCounter % 10 == 0)
			{// ���Ԋu���Ƃ�true��Ԃ�
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	�W���C�p�b�h�̃v���X�����擾
//=============================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	�W���C�p�b�h�̃g���K�[�����擾
//=============================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	�W���C�p�b�h�̃����[�X�����擾
//=============================================================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	�W���C�p�b�h�̃��s�[�g�����擾
//=============================================================================
bool GetJoypadRepeat(JOYKEY key)
{
	// �t���[���J�E���^�[��錾
	int static nFrameCounter[JOYKEY_MAX] = {};

	if (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key))
	{// �ŏ��̓g���K�[
		nFrameCounter[key] = 0;		// �t���[���J�E���^�[�����Z�b�g
		return true;				// true��Ԃ��ďI��
	}

	if (g_joykeyStateRepeat.Gamepad.wButtons & (0x01 << key))
	{// ���s�[�g���Ă�Ȃ����
		nFrameCounter[key]++;		// �t���[���J�E���^�[�𑝂₷
		if (nFrameCounter[key] >= 30)
		{// �J�E���g�����ȏ�Ȃ�
			if (nFrameCounter[key] % 10 == 0)
			{// ���Ԋu���Ƃ�true��Ԃ�
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	�W���C�p�b�h�̐U���ݒ�
//=============================================================================
void SetJoypadVibration(int nLVibration, int nRVibration)
{
	g_joypadVibration.wLeftMotorSpeed = nLVibration;
	g_joypadVibration.wRightMotorSpeed = nRVibration;

	XInputSetState(0, &g_joypadVibration);
}

//=============================================================================
//	�W���C�p�b�h�̏��擾
//=============================================================================
XINPUT_STATE *GetJoypadState(void)
{
	return &g_joykeyState;
}