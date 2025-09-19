//=============================================================================
//
//	���[�_�[�̏��� [radar.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "radar.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RADAR_RADIUS	(256.0f)		// ���[�_�[�̔��a

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRadar = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRadar = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_radarpos = {};									// ���[�_�[�̈ʒu
float g_fLengthRadar = NULL;
float g_fAngleRadar = NULL;

//====================================
//	���[�_�[�̏���������
//====================================
void InitRadar(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntRadar = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\radar000.png",
		&g_pTextureRadar);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRadar,
		NULL);

	Player* pPlayer = GetPlayer();

	g_fLengthRadar = SQRTF(RADAR_RADIUS, RADAR_RADIUS);
	g_fAngleRadar = atan2f(RADAR_RADIUS, RADAR_RADIUS);

	g_radarpos = pPlayer->pos;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffRadar->Lock(0, 0, (void * *)&pVtx, 0);

	pVtx[0].pos.x = g_radarpos.x + sinf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.y = g_radarpos.y + cosf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_radarpos.x + sinf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.y = g_radarpos.y + cosf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_radarpos.x + sinf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.y = g_radarpos.y + cosf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_radarpos.x + sinf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.y = g_radarpos.y + cosf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRadar->Unlock();
}

//=============================================================================
//	���[�_�[�̏I������
//=============================================================================
void UninitRadar(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureRadar != NULL)
	{
		g_pTextureRadar->Release();
		g_pTextureRadar = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRadar != NULL)
	{
		g_pVtxBuffRadar->Release();
		g_pVtxBuffRadar = NULL;
	}
}

//=============================================================================
//	���[�_�[�̕`�揈��
//=============================================================================
void DrawRadar(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Player* pPlayer = GetPlayer();

	if (pPlayer->state != PLAYERSTATE_WAIT)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffRadar, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRadar);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,  0, 2);

	}
}

//=============================================================================
//	���[�_�[�̍X�V����
//=============================================================================
void UpdateRadar(void)
{
	Player* pPlayer = GetPlayer();				// �v���C���[�̏����擾

	D3DXVECTOR3* pCameraPos = GetCamera();		// �J�����̏����擾

	VERTEX_2D* pVtx;							// ���_���ւ̃|�C���^

	g_radarpos = pPlayer->pos;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffRadar->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z + g_fAngleRadar) * g_fLengthRadar;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z - g_fAngleRadar) * g_fLengthRadar;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z + D3DX_PI - g_fAngleRadar) * g_fLengthRadar;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_radarpos.x - pCameraPos->x + sinf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.y = g_radarpos.y - pCameraPos->y + cosf(pPlayer->rot.z + D3DX_PI + g_fAngleRadar) * g_fLengthRadar;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRadar->Unlock();
}

