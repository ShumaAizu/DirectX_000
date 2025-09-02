//=============================================================================
//
//	�X�R�A���� [score.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PLACE		(8)			// �X�R�A�̌���
#define SCORE_POSX		(1020.0f)	// �X�R�A�̍��WX
#define SCORE_POSY		(0.0f)		// �X�R�A�̍��WY
#define SCORE_SIZEX		(30.0f)		// �X�R�A�̃T�C�YX
#define SCORE_SIZEY		(45.0f)		// �X�R�A�̃T�C�YY

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posScore;									// �X�R�A�̈ʒu
int g_nScore;											// �X�R�A�̒l

//=============================================================================
//	�X�R�A�̏���������
//=============================================================================
void InitScore(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore = 0;

	// ������
	g_posScore = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
	g_nScore = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\number000.png", &g_pTextureScore);


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,		// ���̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SCORE_POSX + (nCntScore * SCORE_SIZEX), SCORE_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_POSX + (nCntScore * SCORE_SIZEX) + SCORE_SIZEX, SCORE_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCORE_POSX + (nCntScore * SCORE_SIZEX), SCORE_POSY + SCORE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCORE_POSX + (nCntScore * SCORE_SIZEX) + SCORE_SIZEX, SCORE_POSY + SCORE_SIZEY, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//====================================
//	�X�R�A�̏I������
//====================================
void UninitScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//====================================
//	�X�R�A�̕`�揈��
//====================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntNumber = 0; nCntNumber < NUM_PLACE; nCntNumber++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, 2);
	}
}

//====================================
//	�X�R�A�̍X�V����
//====================================
void UpdateScore(void)
{

}

//====================================
//	�X�R�A�̐ݒ菈��
//====================================
void SetScore(int nScore)
{
	int aTexU[NUM_PLACE];		// �e���̐������i�[
	int nCntNumber = 0;
	g_nScore = nScore;

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	// �e�N�X�`�����W�̐ݒ�
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntNumber = 0; nCntNumber < NUM_PLACE; nCntNumber++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNumber], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNumber] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNumber], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNumber] + 1), 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//====================================
//	�X�R�A�̉��Z����
//====================================
void AddScore(int nValue)
{
	int aTexU[NUM_PLACE];		// �e���̐������i�[
	g_nScore += nValue;

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	// �e�N�X�`�����W�̐ݒ�
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumber = 0; nCntNumber < NUM_PLACE; nCntNumber++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNumber], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNumber] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNumber], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNumber] + 1), 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}