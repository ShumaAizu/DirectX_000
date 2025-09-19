//=============================================================================
//
//	�^�C�g���w�i���� [titlelogo.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlelogo.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLELOGO_DEFAULTPOSX	(640.0f)
#define TITLELOGO_DEFAULTPOSY	(180.0f)
#define TITLELOGO_SIZEX			(500.0f)
#define TITLELOGO_SIZEY			(250.0f)
#define TITLELOGO_INITPOSX		(1280.0f + TITLELOGO_SIZEX)
#define TITLELOGO_INITPOSY		(TITLELOGO_DEFAULTPOSY)
#define TITLELOGO_SPEED			(10.0f)

////*****************************************************************************
//// �^�C�g�����S�\����
////*****************************************************************************
//typedef struct
//{
//	D3DXVECTOR3 pos;		// �ʒu
//	bool bDisp;				// �\�����
//}TitleLogo;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_titleLogo_pos = {};							// �^�C�g�����S�̈ʒu
int g_nStateChangeCounter = 0;

//====================================
//	�^�C�g���̏���������
//====================================
void InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\Title_logo.png",
			&g_pTextureTitleLogo);
	

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	g_titleLogo_pos = { TITLELOGO_INITPOSX , TITLELOGO_INITPOSY , 0.0f };
	g_nStateChangeCounter = 0;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void * *)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TITLELOGO_INITPOSX - TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLELOGO_INITPOSX + TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLELOGO_INITPOSX - TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLELOGO_INITPOSX + TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();
}

//====================================
//	�^�C�g���̏I������
//====================================
void UninitTitleLogo(void)
{

	// �e�N�X�`���̔j��
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
	

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//====================================
//	�^�C�g���̕`�揈��
//====================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//====================================
//	�^�C�g���̍X�V����
//====================================
void UpdateTitleLogo(void)
{
	TITLESTATE TitleState = GetTitleState();

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	switch (TitleState)
	{
	case TITLESTATE_OP:
		g_titleLogo_pos.x -= TITLELOGO_SPEED;

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{
			g_titleLogo_pos = { TITLELOGO_DEFAULTPOSX , TITLELOGO_DEFAULTPOSY , 0.0f };
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_titleLogo_pos.x - TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_titleLogo_pos.x + TITLELOGO_SIZEX, TITLELOGO_INITPOSY - TITLELOGO_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_titleLogo_pos.x - TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_titleLogo_pos.x + TITLELOGO_SIZEX, TITLELOGO_INITPOSY + TITLELOGO_SIZEY, 0.0f);


		if (g_titleLogo_pos.x <= TITLELOGO_DEFAULTPOSX)
		{
			SetRainbowParticle(g_titleLogo_pos, 5000.0f, 30, D3DX_PI, -D3DX_PI);
			SetRainbowParticle(g_titleLogo_pos, 5000.0f, 30, D3DX_PI, -D3DX_PI);
			PlaySound(SOUND_LABEL_SE_TITLESET000);
			g_nStateChangeCounter = 60;
			SetTitleState(TITLESTATE_WAIT);
		}
		break;

	case TITLESTATE_WAIT:
		g_nStateChangeCounter--;
		if (g_nStateChangeCounter <= 0)
		{
			SetTitleState(TITLESTATE_MENU);
		}
		break;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();
}