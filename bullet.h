//=============================================================================
//
//	�e�̏��� [bullet.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_SIZEX		(15)			// �e�̃T�C�YX
#define BULLET_SIZEY		(15)			// �e�̃T�C�YY
#define BULLET_MOVE			(15.0f)			// �e�̈ړ���


//*****************************************************************************
// �e�̎��
//*****************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,				// �v���C���[�̒e
	BULLETTYPE_ENEMY,					// �G�̒e
	BULLETTYPE_MAX						// ����
}BULLETTYPE;

//*****************************************************************************
// �e�̍\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 move;					// �ړ���
	D3DXVECTOR3 rot;					// ����
	BULLETTYPE type;					// �e�̎��
	float fLength;						// �Ίp���̒���
	float fAngle;						// �Ίp���̊p�x
	int nLife;							// ����
	bool bUse;							// �g�p���Ă��邩�ǂ���
}Bullet;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetPlayerBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type);
void SetEnemyBullet(D3DXVECTOR3 pos, float fmove, int nLife, BULLETTYPE type, float fAngleE_P);
void CollisionEnemy(Bullet *pBullet);
void CollisionPlayer(Bullet* pBullet);
void CollisionOption(Bullet* pBullet);

#endif
