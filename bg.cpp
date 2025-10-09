//=============================================================================
//
//	�w�i���� [bg.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "camera.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BG		(3)								// ���d�w�i�̍ő吔

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBg[MAX_BG] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Bg g_abg[MAX_BG];

//=============================================================================
//	�w�i�̏���������
//=============================================================================
void InitBg(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG001.jpg",
		&g_apTextureBg[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_apTextureBg[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_apTextureBg[2]);


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	// ������

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		g_abg[nCntBg].pos = D3DXVECTOR3(0.0f, 50.0f * nCntBg, 0.0f);
		g_abg[nCntBg].move = D3DXVECTOR3(0.0f, 1.0f + nCntBg, 0.0f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x, g_abg[nCntBg].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x + WARLD_WIDTH, g_abg[nCntBg].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x, g_abg[nCntBg].pos.x + WARLD_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x + WARLD_WIDTH, g_abg[nCntBg].pos.x + WARLD_HEIGHT, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
//	�w�i�̏I������
//=============================================================================
void UninitBg(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (g_apTextureBg[nCntBg] != NULL)
		{
			g_apTextureBg[nCntBg]->Release();
			g_apTextureBg[nCntBg] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
//	�w�i�̕`�揈��
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBg = 0; nCntBg < 1; nCntBg++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureBg[nCntBg]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//=============================================================================
//	�w�i�̍X�V����
//=============================================================================
void UpdateBg(void)
{
	int nCntBg;
	
	//for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	//{
	//	g_abg[nCntBg].pos += g_abg[nCntBg].move;

	//	if (g_abg[nCntBg].pos.y >= SCREEN_HEIGHT)
	//	{
	//		g_abg[nCntBg].pos.y = 0.0f - 960;
	//	}
	//}

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x, g_abg[nCntBg].pos.y - pCameraPos->y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x + WARLD_WIDTH, g_abg[nCntBg].pos.y - pCameraPos->y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x, g_abg[nCntBg].pos.y - pCameraPos->y + WARLD_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_abg[nCntBg].pos.x - pCameraPos->x + WARLD_WIDTH, g_abg[nCntBg].pos.y - pCameraPos->y + WARLD_HEIGHT, 0.0f);

		pVtx += 4;
	}


	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}