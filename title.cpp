//=============================================================================
//
//	�^�C�g����ʏ��� [title.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlemenu.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TITLE_UI		(4)
#define TITLEFADE_TIMER		(300)		// �^�C�}�[�̕b��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_UI] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
TITLESTATE g_titleState = TITLESTATE_OP;							// �^�C�g���̏��
int g_nCounterTitleState = 0;										// ��ԊǗ��J�E���^�[
int g_nTitleFadeCounter = 0;

//====================================
//	�^�C�g���̏���������
//====================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	const char* pTitle_UI[MAX_TITLE_UI] =
	{
		"data\\TEXTURE\\bg000.jpg",
		"data\\TEXTURE\\Title.png",
		"data\\TEXTURE\\Title_GAMESTART.png",
		"data\\TEXTURE\\Title_EXIT.png",
	};

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_UI; nCntTitle++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			pTitle_UI[nCntTitle],
			&g_pTextureTitle[nCntTitle]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void * *)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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
	g_pVtxBuffTitle->Unlock();

	g_nTitleFadeCounter = 0;

	// �^�C�g�����j���[�̏�����
	InitTitleMenu();

	PlaySound(SOUND_LABEL_BGM000);
}

//====================================
//	�^�C�g���̏I������
//====================================
void UninitTitle(void)
{
	// �^�C�g�����j���[�̏I������
	UninitTitleMenu();

	// �T�E���h���~�߂�
	StopSound();

	// �e�N�X�`���̔j��
	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_UI; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//====================================
//	�^�C�g���̕`�揈��
//====================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_UI; nCntTitle++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}

	// �^�C�g�����j���[�̕`�揈��
	DrawTitleMenu();
}

//====================================
//	�^�C�g���̍X�V����
//====================================
void UpdateTitle(void)
{
	g_nTitleFadeCounter++;

	//if (GetJoypadTrigger(JOYKEY_START) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	//{// ����L�[�������ꂽ
	//	// ���[�h�ݒ�
	//	SetFade(MODE_TUTORIAL);
	//}

	if (g_nTitleFadeCounter >= TITLEFADE_TIMER)
	{// �J�E���^�[���K��l�𒴂���
		// ���[�h�ݒ�
		SetFade(MODE_RANKING);
	}

	// �^�C�g�����j���[�̍X�V����
	UpdateTitleMenu();
}

//========================================
//	�^�C�g���̏�Ԑݒ�
//========================================
void SetTitleState(TITLESTATE titlestate, int nCounter)
{
	g_titleState = titlestate;

	g_nCounterTitleState = nCounter;
}