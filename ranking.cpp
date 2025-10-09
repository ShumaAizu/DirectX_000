//=============================================================================
//
//	�����L���O��ʏ��� [ranking.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "ranking.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "rankingbg.h"
#include "rankingrank.h"
#include "currentscore.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKINGFADE_TIMER		(300)		// �^�C�}�[�̕b��

//*****************************************************************************
// �����L���O�\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	int nScore;					// �X�R�A
	bool bDisp;					// �\�����
}Ranking;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Ranking g_aranking[MAX_RANK] = {};							// �����L���O���
int g_nRankUpdate = -1;										// �X�V�����NNo.
int g_nRankingFadeCounter = 0;
bool g_bRanking_Modechange = false;

//====================================
//	�����L���O�̏���������
//====================================
void InitRanking(void)
{
	InitRankingBG();

	InitRankingRank();

	InitCurrentScore();

	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureRanking);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_RANK * MAX_NUM),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	g_bRanking_Modechange = false;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		int aTexU[MAX_NUM] = {};

		g_aranking[nCntRank].pos = D3DXVECTOR3(RANKING_POSX, RANKING_POSY + (nCntRank * (RANKING_SIZEY + RANKING_DIFF)), 0.0f);
		g_aranking[nCntRank].bDisp = true;

		aTexU[0] = g_aranking[nCntRank].nScore % 100000 / 10000;
		aTexU[1] = g_aranking[nCntRank].nScore % 10000 / 1000;
		aTexU[2] = g_aranking[nCntRank].nScore % 1000 / 100;
		aTexU[3] = g_aranking[nCntRank].nScore % 100 / 10;
		aTexU[4] = g_aranking[nCntRank].nScore % 10 / 1;

		for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * nCntNum), g_aranking[nCntRank].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_aranking[nCntRank].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * nCntNum), g_aranking[nCntRank].pos.y + RANKING_SIZEY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_aranking[nCntRank].pos.y + RANKING_SIZEY, 0.0f);

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
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

	g_nRankingFadeCounter = 0;

	PlaySound(SOUND_LABEL_BGM_RANKING000);
}

//====================================
//	�����L���O�̏I������
//====================================
void UninitRanking(void)
{
	// �T�E���h���~�߂�
	StopSound();

	UninitRankingBG();

	UninitRankingRank();

	UninitCurrentScore();

	SetGameState(GAMESTATE_NORMAL, 60);

	g_nRankUpdate = -1;

	// �e�N�X�`���̔j��
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//====================================
//	�����L���O�̕`�揈��
//====================================
void DrawRanking(void)
{
	DrawRankingBG();

	DrawRankingRank();

	if (GetGameState()== GAMESTATE_END)
	{
		DrawCurrentScore();
	}

	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (g_aranking[nCntRank].bDisp == true)
		{
			for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
			{
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntRank * MAX_NUM) + nCntNum) * 4, 2);
			}
		}
	}
}

//====================================
//	�����L���O�̍X�V����
//====================================
void UpdateRanking(void)
{
	UpdateRankingBG();

	UpdateRankingRank();

	UpdateCurrentScore();

	g_nRankingFadeCounter++;
	static int nRankDispCounter = 0;

	if (g_nRankUpdate != -1)
	{
		nRankDispCounter++;
		if (nRankDispCounter % 10 == 0)
		{
			g_aranking[g_nRankUpdate].bDisp = g_aranking[g_nRankUpdate].bDisp ? false : true;
		}
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true) && GetFade() != FADE_OUT)
	{// ����L�[�������ꂽ
		// ���[�h�ݒ�
		if (g_bRanking_Modechange == false)
		{
			PlaySound(SOUND_LABEL_SE_SELECT000);
		}
		g_bRanking_Modechange = true;
		SetFade(MODE_TITLE, 0.025f, 0.025f);
	}

	if (g_nRankingFadeCounter >= RANKINGFADE_TIMER && GetFade() != FADE_OUT)
	{
		SetFade(MODE_TITLE, 0.025f, 0.025f);
	}
}

//====================================
//	�����L���O�̃��Z�b�g����
//====================================
void ResetRanking(void)
{
	int nScoreRank[MAX_RANK] = {};

	FILE* pFile;
#ifdef _DEBUG
	pFile = fopen("data\\Rankingtest.bin", "rb");
#else
	pFile = fopen("data\\Ranking001.bin", "rb");
#endif

	if (pFile == NULL)
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			nScoreRank[nCntRank] = (nCntRank + 1) * 2500;
		}

		qsort(&nScoreRank[0], MAX_RANK, sizeof(int), Sort_desc);		// �\�[�g

		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			g_aranking[nCntRank].nScore = nScoreRank[nCntRank];
		}
		return;
	}

	fread(&nScoreRank[0], sizeof(int), MAX_RANK, pFile);

	fclose(pFile);

	for (int nCntScoreRank = 0; nCntScoreRank < MAX_RANK; nCntScoreRank++)
	{
		g_aranking[nCntScoreRank].nScore = nScoreRank[nCntScoreRank];
	}
}

//====================================
//	�����L���O�̐ݒ菈��
//====================================
void SetRanking(int nScore)
{
	int nCntScoreRank = 0;
	int nScoreRank[MAX_RANK + 1] = {};		// �V�����X�R�A���܂߂��R�s�[�z��

	// �S���R�s�[
	for (nCntScoreRank = 0; nCntScoreRank < MAX_RANK; nCntScoreRank++)
	{
		nScoreRank[nCntScoreRank] = g_aranking[nCntScoreRank].nScore;
	}

	nScoreRank[nCntScoreRank] = nScore;		// �Ō�ɍ���̃X�R�A

	qsort(&nScoreRank[0], MAX_RANK + 1, sizeof(int), Sort_desc);		// �\�[�g

	// �����L���O�̏��ɖ߂�
	for (nCntScoreRank = 0; nCntScoreRank < MAX_RANK; nCntScoreRank++)
	{
		g_aranking[nCntScoreRank].nScore = nScoreRank[nCntScoreRank];
		if (g_aranking[nCntScoreRank].nScore == nScore && g_nRankUpdate == -1)
		{// ����̃X�R�A�������N�C�����Ă�����
			g_nRankUpdate = nCntScoreRank;		// �i���o�[���L�^
		}
	}

	FILE* pFile;		// �t�@�C���ւ̃|�C���^

#ifdef _DEBUG
	pFile = fopen("data\\Rankingtest.bin", "wb");
#else
	// �t�@�C�����������݃��[�h�ŊJ��
	pFile = fopen("data\\Ranking001.bin", "wb");
#endif
	if (pFile == NULL)
	{// �t�@�C�����J���Ȃ�������
		return;
	}

	// ��������
	fwrite(&nScoreRank[0], sizeof(int), MAX_RANK, pFile);

	fclose(pFile);			// �t�@�C�������

}

//====================================
//	�X�R�A�̃\�[�g����
//====================================
int Sort_desc(const void* pCastScore1, const void* pCastScore2)
{
	//int�^�̃|�C���^��錾
	const int* pScore1;
	const int* pScore2;

	//������int�^�ɃL���X�g���đ��
	pScore1 = (const int*)pCastScore1;
	pScore2 = (const int*)pCastScore2;

	//�ϐ��̒l���r
	return *pScore2 - *pScore1;
}