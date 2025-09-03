//=============================================================================
//
//	�E�F�[�u���� [wave.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wave.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "life.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_ENEMY		(5)			// �G�̎��
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
}

void LoadWave(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	pFile = fopen("data\\wave.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	//���[�J���ϐ��錾
	char aString[MAX_WARD] = {};
	char aStrRelease[3] = {};
	D3DXVECTOR3 pos = {};
	int type = 0;
	int life = 0;

	while (true)
	{
		(void)fscanf(pFile, "%s", &aString[0]);

		if (strcmp(&aString[0], "SETENEMY") == 0)
		{ // SETENEMY��ǂݎ����
			while (true)
			{
				(void)fscanf(pFile, "%s", &aString[0]);

				if (strcmp(&aString[0], "POS") == 0)
				{ // POS��ǂݎ����
					(void)fscanf(pFile, "%s", &aStrRelease[0]);

					(void)fscanf(pFile, "%f", &pos.x);
					(void)fscanf(pFile, "%f", &pos.y);
					(void)fscanf(pFile, "%f", &pos.z);
				}

				if (strcmp(&aString[0], "TYPE") == 0)
				{
					(void)fscanf(pFile, "%s", &aStrRelease[0]);

					(void)fscanf(pFile, "%d", &type);

				}

				if (strcmp(&aString[0], "LIFE") == 0)
				{
					(void)fscanf(pFile, "%s", &aStrRelease[0]);

					(void)fscanf(pFile, "%d", &life);
				}

				if (strcmp(&aString[0], "ENDSET") == 0)
				{
					//SetEnemy(pos, type, life);
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