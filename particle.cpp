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
	PARTICLETYPE type;		// ���
	float fStartAngle;		// �J�n�n�_�p�x
	float fEndAngle;		// �I���n�_�p�x
	float fRadius;			// ���a
	int nLife;				// ����
	bool bUse;				// �g�p��
}Particle;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Particle g_aParticle[MAX_PARTICLE];			// �p�[�e�B�N���̏��

//====================================
//	�p�[�e�B�N���̏���������
//====================================
void InitParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = {};
		g_aParticle[nCntParticle].col = {0.0f, 0.0f, 0.0f, 1.0f};
		g_aParticle[nCntParticle].type = PARTICLETYPE_NORMAL;
		g_aParticle[nCntParticle].fRadius = {};
		g_aParticle[nCntParticle].fStartAngle = {};
		g_aParticle[nCntParticle].fEndAngle = {};
		g_aParticle[nCntParticle].nLife = {};
		g_aParticle[nCntParticle].bUse = false;
	}
}

//====================================
//	�p�[�e�B�N���̏I������
//====================================
void UninitParticle(void)
{

}

//====================================
//	�p�[�e�B�N���̍X�V����
//====================================
void UpdateParticle(void)
{
	D3DXVECTOR3 pos = {};		// �ʒu
	D3DXVECTOR3 move = {};		// �p�[�e�B�N���̈ړ���
	D3DXCOLOR col = {};			// �F
	float fMove = {};			// �ړ���
	float fRadius = {};			// ���a
	int nLife = {};				// ����

	float fAngle = {};
	int nStartAngle = {};
	int nEndAngle = {};

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
			{
				pos = g_aParticle[nCntParticle].pos;

				//pos.x += (float)(rand() % 300) / 100.0f + 1.0f;
				//pos.y += (float)(rand() % 300) / 100.0f + 1.0f;

				nStartAngle = (int)(g_aParticle[nCntParticle].fStartAngle * 100) + 1;
				nEndAngle = (int)(g_aParticle[nCntParticle].fEndAngle * 100);

				nStartAngle = nStartAngle - nEndAngle;

				fAngle = (float)(rand() % nStartAngle + nEndAngle) / 100.0f;

				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				else if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}

				fMove = (float)(rand() % 500) / 100.0f + 0.5f;

				move.x = sinf(fAngle) * fMove;
				move.y = cosf(fAngle) * fMove;

				switch (g_aParticle[nCntParticle].type)
				{
				case PARTICLETYPE_NORMAL:
					col = g_aParticle[nCntParticle].col;
					break;

				case PARTICLETYPE_RAINBOW:
					col.r = (float)(rand() % 101) / 100.0f;
					col.g = (float)(rand() % 101) / 100.0f;
					col.b = (float)(rand() % 101) / 100.0f;
					col.a = 1.0f;
					break;

				}

				fRadius = (float)(rand() % (int)g_aParticle[nCntParticle].fRadius) / 100.0f + 5.0f;
				nLife = 200;

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

//====================================
//	�p�[�e�B�N���̕`�揈��
//====================================
void DrawParticle(void)
{

}

//====================================
//	�p�[�e�B�N���̐ݒ菈��
//====================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, float fStartAngle, float fEndAngle)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{// �p�[�e�B�N�����g�p���Ă��Ȃ�
			g_aParticle[nCntParticle].pos = pos;			// �󂯎�����ʒu����
			g_aParticle[nCntParticle].col = col;			// �󂯎�����F����
			g_aParticle[nCntParticle].nLife = nLife;		// �󂯎������������
			g_aParticle[nCntParticle].fRadius = fRadius;	// �󂯎�������a����
			g_aParticle[nCntParticle].fStartAngle = fStartAngle;
			g_aParticle[nCntParticle].fEndAngle = fEndAngle;
			g_aParticle[nCntParticle].type = PARTICLETYPE_NORMAL;
			g_aParticle[nCntParticle].bUse = true;			// �G�t�F�N�g���g�p����Ă����Ԃɂ���
			break;		// ������for���𔲂���
		}
	}
}

//====================================
//	�p�[�e�B�N���̐ݒ菈��
//====================================
void SetRainbowParticle(D3DXVECTOR3 pos, float fRadius, int nLife, float fStartAngle, float fEndAngle)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{// �p�[�e�B�N�����g�p���Ă��Ȃ�
			g_aParticle[nCntParticle].pos = pos;			// �󂯎�����ʒu����
			g_aParticle[nCntParticle].nLife = nLife;		// �󂯎������������
			g_aParticle[nCntParticle].fRadius = fRadius;	// �󂯎�������a����
			g_aParticle[nCntParticle].fStartAngle = fStartAngle;
			g_aParticle[nCntParticle].fEndAngle = fEndAngle;

			g_aParticle[nCntParticle].col.r = (float)(rand() % 101) / 100.0f;
			g_aParticle[nCntParticle].col.g = (float)(rand() % 101) / 100.0f;
			g_aParticle[nCntParticle].col.b = (float)(rand() % 101) / 100.0f;

			g_aParticle[nCntParticle].type = PARTICLETYPE_RAINBOW;

			g_aParticle[nCntParticle].bUse = true;			// �G�t�F�N�g���g�p����Ă����Ԃɂ���
			break;		// ������for���𔲂���
		}
	}
}