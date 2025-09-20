//=============================================================================
//
//	�E�F�[�u���� [wave.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wave.h"
#include "input.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "time.h"
#include "sound.h"
#include "life.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_WARD		(256)		// ������̍ő�
#define WAVE_TIME		(900)		// �E�F�[�u�̕b��(�t���[��)
#define MAX_WAVE		(4)			// �E�F�[�u�̍ő吔

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWave = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
int g_nTimeLine = 0;
int g_nWave = 0;

//====================================
//	�G�̏���������
//====================================
void InitWave(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTimeLine = 0;

	g_nWave = 3;

	LoadWave(g_nWave);
}

//=============================================================================
//	�G�̏I������
//=============================================================================
void UninitWave(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffWave != NULL)
	{
		g_pVtxBuffWave->Release();
		g_pVtxBuffWave = NULL;
	}
}

//=============================================================================
//	�G�̕`�揈��
//=============================================================================
void DrawWave(void)
{

}

//=============================================================================
//	�G�̍X�V����
//=============================================================================
void UpdateWave(void)
{
	int* pNumEnemy = GetNumEnemy();		// �G�̐����擾

	Enemy* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->nTimeLine == g_nTimeLine)
		{// �^�C�����C�����d�Ȃ�����
			pEnemy->bUse = true;		// �o��
		}
	}

	g_nTimeLine++;			// �^�C�����C����i�߂�

	while (true)
	{
		if (*pNumEnemy != 0 && CheckEnemy() == false)
		{// �E�F�[�u�̓G���c���Ă��邩�ՖʂɓG�����Ȃ�
			Enemy* pEnemy = GetEnemy();
			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->nTimeLine == g_nTimeLine)
				{// �^�C�����C�����d�Ȃ��Ă�����o��
					PlaySound(SOUND_LABEL_SE_APPEAR000);
					pEnemy->bUse = true;
				}
			}
		}

		if (*pNumEnemy == 0 || CheckEnemy() == true)
		{// �E�F�[�u�̓G�����Ȃ��Ȃ��� / �G���o�Ă���
			break;
		}

		// ���[�v���Ƀ^�C�����C����i�߂�
		g_nTimeLine++;
	}


	if (g_nTimeLine >= WAVE_TIME || *pNumEnemy == 0)
	{
		g_nWave++;
		g_nTimeLine = 0;
		if (g_nWave < MAX_WAVE)
		{
			LoadWave(g_nWave);
		}
	}

	if (((*pNumEnemy <= 0 && g_nWave > MAX_WAVE) || GetTime() <= 0 || GetScore() <= 0) && GetGameState() != GAMESTATE_END)
	{// �G�����Ȃ��Ȃ��� / �^�C�����Ȃ��Ȃ��� / �X�R�A���Ȃ��Ȃ���
		SetGameState(GAMESTATE_END, 60);
	}
}

//=============================================================================
//	�G�̃��[�h����
//=============================================================================
void LoadWave(int nWave)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	int* pNumEnemy = GetNumEnemy();

	const char* pWaveFileName[MAX_WAVE] =
	{
		"data\\WAVE\\wave000.txt",
		"data\\WAVE\\wave001.txt",
		"data\\WAVE\\wave002.txt",
		"data\\WAVE\\wave003.txt"
	};

	pFile = fopen(pWaveFileName[nWave], "r");

	if (pFile == NULL)
	{// �J���Ȃ�������
		return;
	}

	//���[�J���ϐ��錾
	char aString[MAX_WARD];		// �������ǂݍ���
	char aStrRelease[3];		// �s�v�� = ��ǂݍ���

	// ���ꂼ��̒l��ǂݍ���
	int nNumEnemy = {};
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 rot = {};
	float fMove = 0;
	float fRadius = 0;
	int type = 0;
	int life = 0;
	int timeline = 0;
	int nScore = 0;
	int nData;

	while (true)
	{
		nData = fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "NUM_ENEMY") == 0)
		{
			fscanf(pFile, "%s", &aStrRelease[0]);

			fscanf(pFile, "%d", &nNumEnemy);

			*pNumEnemy += nNumEnemy;
		}

		if (strcmp(&aString[0], "SETENEMY") == 0)
		{// SETENEMY��ǂݎ����
			while (true)
			{
				nData = fscanf(pFile, "%s", &aString[0]);

				if (strcmp(&aString[0], "POS") == 0)
				{// POS��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &pos.x);
					fscanf(pFile, "%f", &pos.y);
					fscanf(pFile, "%f", &pos.z);
				}

				if (strcmp(&aString[0], "ROT") == 0)
				{// ROT��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &rot.z);
				}

				if (strcmp(&aString[0], "RADIUS") == 0)
				{// RADIUS��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &fRadius);
				}

				if (strcmp(&aString[0], "MOVE") == 0)
				{// MOVE��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%f", &fMove);
				}

				if (strcmp(&aString[0], "TYPE") == 0)
				{// TYPE��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%d", &type);

				}

				if (strcmp(&aString[0], "LIFE") == 0)
				{// LIFE��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%d", &life);
				}

				if (strcmp(&aString[0], "TIMELINE") == 0)
				{// LIFE��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%d", &timeline);
				}

				if (strcmp(&aString[0], "SCORE") == 0)
				{// SCORE��ǂݎ����
					fscanf(pFile, "%s", &aStrRelease[0]);

					fscanf(pFile, "%d", &nScore);
				}

				if (strcmp(&aString[0], "ENDSET") == 0)
				{// ENDSET��ǂݎ����
					SetEnemy(pos, rot, fRadius, fMove, type, life, timeline, nScore);
					break;
				}
			}
		}

		if (strcmp(&aString[0], "END_SCRIPT") == 0)
		{
			break;
		}
	}

	fclose(pFile);
}