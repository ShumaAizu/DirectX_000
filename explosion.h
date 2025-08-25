//=============================================================================
//
//	�������� [explosion.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EXPLOSION_SIZEX		(30)		//�����̃T�C�YX
#define EXPLOSION_SIZEY		(30)		//�����̃T�C�YY

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif
