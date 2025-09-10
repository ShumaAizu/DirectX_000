//=============================================================================
//
//	�E�F�[�u���� [wave.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wave.h"
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
#define MAX_WARD		(256)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWave = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
int g_nTimeLine = 0;

//====================================
//	�G�̏���������
//====================================
void InitWave(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTimeLine = 0;

	LoadWave();
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
	Enemy* pEnemy = GetEnemy();
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->nTimeLine == g_nTimeLine)
		{
			pEnemy->bUse = true;
		}
	}

	g_nTimeLine++;

	int* pNumEnemy = GetNumEnemy();		// �G�̐����擾

	if ((*pNumEnemy <= 0 || GetTime() <= 0 || GetScore() <= 0) && GetGameState() != GAMESTATE_END)
	{// �G�����Ȃ��Ȃ���/�^�C�����Ȃ��Ȃ���/�X�R�A���Ȃ��Ȃ���
		SetGameState(GAMESTATE_END, 60);
	}
}

//=============================================================================
//	�G�̃��[�h����
//=============================================================================
void LoadWave(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	int* pNumEnemy = GetNumEnemy();

	pFile = fopen("data\\wave.txt", "r");

	if (pFile != NULL)
	{// �J������
		//���[�J���ϐ��錾
		char aString[MAX_WARD];		// �������ǂݍ���
		char aStrRelease[3];		// �s�v�� = ��ǂݍ���

		// ���ꂼ��̒l��ǂݍ���
		D3DXVECTOR3 pos = {};
		D3DXVECTOR3 rot = {};
		float fMove = 0;
		int type = 0;
		int life = 0;
		int timeline = 0;
		ENEMYMOVESTATE movestate; 
		int nData;

		while (true)
		{
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM_ENEMY") == 0)
			{
				fscanf(pFile, "%s", &aStrRelease[0]);

				fscanf(pFile, "%d", pNumEnemy);
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

					if (strcmp(&aString[0], "ENDSET") == 0)
					{// ENDSET��ǂݎ����
						SetEnemy(pos, rot, fMove, type, life, timeline);
						break;
					}
				}
			}

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}