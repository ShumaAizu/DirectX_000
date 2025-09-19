//=============================================================================
//
//	�^�C�g�����j���[���� [titlemenu.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "titlemenu.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLEMENU_POSX		(440.0f)
#define TITLEMENU_POSY		(410.0f)
#define TITLEMENU_SIZEX		(400.0f)
#define TITLEMENU_SIZEY		(100.0f)
#define TITLEFADE_TIMER		(300)		// �^�C�}�[�̕b��

//*****************************************************************************
// �^�C�g�����j���[�\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	int nDispCounter;		// �J�E���^�[
	bool bDisp;				// �\�����
}TitleMenu;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTitleMenu[TITLEMENU_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMenu = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
TITLEMENU g_titleMenu = TITLEMENU_GAMESTART;						// �^�C�g�����j���[�̏��
TitleMenu g_atitleMenu[TITLEMENU_MAX] = {};							// �^�C�g�����j���[�̏��
int g_nMenuChangeCounter = 0;										// ���j���[�؂�ւ��J�E���^�[
bool g_bUpdate_TitleMenu = true;									// �^�C�g�����j���[����\��
int g_nTitleFadeCounter = 0;

//=============================================================================
//	�^�C�g�����j���[�̏���������
//=============================================================================
void InitTitleMenu(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[TITLEMENU_MAX] =
	{
		"data\\TEXTURE\\Title_GAMESTART.png",
		"data\\TEXTURE\\Title_EXIT.png",
	};


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMenu,
		NULL);

	for (int nCntTitleMenuTex = 0; nCntTitleMenuTex < TITLEMENU_MAX; nCntTitleMenuTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntTitleMenuTex], &g_apTextureTitleMenu[nCntTitleMenuTex]);
	}

	// ������
	g_titleMenu = TITLEMENU_GAMESTART;
	g_bUpdate_TitleMenu = true;
	g_nMenuChangeCounter = 60;
	g_nTitleFadeCounter = 0;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleMenu->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		g_atitleMenu[nCntTitleMenu].pos = D3DXVECTOR3(TITLEMENU_POSX, TITLEMENU_POSY, 0.0f);
		g_atitleMenu[nCntTitleMenu].nDispCounter = 4;
		g_atitleMenu[nCntTitleMenu].bDisp = true;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(TITLEMENU_POSX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TITLEMENU_POSX + TITLEMENU_SIZEX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TITLEMENU_POSX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY) + TITLEMENU_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TITLEMENU_POSX + TITLEMENU_SIZEX, TITLEMENU_POSY + (nCntTitleMenu * TITLEMENU_SIZEY) + TITLEMENU_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntTitleMenu == g_titleMenu)
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
	g_pVtxBuffTitleMenu->Unlock();
}

//=============================================================================
//	�^�C�g�����j���[�̏I������
//=============================================================================
void UninitTitleMenu(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (g_apTextureTitleMenu[nCntTitleMenu] != NULL)
		{
			g_apTextureTitleMenu[nCntTitleMenu]->Release();
			g_apTextureTitleMenu[nCntTitleMenu] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleMenu != NULL)
	{
		g_pVtxBuffTitleMenu->Release();
		g_pVtxBuffTitleMenu = NULL;
	}
}

//=============================================================================
//	�^�C�g�����j���[�̕`�揈��
//=============================================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleMenu, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (g_atitleMenu[nCntTitleMenu].bDisp == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTitleMenu[nCntTitleMenu]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitleMenu * 4, 2);
		}
	}
	
}

//=============================================================================
//	�^�C�g�����j���[�̍X�V����
//=============================================================================
void UpdateTitleMenu(void)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (nCntTitleMenu == g_titleMenu)
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

	if (g_bUpdate_TitleMenu == true)
	{
		if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStick(JOYSTICK_UP) == true)
		{ // ������L�[�������ꂽ��
			// ���݂̃��[�h�ɍ��킹�ĕύX
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_titleMenu)
			{
			case TITLEMENU_GAMESTART:
				g_titleMenu = TITLEMENU_EXIT;
				break;

			case 	TITLEMENU_EXIT:
				g_titleMenu = TITLEMENU_GAMESTART;
				break;
			}
		}

		if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStick(JOYSTICK_DOWN) == true)
		{ // �������L�[�������ꂽ��
			// ���݂̃��[�h�ɍ��킹�ĕύX
			PlaySound(SOUND_LABEL_SE_SELECT001);
			switch (g_titleMenu)
			{
			case TITLEMENU_GAMESTART:
				g_titleMenu = TITLEMENU_EXIT;
				break;

			case TITLEMENU_EXIT:
				g_titleMenu = TITLEMENU_GAMESTART;
				break;
			}
		}
	}

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // ����L�[�������ꂽ��
		if (g_bUpdate_TitleMenu == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bUpdate_TitleMenu = false;
	}

	if (g_bUpdate_TitleMenu == false)
	{
		g_nMenuChangeCounter--;
		g_atitleMenu[g_titleMenu].nDispCounter++;
		if (g_atitleMenu[g_titleMenu].nDispCounter % 5 == 0 && g_nMenuChangeCounter >= 0)
		{
			g_atitleMenu[g_titleMenu].bDisp = g_atitleMenu[g_titleMenu].bDisp ? false : true;
		}

		if (g_nMenuChangeCounter <= 0)
		{
			// ���݂̃��[�h�ɍ��킹�ĕύX
			switch (g_titleMenu)
			{
			case TITLEMENU_GAMESTART:
				SetFade(MODE_TUTORIAL, 0.025f, 0.025f);
				break;

			case TITLEMENU_EXIT:
				PostQuitMessage(0);
				break;
			}
		}
	}

	g_nTitleFadeCounter++;

	if (GetKeyboardAny() == true || GetJoypadAny() == true)
	{
		g_nTitleFadeCounter = 0;
	}

	if (g_nTitleFadeCounter >= TITLEFADE_TIMER && g_bUpdate_TitleMenu != false)
	{// �J�E���^�[���K��l�𒴂���
		// ���[�h�ݒ�
		SetFade(MODE_RANKING, 0.025f, 0.025f);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleMenu->Unlock();
}

//=============================================================================
//	�^�C�g�����j���[�̐ݒ菈��
//=============================================================================
void SetTitleMenu(TITLEMENU titlemenu)
{
	g_titleMenu = titlemenu;
}