//=============================================================================
//
//	�^�C�g�����j���[���� [selectlevel.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "titlemenu.h"
#include "selectlevel.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SELECTLEVEL_POSX		(440.0f)
#define SELECTLEVEL_POSY		(410.0f)
#define SELECTLEVEL_SIZEX		(400.0f)
#define SELECTLEVEL_SIZEY		(100.0f)
#define TITLEFADE_TIMER		(300)		// �^�C�}�[�̕b��

//*****************************************************************************
// �^�C�g�����j���[�\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	int nDispCounter;		// �J�E���^�[
	bool bDisp;				// �\�����
}SelectLevel;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureSelectLevel[SELECTLEVEL_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectLevel = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
SELECTLEVEL g_SelectLevel = SELECTLEVEL_NORMAL;						// �^�C�g�����j���[�̏��
SelectLevel g_aSelectLevel[SELECTLEVEL_MAX] = {};							// �^�C�g�����j���[�̏��
int g_nLevelMenuChangeCounter = 0;										// ���j���[�؂�ւ��J�E���^�[
bool g_bUpdate_SelectLevel = true;									// �^�C�g�����j���[����\��

//=============================================================================
//	�^�C�g�����j���[�̏���������
//=============================================================================
void InitSelectLevel(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[SELECTLEVEL_MAX] =
	{
		"data\\TEXTURE\\Select_NORMAL.png",
		"data\\TEXTURE\\Select_HARD.png",
	};


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SELECTLEVEL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectLevel,
		NULL);

	for (int nCntSelectLevelTex = 0; nCntSelectLevelTex < SELECTLEVEL_MAX; nCntSelectLevelTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntSelectLevelTex], &g_apTextureSelectLevel[nCntSelectLevelTex]);
	}

	// ������
	g_SelectLevel = SELECTLEVEL_NORMAL;
	g_bUpdate_SelectLevel = true;
	g_nLevelMenuChangeCounter = 60;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectLevel->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		g_aSelectLevel[nCntSelectLevel].pos = D3DXVECTOR3(SELECTLEVEL_POSX, SELECTLEVEL_POSY, 0.0f);
		g_aSelectLevel[nCntSelectLevel].nDispCounter = 4;
		g_aSelectLevel[nCntSelectLevel].bDisp = true;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SELECTLEVEL_POSX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SELECTLEVEL_POSX + SELECTLEVEL_SIZEX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SELECTLEVEL_POSX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY) + SELECTLEVEL_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SELECTLEVEL_POSX + SELECTLEVEL_SIZEX, SELECTLEVEL_POSY + (nCntSelectLevel * SELECTLEVEL_SIZEY) + SELECTLEVEL_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntSelectLevel == g_SelectLevel)
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
	g_pVtxBuffSelectLevel->Unlock();
}

//=============================================================================
//	�^�C�g�����j���[�̏I������
//=============================================================================
void UninitSelectLevel(void)
{
	// �e�N�X�`���̔j��
	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		if (g_apTextureSelectLevel[nCntSelectLevel] != NULL)
		{
			g_apTextureSelectLevel[nCntSelectLevel]->Release();
			g_apTextureSelectLevel[nCntSelectLevel] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffSelectLevel != NULL)
	{
		g_pVtxBuffSelectLevel->Release();
		g_pVtxBuffSelectLevel = NULL;
	}
}

//=============================================================================
//	�^�C�g�����j���[�̕`�揈��
//=============================================================================
void DrawSelectLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSelectLevel, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		if (g_aSelectLevel[nCntSelectLevel].bDisp == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureSelectLevel[nCntSelectLevel]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelectLevel * 4, 2);
		}
	}
	
}

//=============================================================================
//	�^�C�g�����j���[�̍X�V����
//=============================================================================
void UpdateSelectLevel(void)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectLevel->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSelectLevel = 0; nCntSelectLevel < SELECTLEVEL_MAX; nCntSelectLevel++)
	{
		if (nCntSelectLevel == g_SelectLevel)
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

	if (g_bUpdate_SelectLevel == true)
	{
		if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStick(JOYSTICK_UP) == true)
		{ // ������L�[�������ꂽ��
			// ���݂̃��[�h�ɍ��킹�ĕύX
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_SelectLevel)
			{
			case SELECTLEVEL_NORMAL:
				g_SelectLevel = SELECTLEVEL_HARD;
				break;

			case 	SELECTLEVEL_HARD:
				g_SelectLevel = SELECTLEVEL_NORMAL;
				break;
			}
		}

		if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStick(JOYSTICK_DOWN) == true)
		{ // �������L�[�������ꂽ��
			// ���݂̃��[�h�ɍ��킹�ĕύX
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_SelectLevel)
			{
			case SELECTLEVEL_NORMAL:
				g_SelectLevel = SELECTLEVEL_HARD;
				break;

			case SELECTLEVEL_HARD:
				g_SelectLevel = SELECTLEVEL_NORMAL;
				break;
			}
		}
	}

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // ����L�[�������ꂽ��
		if (g_bUpdate_SelectLevel == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bUpdate_SelectLevel = false;
	}

	if (GetKeyboardTrigger(DIK_Q) == true || GetJoypadTrigger(JOYKEY_B) == true)
	{
		SetTitleState(TITLESTATE_MENU);
		g_SelectLevel = SELECTLEVEL_NORMAL;
		UninitTitleMenu();
		InitTitleMenu();
	}

	if (g_bUpdate_SelectLevel == false)
	{
		g_nLevelMenuChangeCounter--;
		g_aSelectLevel[g_SelectLevel].nDispCounter++;
		if (g_aSelectLevel[g_SelectLevel].nDispCounter % 5 == 0 && g_nLevelMenuChangeCounter >= 0)
		{
			g_aSelectLevel[g_SelectLevel].bDisp = g_aSelectLevel[g_SelectLevel].bDisp ? false : true;
		}

		if (g_nLevelMenuChangeCounter <= 0)
		{
			// ���݂̃��[�h�ɍ��킹�ĕύX
			switch (g_SelectLevel)
			{
			case SELECTLEVEL_NORMAL:
				SetGameMode(GAMEMODE_NORMAL);
				break;

			case SELECTLEVEL_HARD:
				SetGameMode(GAMEMODE_HARD);
				break;
			}

			SetFade(MODE_GAME, 0.025f, 0.025f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelectLevel->Unlock();
}

//=============================================================================
//	�^�C�g�����j���[�̐ݒ菈��
//=============================================================================
void SetSelectLevel(SELECTLEVEL selectlevel)
{
	g_SelectLevel = selectlevel;
}