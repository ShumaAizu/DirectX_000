//=============================================================================
//
//	�p�[�e�B�N������ [particle.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "particle.h"
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTICLE		(128)		// �p�[�e�B�N���̍ő吔

//*****************************************************************************
// �p�[�e�B�N���\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	float fRadius;			// ���a
	int nLife;				// ����
	bool bUse;				// �g�p��
}Particle;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Particle g_aParticle[MAX_PARTICLE];			// �p�[�e�B�N���̏��

void InitParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = {};
		g_aParticle[nCntParticle].col = {};
		g_aParticle[nCntParticle].fRadius = {};
		g_aParticle[nCntParticle].nLife = {};
		g_aParticle[nCntParticle].bUse = false;
	}
}

void UninitParticle(void)
{

}

void UpdateParticle(void)
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �p�[�e�B�N���̈ړ���
	D3DXCOLOR col;			// �F
	float fMove;			// �ړ���
	float fRadius;			// ���a
	int nLife;				// ����

	float fAngle;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			for (int nCntAppear = 0; nCntAppear < 5; nCntAppear++)
			{
				pos = g_aParticle[nCntParticle].pos;

				fAngle = (float)(rand() % 629 - 314) / 100.0f;
				fMove = (float)(rand() % 500) / 100.0f + 0.1;

				move.x = sinf(fAngle) * fMove;
				move.y = cosf(fAngle) * fMove;

				col = g_aParticle[nCntParticle].col;
				fRadius = g_aParticle[nCntParticle].fRadius;
				nLife = g_aParticle[nCntParticle].nLife;

				SetEffect(pos, move, col, fRadius, nLife);
			}

			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}

}

void DrawParticle(void)
{

}

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{// �G�t�F�N�g���g�p���Ă��Ȃ�
			g_aParticle[nCntParticle].pos = pos;			// �󂯎�����ʒu����
			g_aParticle[nCntParticle].col = col;			// �󂯎�����F����
			g_aParticle[nCntParticle].nLife = nLife;		// �󂯎������������
			g_aParticle[nCntParticle].fRadius = fRadius;	// �󂯎�������a����

			g_aParticle[nCntParticle].bUse = true;			// �G�t�F�N�g���g�p����Ă����Ԃɂ���
			break;		// ������for���𔲂���
		}
	}
}