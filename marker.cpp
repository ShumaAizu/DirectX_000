//=============================================================================
//
//	�}�[�J�[���� [marker.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "marker.h"
#include "enemy.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MARKER		(MAX_ENEMY)							// �}�[�J�[�̍ő吔

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureMarker = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMarker = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Marker g_amarker[MAX_MARKER] = {};

//=============================================================================
//	�}�[�J�[�̏���������
//=============================================================================
void InitMarker(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\option000.png",
		&g_apTextureMarker);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MARKER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMarker,
		NULL);

	// ������

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffMarker->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		g_amarker[nCntMarker].pos = D3DXVECTOR3(0.0f, 50.0f * nCntMarker, 0.0f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.x + SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.x + SCREEN_HEIGHT, 0.0f);

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

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMarker->Unlock();
}

//=============================================================================
//	�}�[�J�[�̏I������
//=============================================================================
void UninitMarker(void)
{
	// �e�N�X�`���̔j��
	if (g_apTextureMarker != NULL)
	{
		g_apTextureMarker->Release();
		g_apTextureMarker = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMarker != NULL)
	{
		g_pVtxBuffMarker->Release();
		g_pVtxBuffMarker = NULL;
	}
}

//=============================================================================
//	�}�[�J�[�̕`�揈��
//=============================================================================
void DrawMarker(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMarker, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureMarker);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMarker * 4, 2);
	}
}

//=============================================================================
//	�}�[�J�[�̍X�V����
//=============================================================================
void UpdateMarker(void)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	Enemy* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			continue;
		}


	}

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		if (g_amarker[nCntMarker].bUse == true)
		{

		}

		pVtx += 4;
	}

	//for (nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	//{
	//	// ���_���W�̐ݒ�
	//	pVtx[0].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.y, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.y, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x, g_amarker[nCntMarker].pos.y + SCREEN_HEIGHT, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(g_amarker[nCntMarker].pos.x + SCREEN_WIDTH, g_amarker[nCntMarker].pos.y + SCREEN_HEIGHT, 0.0f);

	//	pVtx += 4;
	//}


	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMarker->Unlock();
}