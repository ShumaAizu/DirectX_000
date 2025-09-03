//=============================================================================
//
//	�|�[�Y���� [pause.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_MENU_POSX		(515.0f)
#define PAUSE_MENU_POSY		(290.0f)
#define PAUSE_MENU_SIZEX	(250.0f)
#define PAUSE_MENU_SIZEY	(100.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
PAUSE_MENU g_pauseMenu = PAUSE_MENU_CONTINUE;					// �|�[�Y���j���[�̏��
bool g_bPauseDisp = true;

//=============================================================================
//	�|�[�Y���j���[�̏���������
//=============================================================================
void InitPause(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Pause_Menu_CONTINUE.png",
		&g_apTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Pause_Menu_RETRY.png",
		&g_apTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Pause_Menu_QUIT.png",
		&g_apTexturePause[2]);


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ������

	g_pauseMenu = PAUSE_MENU_CONTINUE;
	g_bPauseDisp = true;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(PAUSE_MENU_POSX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_MENU_POSX + PAUSE_MENU_SIZEX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_MENU_POSX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY) + PAUSE_MENU_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_MENU_POSX + PAUSE_MENU_SIZEX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY) + PAUSE_MENU_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntPause == g_pauseMenu)
		{ // �I������Ă���Εs�����x��߂�
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // �I������Ă��Ȃ���Εs�����x��������
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
//	�|�[�Y���j���[�̏I������
//=============================================================================
void UninitPause(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
//	�|�[�Y���j���[�̕`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bPauseDisp == true)
	{
		for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
		}
	}
}

//=============================================================================
//	�|�[�Y���j���[�̍X�V����
//=============================================================================
void UpdatePause(void)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (nCntPause == g_pauseMenu)
		{ // �I������Ă���Εs�����x��߂�
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // �I������Ă��Ȃ���Εs�����x��������
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		pVtx += 4;
	}

	if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_UP) == true)
	{ // ������L�[�������ꂽ��
		// ���݂̃��[�h�ɍ��킹�ĕύX
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;

		case PAUSE_MENU_RETRY:
			g_pauseMenu = PAUSE_MENU_CONTINUE;
			break;

		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_RETRY;
			break;
		}
	}

	if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_DOWN) == true)
	{ // �������L�[�������ꂽ��
		// ���݂̃��[�h�ɍ��킹�ĕύX
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			g_pauseMenu = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_RETRY:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;

		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_CONTINUE;
			break;
		}
	}

	if (GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{ // ����L�[�������ꂽ��
		// �|�[�Y������
		SetEnablePause(false);

		// ���݂̃��[�h�ɍ��킹�ĕύX
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			
			break;

		case PAUSE_MENU_RETRY:
			SetFade(MODE_GAME);
			break;

		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		g_bPauseDisp = g_bPauseDisp ? false : true;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
//	�|�[�Y���j���[�̐ݒ菈��
//=============================================================================
void SetPauseMenu(PAUSE_MENU pause_menu)
{
	g_pauseMenu = pause_menu;
}