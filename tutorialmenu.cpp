//=============================================================================
//
//	�^�C�g�����j���[���� [tutorialmenu.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "tutorial.h"
#include "tutorialmenu.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIALMENU_POSX		(780.0f)
#define TUTORIALMENU_POSY		(500.0f)
#define TUTORIALMENU_SIZEX		(500.0f)
#define TUTORIALMENU_SIZEY		(200.0f)
#define TUTORIALFADE_TIMER		(300)		// �^�C�}�[�̕b��

//*****************************************************************************
// �^�C�g�����j���[�\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	int nDispCounter;		// �J�E���^�[
	bool bDisp;				// �\�����
}TutorialMenu;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTutorialMenu = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMenu = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
TutorialMenu g_tutorialMenu = {};									// �^�C�g�����j���[�̏��
int g_nTutorialChangeCounter = 0;									// ���j���[�؂�ւ��J�E���^�[
bool g_bUpdate_TutorialMenu = true;									// �^�C�g�����j���[����\��
bool g_bTutorialFade = false;

//=============================================================================
//	�^�C�g�����j���[�̏���������
//=============================================================================
void InitTutorialMenu(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialMenu,
		NULL);


		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\PRESS.png", &g_apTextureTutorialMenu);

	// ������
	g_tutorialMenu.nDispCounter = 0;
	g_bUpdate_TutorialMenu = true;
	g_nTutorialChangeCounter = 60;
	g_bTutorialFade = false;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialMenu->Lock(0, 0, (void * *)&pVtx, 0);

	g_tutorialMenu.pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY, 0.0f);
	g_tutorialMenu.nDispCounter = 4;
	g_tutorialMenu.bDisp = true;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIALMENU_POSX + TUTORIALMENU_SIZEX, TUTORIALMENU_POSY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY + TUTORIALMENU_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIALMENU_POSX + TUTORIALMENU_SIZEX, TUTORIALMENU_POSY + TUTORIALMENU_SIZEY, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialMenu->Unlock();
}

//=============================================================================
//	�^�C�g�����j���[�̏I������
//=============================================================================
void UninitTutorialMenu(void)
{
// �e�N�X�`���̔j��
	if (g_apTextureTutorialMenu != NULL)
	{
		g_apTextureTutorialMenu->Release();
		g_apTextureTutorialMenu = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorialMenu != NULL)
	{
		g_pVtxBuffTutorialMenu->Release();
		g_pVtxBuffTutorialMenu = NULL;
	}
}

//=============================================================================
//	�^�C�g�����j���[�̕`�揈��
//=============================================================================
void DrawTutorialMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialMenu, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	if (g_tutorialMenu.bDisp == true)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTutorialMenu);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	
}

//=============================================================================
//	�^�C�g�����j���[�̍X�V����
//=============================================================================
void UpdateTutorialMenu(void)
{
	g_tutorialMenu.nDispCounter++;

	if (g_tutorialMenu.nDispCounter % 60 == 0 && g_bTutorialFade != true)
	{
		g_tutorialMenu.bDisp = g_tutorialMenu.bDisp ? false : true;
		if (g_tutorialMenu.bDisp == false)
		{
			g_tutorialMenu.nDispCounter = 54;
		}
		else
		{
			g_tutorialMenu.nDispCounter = 0;
		}
	}

	if (GetJoypadTrigger(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{// ����L�[�������ꂽ
		// ���[�h�ݒ�
		if (g_bTutorialFade != true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bTutorialFade = true;
	}

	if (g_bTutorialFade == true)
	{
		g_nTutorialChangeCounter--;
		g_tutorialMenu.nDispCounter++;
		if (g_tutorialMenu.nDispCounter % 5 == 0 && g_nTutorialChangeCounter >= 0)
		{
			g_tutorialMenu.bDisp = g_tutorialMenu.bDisp ? false : true;
		}

		if (g_nTutorialChangeCounter <= 0)
		{
			SetFade(MODE_GAME, 0.025f, 0.025f);
		}
	}
}