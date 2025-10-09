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
#define TUTORIALMENU_POSX		(640.0f)
#define TUTORIALMENU_POSY		(75.0f)
#define TUTORIALMENU_SIZEX		(300.0f)
#define TUTORIALMENU_SIZEY		(75.0f)

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
LPDIRECT3DTEXTURE9 g_apTextureTutorialMenu[TUTORIALEVENT_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMenu = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
TutorialMenu g_tutorialMenu = {};										// �^�C�g�����j���[�̏��

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

	const char* pTutorial[TUTORIALEVENT_MAX] =
	{
		"data\\TEXTURE\\Tutorial001.png",
		"data\\TEXTURE\\Tutorial002.png",
		"data\\TEXTURE\\Tutorial003.png",
		"data\\TEXTURE\\Tutorial004.png"
	};

	for (int nCntTutorialEvent = 0; nCntTutorialEvent < TUTORIALEVENT_MAX; nCntTutorialEvent++)
	{
		D3DXCreateTextureFromFile(pDevice, pTutorial[nCntTutorialEvent], &g_apTextureTutorialMenu[nCntTutorialEvent]);
	}

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialMenu->Lock(0, 0, (void * *)&pVtx, 0);

	g_tutorialMenu.pos = D3DXVECTOR3(TUTORIALMENU_POSX, TUTORIALMENU_POSY, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TUTORIALMENU_POSX - TUTORIALMENU_SIZEX, TUTORIALMENU_POSY - TUTORIALMENU_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIALMENU_POSX + TUTORIALMENU_SIZEX, TUTORIALMENU_POSY - TUTORIALMENU_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIALMENU_POSX - TUTORIALMENU_SIZEX, TUTORIALMENU_POSY + TUTORIALMENU_SIZEY, 0.0f);
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
	for (int nCntTutorialEvent = 0; nCntTutorialEvent < TUTORIALEVENT_MAX; nCntTutorialEvent++)
	{
		if (g_apTextureTutorialMenu[nCntTutorialEvent] != NULL)
		{
			g_apTextureTutorialMenu[nCntTutorialEvent]->Release();
			g_apTextureTutorialMenu[nCntTutorialEvent] = NULL;
		}
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
	TUTORIALEVENT tutorialevent = GetTutorialEvent();

	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialMenu, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureTutorialMenu[tutorialevent]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=============================================================================
//	�^�C�g�����j���[�̍X�V����
//=============================================================================
void UpdateTutorialMenu(void)
{

}