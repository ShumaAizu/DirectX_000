//=============================================================================
//
//	�G�t�F�N�g���� [effect.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// �G�t�F�N�g�̏��
//*****************************************************************************
typedef enum
{
	EFFECTSTATE_NORMAL = 0,		// �ʏ�
	EFFECTSTATE_COLLECT,		// �W��
	EFFECTSTATE_MAX
}EFFECTSTATE;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusDecrease, float fAlphaDecrease, int nLife);
void SetCollectParticle(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif
