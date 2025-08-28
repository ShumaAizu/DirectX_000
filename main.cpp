//=============================================================================
//
//	�V���[�e�B���O�Q�[������ [main.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"
#include "option.h"
#include <crtdbg.h>


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			"WindowClass"					// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME			"�V���[�e�B���O�Q�[������"		// �E�C���h�E�̖��O

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void DrawControl(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3D9 g_pD3D = NULL;					// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		// Direct3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;					// ���[�h���
LPD3DXFONT g_pFont = NULL;					// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;						// FPS�J�E���^

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O
#endif // _DEBUG

	WNDCLASSEX wcex =									// �E�C���h�E�N���X�̍\����
	{
		sizeof(WNDCLASSEX),								// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,										// �E�C���h�E�̃X�^�C��
		WindowProc,										// �E�C���h�E�v���V�[�W��
		0,												// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,												// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,										// �C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),				// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),					// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),						// �N���C�A���g�̈�̔w�i�F
		NULL,											// ���j���[�o�[
		CLASS_NAME,										// �E�C���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)					// �t�@�C���̃A�C�R��
	};

	HWND hWnd;				// �E�C���h�E�n���h��(���ʎq)
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// ��ʃT�C�Y�̍\����

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,							// �g���E�C���h�E�X�^�C��
						  CLASS_NAME,					// �E�C���h�E�N���X�̖��O
						  WINDOW_NAME,					// �E�C���h�E�̖��O
						  WS_OVERLAPPEDWINDOW,			// �E�C���h�E�X�^�C��
						  CW_USEDEFAULT,				// �E�C���h�E�̍���X���W
						  CW_USEDEFAULT,				// �E�C���h�E�̍���Y���W
						  (rect.right - rect.left),		// �E�C���h�E�̕�
						  (rect.bottom - rect.top),		// �E�C���h�E�̍���
						  NULL,							// �e�E�C���h�E�̃n���h��
						  NULL,							// ���j���[�n���h���܂��͎q�E�C���h�EID
						  hInstance,					// �C���X�^���X�n���h��
						  NULL);						// �E�C���h�E�쐬�f�[�^

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// ���������������s�����ꍇ
		return - 1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;						// ����������
	dwExecLastTime = timeGetTime();			// ���ݎ������擾(�ۑ�)

	dwFrameCount = 0;						// ����������
	dwFPSLastTime = timeGetTime();			// ���ݎ������擾(�ۑ�)

	// �E�C���h�E��\��
	ShowWindow(hWnd, nCmdShow);				// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);						// �N���C�A���g�̈���X�V

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�����烋�[�v�𔲂���
				break;
			}
			else
			{
			// ���b�Z�[�W�̐ݒ�
			TranslateMessage(&msg);				// ���z���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
			DispatchMessage(&msg);				// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();		// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPS���v���������Ԃ�ۑ�
				dwFrameCount = 0;				// �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60����1�b�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���[���ݎ���]��ۑ�

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;					// �t���[���J�E���g�����Z
			}
		}
	}

	// �I������
	Uninit();

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===============================================================================
// �E�C���h�E�v���V�[�W��
//===============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				// �E�C���h�E��j������
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:								// ����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			// �E�C���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;							// 0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===============================================================================
// ����������
//===============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// DirectX�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// �Q�[����ʃT�C�Y(��)

	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// �Q�[����ʃT�C�Y(����)

	d3dpp.BackBufferFormat = d3ddm.Format;							// �o�b�N�o�b�t�@�̌`��

	d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)

	d3dpp.EnableAutoDepthStencil = TRUE;							// �f�u�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// �f�u�X�o�b�t�@�Ƃ���16bit���g��

	d3dpp.Windowed = bWindow;										// �E�C���h�E���[�h

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	// �`�揈���ƒ��_�������n�[�h�E�F�A�ōs��
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// ���_�����̂�CPU�ōs��
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// ����CPU�ōs��
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̊g�k��Ԃ̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
				   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			   	   "Terminal", &g_pFont);

	// �����V�[�h�Z�b�g
	srand((unsigned)timeGetTime());
	
	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// �W���C�p�b�h�̏���������
	if(FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// �T�E���h�̏���������
	InitSound(hWnd);

	// ���[�h�̐ݒ�
	InitFade(g_mode);


	return S_OK;
}
//===============================================================================
// �I������
//===============================================================================
void Uninit(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �W���C�p�b�h�̏I������
	UninitJoypad();

	// �^�C�g����ʂ̏I������
	UninitTitle();

	// �Q�[����ʂ̏I������
	UninitGame();

	// ���U���g��ʂ̏I������
	UninitResult();

	// �����L���O��ʂ̏I������
	UninitRanking();

	// �t�F�[�h�̏I������
	UninitFade();

	// �T�E���h���~�߂�
	StopSound();

	// �T�E���h�̏I������
	UninitSound();

	// �f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//===============================================================================
// �X�V����
//===============================================================================
void Update(void)
{
	// �e��I�u�W�F�N�g�X�V����
	UpdateKeyboard();

	// �W���C�p�b�h�̍X�V����
	UpdateJoypad();

	switch (g_mode)
	{
		// �^�C�g�����[�h
	case MODE_TITLE:
		UpdateTitle();
		break;

		// �Q�[�����[�h
	case MODE_GAME:
		UpdateGame();
		break;

		// ���U���g���[�h
	case MODE_RESULT:
		UpdateResult();
		break;

		// �����L���O���[�h
	case MODE_RANKING:
		UpdateRanking();
		break;
	}

	// �t�F�[�h�̍X�V����
	UpdateFade();
}

//===============================================================================
// �`�揈��
//===============================================================================
void Draw(void)
{
	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ
		// �e��I�u�W�F�N�g�̕`�揈��

		switch (g_mode)
		{
			// �^�C�g�����[�h
		case MODE_TITLE:
			DrawTitle();
			break;

			// �Q�[�����[�h
		case MODE_GAME:
			DrawGame();
			break;

			// ���U���g���[�h
		case MODE_RESULT:
			DrawResult();
			break;

			// �����L���O���[�h
		case MODE_RANKING:
			DrawRanking();
			break;
		}

		// �t�F�[�h�̕`�揈��
		DrawFade();

		// �f�o�b�O�\��
#ifdef _DEBUG
		DrawDebug();
#endif

		// ������@�\��
		DrawControl();

		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==================================================================================
//	�f�o�C�X�̎擾
//==================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==================================================================================
//	���[�h�̐ݒ�
//==================================================================================
void SetMode(MODE mode)
{
	// ���݂̃��[�h�̏I������
	switch (g_mode)
	{
		// �^�C�g�����[�h
	case MODE_TITLE:
		UninitTitle();
		break;

		// �Q�[�����[�h
	case MODE_GAME:
		UninitGame();
		break;

		// ���U���g���[�h
	case MODE_RESULT:
		UninitResult();
		break;
	}

	// �V�������[�h�̏���������
	switch (mode)
	{
		// �^�C�g�����[�h
	case MODE_TITLE:
		InitTitle();
		break;

		// �Q�[�����[�h
	case MODE_GAME:
		InitGame();
		break;

		// ���U���g���[�h
	case MODE_RESULT:
		InitResult();
		break;

		// �����L���O���[�h
	case MODE_RANKING:
		InitRanking();
		break;
	}

	// ���[�h�̍X�V
	g_mode = mode;
}

//==================================================================================
//	���[�h�̎擾
//==================================================================================
MODE GetMode(void)
{
	return g_mode;		// ���݂̃��[�h��Ԃ�
}

//==================================================================================
//	�f�o�b�O�\��
//==================================================================================
void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rect1 = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStr[256] = {};		// ��ʂɕ\�����镶����
	char aStr1[256] = {};		// ��ʂɕ\�����镶����

	XINPUT_STATE* pjoystate = GetJoypadState();
	Player* pPlayer = GetPlayer();
	Option* pOption = GetOption();

	// ���������
	wsprintf(&aStr[0], "FPS:%d\nLX:%d\nLY:%d", g_nCountFPS, pjoystate->Gamepad.sThumbLX, pjoystate->Gamepad.sThumbLY);		// FPS�\��
	sprintf(&aStr1[0], "OpitonPos = { %.2f,%.2f,%.2f }", pOption->pos.x, pOption->pos.y, pOption->rot.z);

	// �e�L�X�g��`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==================================================================================
//	������@�\��
//==================================================================================
void DrawControl(void)
{
	RECT rect = { 0, 600, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256] = {};		// ��ʂɕ\�����镶����

	wsprintf(&aStr[0], "L�X�e�B�b�N WASD : �ړ�\nA SPACEKEY : �V���b�g\nX F1 : �I�v�V��������\nLB/RB : �I�v�V������]");

	// �e�L�X�g��`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}