//=============================================================================
//
//	キーボード入力処理 [input.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "input.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			// キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			// キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];			// キーボードのリピート情報
BYTE g_aKeyCount[NUM_KEY_MAX];					// キーボードのカウント
XINPUT_STATE g_joykeyState;						// ジョイパッドのプレス情報
XINPUT_STATE g_joykeyStateTrigger;				// ジョイパッドのトリガー情報
XINPUT_STATE g_joykeyStateRelease;				// ジョイパッドのリリース情報
XINPUT_STATE g_joykeyStateRepeat;				// ジョイパッドのリピート情報
XINPUT_VIBRATION g_joypadVibration;				// ジョイパッドの振動情報
XINPUT_KEYSTROKE g_joypadStroke[NUM_JOYSTROKE_MAX];
int g_nVibCounter = 0;
bool g_bJoyStickRepeat[NUM_JOYSTROKE_MAX] = {};

//=============================================================================
//	キーボード初期化処理
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// デバイスへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
//	ジョイパッドの初期化処理
//=============================================================================
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&g_joykeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joypadVibration, 0, sizeof(XINPUT_VIBRATION));
	memset(&g_joypadStroke[0], 0, sizeof(XINPUT_KEYSTROKE));

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//=============================================================================
//	キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	// 入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
//	ジョイパッドの終了処理
//=============================================================================
void UninitJoypad(void)
{
	// Xinputのステートを設定(無効にする)
	XInputEnable(false);
}

//=============================================================================
//	キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報
	int nCntKey;
	int nCntPressKey = 0;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		// トリガー
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	// リリース
			g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] & aKeyState[nCntKey]);							// リピート
			g_aKeyState[nCntKey] = aKeyState[nCntKey];															// キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
//	ジョイパッドの更新処理
//=============================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		// ジョイパッドの入力情報
	XINPUT_KEYSTROKE joykeystroke;

	// ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyStateTrigger.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
		g_joykeyStateRelease.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & g_joykeyStateRelease.Gamepad.wButtons;
		g_joykeyStateRepeat.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons);

		g_joykeyState = joykeyState;		// ジョイパッドのプレス情報を保存
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
//	キーボードのプレス情報を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのトリガー情報を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのリリース情報を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//	キーボードのリピート情報を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	// フレームカウンターを宣言
	int static nFrameCounter[NUM_KEY_MAX] = {};

	if (g_aKeyStateTrigger[nKey] & 0x80)
	{// 最初はトリガー
		nFrameCounter[nKey] = 0;		// フレームカウンターをリセット
		return true;					// trueを返して終了
	}

	if (g_aKeyStateRepeat[nKey] & 0x80)
	{// リピートしてるなら入る
   		nFrameCounter[nKey]++;		// フレームカウンターを増やす
		if (nFrameCounter[nKey] >= 30)
		{// カウントが一定以上なら
			if (nFrameCounter[nKey] % 10 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドのプレス情報を取得
//=============================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのプレス情報を取得
//=============================================================================
bool GetJoypadStroke(WORD key)
{
	static int nCounter[NUM_JOYSTROKE_MAX] = {};		// カウンターを回す
	if (g_joypadStroke[key - JOYKEYSTROKE_START].VirtualKey & key)
	{// 取得したキーが一致していたら
		if (g_bJoyStickRepeat[key - JOYKEYSTROKE_START] == false && g_joypadStroke[key - JOYKEYSTROKE_START].Flags == XINPUT_KEYSTROKE_KEYDOWN)
		{// リピートがオフかつプレスの時
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = true;		// リピート待機
			return true;					// いったん返す
		}

		if (g_joypadStroke[key - JOYKEYSTROKE_START].Flags == 5)
		{// リピートになったら
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = false;		// 待機状態から戻す
			
			nCounter[key - JOYKEYSTROKE_START]++;						// カウントを回す
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
			g_bJoyStickRepeat[key - JOYKEYSTROKE_START] = false;		// 待機状態から戻す
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドのトリガー情報を取得
//=============================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリリース情報を取得
//=============================================================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=============================================================================
//	ジョイパッドのリピート情報を取得
//=============================================================================
bool GetJoypadRepeat(JOYKEY key)
{
	// フレームカウンターを宣言
	int static nFrameCounter[JOYKEY_MAX] = {};

	if (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key))
	{// 最初はトリガー
		nFrameCounter[key] = 0;		// フレームカウンターをリセット
		return true;				// trueを返して終了
	}

	if (g_joykeyStateRepeat.Gamepad.wButtons & (0x01 << key))
	{// リピートしてるなら入る
		nFrameCounter[key]++;		// フレームカウンターを増やす
		if (nFrameCounter[key] >= 30)
		{// カウントが一定以上なら
			if (nFrameCounter[key] % 10 == 0)
			{// 一定間隔ごとにtrueを返す
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
//	ジョイパッドの振動設定
//=============================================================================
void SetJoypadVibration(int nLVibration, int nRVibration, int nVibCounter)
{
	g_joypadVibration.wLeftMotorSpeed = nLVibration;
	g_joypadVibration.wRightMotorSpeed = nRVibration;
	g_nVibCounter = nVibCounter;

	XInputSetState(0, &g_joypadVibration);
}

//=============================================================================
//	ジョイパッドの情報取得
//=============================================================================
XINPUT_STATE *GetJoypadState(void)
{
	return &g_joykeyState;
}

//=============================================================================
//	ジョイパッドの情報取得
//=============================================================================
XINPUT_KEYSTROKE* GetJoypadStroke(void)
{
	return &g_joypadStroke[0];
}