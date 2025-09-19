//=============================================================================
//
//	���݃X�R�A���� [currentscore.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "score.h"
#include "currentscore.h"
#include "ranking.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCurrentScore[2] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCurrentScore = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_CurrentScore_pos = {};
D3DXVECTOR3 g_CurrentScoreInfo_pos = {};

//=============================================================================
//	�X�R�A�̏���������
//=============================================================================
void InitCurrentScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureCurrentScore[0]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM + 1,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCurrentScore,
		NULL);

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffCurrentScore->Lock(0, 0, (void**)&pVtx, 0);

		int aTexU[MAX_NUM] = {};

		int nScore = GetScore();

		g_CurrentScore_pos = D3DXVECTOR3(RANKING_POSX, RANKING_POSY + (MAX_RANK * (RANKING_SIZEY + RANKING_DIFF)), 0.0f);

		aTexU[0] = nScore % 100000 / 10000;
		aTexU[1] = nScore % 10000 / 1000;
		aTexU[2] = nScore % 1000 / 100;
		aTexU[3] = nScore % 100 / 10;
		aTexU[4] = nScore % 10 / 1;

		for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * nCntNum), g_CurrentScore_pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_CurrentScore_pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * nCntNum), g_CurrentScore_pos.y + RANKING_SIZEY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_CurrentScore_pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_CurrentScore_pos.y + RANKING_SIZEY, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 1.0f);

			pVtx += 4;
		}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCurrentScore->Unlock();
}

//====================================
//	�X�R�A�̏I������
//====================================
void UninitCurrentScore(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		if (g_pTextureCurrentScore[nCntTex] != NULL)
		{
			g_pTextureCurrentScore[nCntTex]->Release();
			g_pTextureCurrentScore[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffCurrentScore != NULL)
	{
		g_pVtxBuffCurrentScore->Release();
		g_pVtxBuffCurrentScore = NULL;
	}
}

//====================================
//	�X�R�A�̕`�揈��
//====================================
void DrawCurrentScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCurrentScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < MAX_NUM + 1; nCntNumber++)
	{
		if (nCntNumber > MAX_NUM)
		{
			pDevice->SetTexture(0, g_pTextureCurrentScore[1]);
		}
		else
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCurrentScore[0]);
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, 2);
	}
}

//====================================
//	�X�R�A�̍X�V����
//====================================
void UpdateCurrentScore(void)
{

}