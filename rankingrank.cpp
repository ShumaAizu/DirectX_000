//=============================================================================
//
//	�����L���O��ʏ��� [rankingrank.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "ranking.h"
#include "rankingrank.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKINGRANKFADE_TIMER		(300)		// �^�C�}�[�̕b��

//*****************************************************************************
// �����L���O�\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	int nScore;					// �X�R�A
	bool bDisp;					// �\�����
}RankingRank;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
RankingRank g_arankingrank[MAX_RANK] = {};						// �����L���O���

//====================================
//	�����L���O�̏���������
//====================================
void InitRankingRank(void)
{

	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rank.png",
		&g_pTextureRankingRank);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingRank->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{

		g_arankingrank[nCntRank].pos = D3DXVECTOR3(RANKINGRANK_POSX, RANKINGRANK_POSY + (nCntRank * (RANKINGRANK_SIZEY + RANKINGRANK_DIFF)), 0.0f);
		g_arankingrank[nCntRank].bDisp = true;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x, g_arankingrank[nCntRank].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x + RANKINGRANK_SIZEX, g_arankingrank[nCntRank].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x, g_arankingrank[nCntRank].pos.y + RANKINGRANK_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x + RANKINGRANK_SIZEX, g_arankingrank[nCntRank].pos.y + RANKINGRANK_SIZEY, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.2f * nCntRank);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.2f * nCntRank);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f * (nCntRank + 1));
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f * (nCntRank + 1));

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingRank->Unlock();
}

//====================================
//	�����L���O�̏I������
//====================================
void UninitRankingRank(void)
{

	// �e�N�X�`���̔j��
	if (g_pTextureRankingRank != NULL)
	{
		g_pTextureRankingRank->Release();
		g_pTextureRankingRank = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;
	}
}

//====================================
//	�����L���O�̕`�揈��
//====================================
void DrawRankingRank(void)
{

	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankingRank);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (g_arankingrank[nCntRank].bDisp == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
		}
	}
}

//====================================
//	�����L���O�̍X�V����
//====================================
void UpdateRankingRank(void)
{

}