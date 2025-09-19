//=============================================================================
//
//	�X�R�A���� [score.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "score.h"
#include "powerup.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCORE		(50000)				// �X�R�A�̍ő�
#define MAX_SCORE_UI	(SCOREUI_MAX)		// �X�R�A��UI�̐�
#define SCORE_POSX		(880.0f)			// �X�R�A�̍��WX
#define SCORE_POSY		(672.0f)			// �X�R�A�̍��WY
#define SCORE_SIZEX		(480.0f)			// �X�R�A�̃T�C�YX
#define SCORE_SIZEY		(32.0f)				// �X�R�A�̃T�C�YY
#define INIT_SCORE		(10000)				// �X�R�A�̏����l

//*****************************************************************************
// �X�R�AUI�\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	SCOREUI type;				// �X�R�AUI�̎��
}ScoreUI;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureScore[MAX_SCORE_UI] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
ScoreUI g_ascoreUI[MAX_SCORE_UI] = {};								// �X�R�AUI�̏��
int g_nScore;														// �X�R�A�̒l

//=============================================================================
//	�X�R�A�̏���������
//=============================================================================
void InitScore(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore = 0;

	const char* pUIPass[MAX_SCORE_UI] = {
		"data\\TEXTURE\\Score_UI000.png",
		"data\\TEXTURE\\Score_UI001.png",
		"data\\TEXTURE\\Score_UI002.png"
	};

	// ������
	g_nScore = 0;

	for (int nCntScoreTex = 0; nCntScoreTex < MAX_SCORE_UI; nCntScoreTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, pUIPass[nCntScoreTex], &g_pTextureScore[nCntScoreTex]);
	}


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE_UI,		// ���̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCORE_UI; nCntScore++)
	{
		g_ascoreUI[nCntScore].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
		g_ascoreUI[nCntScore].type = (SCOREUI)nCntScore;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SCORE_POSX - SCORE_SIZEX, SCORE_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCORE_POSX - SCORE_SIZEX, SCORE_POSY + SCORE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY + SCORE_SIZEY, 0.0f);

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

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

	SetScore(INIT_SCORE);
}

//====================================
//	�X�R�A�̏I������
//====================================
void UninitScore(void)
{

	g_nScore = 0;

	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MAX_SCORE_UI; nCntTex++)
	{
		if (g_pTextureScore[nCntTex] != NULL)
		{
			g_pTextureScore[nCntTex]->Release();
			g_pTextureScore[nCntTex] = NULL;
		}
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

	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_UI; nCntNumber++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore[nCntNumber]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, 2);
	}
}

//====================================
//	�X�R�A�̍X�V����
//====================================
void UpdateScore(void)
{
	float fRatio = 0;

	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	bool bPowerUp = GetPowerUp();

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScoreUI = 0; nCntScoreUI < MAX_SCORE_UI; nCntScoreUI++)
	{
		if (g_ascoreUI[nCntScoreUI].type != SCOREUI_CURTAIN)
		{
			continue;
		}

		fRatio = ((float)g_nScore / MAX_SCORE);

		pVtx += (nCntScoreUI * 4);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SCORE_POSX - (SCORE_SIZEX - SCORE_SIZEX * fRatio), SCORE_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCORE_POSX - (SCORE_SIZEX - SCORE_SIZEX * fRatio), SCORE_POSY + SCORE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCORE_POSX, SCORE_POSY + SCORE_SIZEY, 0.0f);

		if (bPowerUp == true)
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//====================================
//	�X�R�A�̐ݒ菈��
//====================================
void SetScore(int nScore)
{
	//int aTexU[NUM_PLACE];		// �e���̐������i�[
	int nCntNumber = 0;
	g_nScore = nScore;

	//aTexU[0] = g_nScore % 100000000 / 10000000;
	//aTexU[1] = g_nScore % 10000000 / 1000000;
	//aTexU[2] = g_nScore % 1000000 / 100000;
	//aTexU[3] = g_nScore % 100000 / 10000;
	//aTexU[4] = g_nScore % 10000 / 1000;
	//aTexU[5] = g_nScore % 1000 / 100;
	//aTexU[6] = g_nScore % 100 / 10;
	//aTexU[7] = g_nScore % 10 / 1;
}

//====================================
//	�X�R�A�̉��Z����
//====================================
void AddScore(int nValue)
{
	g_nScore += nValue;
}

//====================================
//	�X�R�A�̌��Z����
//====================================
void SubScore(int nValue)
{
	g_nScore -= nValue;
}

//====================================
//	�X�R�A�̎擾
//====================================
int GetScore(void)
{
	return g_nScore;
}