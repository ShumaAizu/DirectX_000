//=============================================================================
//
//	�p�[�e�B�N������ [particle.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// �p�[�e�B�N���̎��
//*****************************************************************************
typedef enum
{
	PARTICLETYPE_NORMAL = 0,		// �ʏ�
	PARTICLETYPE_RAINBOW,			// ���F
	PARTICLETYPE_MAX
}PARTICLETYPE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, float fStartAngle, float fEndAngle);
void SetRainbowParticle(D3DXVECTOR3 pos, float fRadius, int nLife, float fStartAngle, float fEndAngle);

#endif // _PARTICLE_H_