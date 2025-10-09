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
XINPUT_KEYSTROKE g_joypadStroke[NUM_JOYSTROKE_MAX];
int g_nVibCounter = 0;
bool g_bJoyStick[JOYSTICK_MAX] = {};
bool g_bJoyStickRepeat[NUM_JOYSTROKE_MAX] = {};
bool g_JoypadControl = false;

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
	memset(&g_joypadStroke[0], 0, sizeof(XINPUT_KEYSTROKE));
	g_JoypadControl = false;
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
	XINPUT_KEYSTROKE joykeystroke;

	// �W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyStateTrigger.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
		g_joykeyStateRelease.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & g_joykeyStateRelease.Gamepad.wButtons;
		g_joykeyStateRepeat.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons);
		g_joykeyState = joykeyState;		// �W���C�p�b�h�̃v���X����ۑ�

		if (joykeyState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICK_UP] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICK_UP] = false;
		}

		if (joykeyState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICK_DOWN] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICK_DOWN] = false;
		}

		if (joykeyState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICK_LEFT] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICK_LEFT] = false;
		}

		if (joykeyState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_bJoyStick[JOYSTICK_RIGHT] = true;
		}
		else
		{
			g_bJoyStick[JOYSTICK_RIGHT] = false;
		}
	}

	if (XInputGetKeystroke(0, 0, &joykeystroke) == ERROR_SUCCESS)
	{
		for (int nCntKey = 0; nCntKey < NUM_JOYSTROKE_MAX; nCntKey++)
		{
			if (joykeystroke.VirtualKey - JOYKEYSTROKE_START == nCntKey)
			{
				g_joypadStroke[nCntKey] = joykeystroke;
			}
		}
	}

	if (g_joypadVibration.wLeftMotorSpeed != NULL || g_joypadVibration.wRightMotorSpeed != NULL)
	{
		g_nVibCounter--;
		if (g_nVibCounter <= 0)
		{
			g_joypadVibration.wLeftMotorSpeed = 0;
			g_joypadVibration.wRightMotorSpeed = 0;

			XInputSetState(0, &g_joypadVibration);
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
	int static nFrameCounter[NUM_KEY_MAX] = {};

	if (g_aKeyStateTrigger[nKey] & 0x80)
	{// �ŏ��̓g���K�[
		nFrameCounter[nKey] = 0;		// �t���[���J�E���^�[�����Z�b�g
		return true;					// true��Ԃ��ďI��
	}

	if (g_aKeyStateRepeat[nKey] & 0x80)
	{// ���s�[�g���Ă�Ȃ����
   		nFrameCounter[nKey]++;		// �t���[���J�E���^�[�𑝂₷
		if (nFrameCounter[nKey] >= 30)
		{// �J�E���g�����ȏ�Ȃ�
			if (nFrameCounter[nKey] % 5 == 0)
			{// ���Ԋu���Ƃ�true��Ԃ�
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	�L�[�{�[�h�̏����擾
//=============================================================================
bool GetKeyboardAny(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (g_aKeyState[nCntKey] & 0x80)
		{
			return true;
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
//	�W���C�p�b�h�̃v���X�����擾
//=============================================================================
bool GetJoypadStroke(WORD key)
{
	static int nCounter[NUM_JOYSTROKE_MAX] = {};		// �J�E���^�[����
	if (g_joypadStroke[key - JOYKEYSTROKE_START].VirtualKey & key)
	{// �擾�����L�[����v���Ă�����
		if (g_bJoyStickRepeat[key - JOYKEYSTROKE_START] == false && g_joypadStroke[key - JOYKEYSTROKE_START].Flags == XINPUT_KEYSTROKE_KEYDOWN)
		{// ���s�[�g���I�t���v���X�̎�
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = true;		// ���s�[�g�ҋ@
			return true;					// ��������Ԃ�
		}

		if (g_joypadStroke[key - JOYKEYSTROKE_START].Flags == 5)
		{// ���s�[�g�ɂȂ�����
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = false;		// �ҋ@��Ԃ���߂�
			
			nCounter[key - JOYKEYSTROKE_START]++;						// �J�E���g����
			if (nCounter[key - JOYKEYSTROKE_START] % 5 == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			nCounter[key - JOYKEYSTROKE_START] = 0;
		}

		if(g_joypadStroke[key - JOYKEYSTROKE_START].Flags == XINPUT_KEYSTROKE_KEYUP)
		{
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = false;		// �ҋ@��Ԃ���߂�
		}
	}

	return false;
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
			if (nFrameCounter[key] % 5 == 0)
			{// ���Ԋu���Ƃ�true��Ԃ�
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	�W���C�X�e�B�b�N�̃��s�[�g�����擾
//=============================================================================
bool GetJoypadStick(JOYSTICK stick)
{
	// �t���[���J�E���^�[
	static int nFrameCounter[JOYSTICK_MAX] = {};

	if (g_bJoyStick[stick] == true && nFrameCounter[stick] == 0)
	{
		nFrameCounter[stick] = 5;
		return true;
	}

	if (g_bJoyStick[stick] == true)
	{
		nFrameCounter[stick]++;
		if (nFrameCounter[stick] >= 30)
		{
			if (nFrameCounter[stick] % 5 == 0)
			{
				return true;
			}
		}
		
		return false;
	}

	if (g_bJoyStick[stick] == false)
	{
		nFrameCounter[stick] = 0;
		return false;
	}

	return false;

}

//=============================================================================
//	�W���C�p�b�h�̏����擾
//=============================================================================
bool GetJoypadAny(void)
{
	for (int nCntJoyKey = 0; nCntJoyKey < JOYKEY_MAX; nCntJoyKey++)
	{
		if (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << nCntJoyKey))
		{
			return true;
		}
	}

	for (int nCntJoyStick = 0; nCntJoyStick < JOYSTICK_MAX; nCntJoyStick++)
	{
		if (g_bJoyStick[nCntJoyStick] == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	�W���C�p�b�h�̐U���ݒ�
//=============================================================================
void SetJoypadVibration(int nLVibration, int nRVibration, int nVibCounter)
{
	g_joypadVibration.wLeftMotorSpeed = nLVibration;
	g_joypadVibration.wRightMotorSpeed = nRVibration;
	g_nVibCounter = nVibCounter;

	XInputSetState(0, &g_joypadVibration);
}

//=============================================================================
//	�W���C�p�b�h�̏��擾
//=============================================================================
XINPUT_STATE *GetJoypadState(void)
{
	return &g_joykeyState;
}

//=============================================================================
//	�W���C�p�b�h�̏��擾
//=============================================================================
XINPUT_KEYSTROKE* GetJoypadStroke(void)
{
	return &g_joypadStroke[0];
}

bool GetJoypadControl(void)
{
	return g_JoypadControl;
}