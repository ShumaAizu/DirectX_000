////=============================================================================
////
////	�e�̏��� [bullet.h]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#ifndef _ENEMYBULLET_H_
//#define _ENEMYBULLET_H_
//
//#include "main.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define ENEMYBULLET_SIZEX		(7.5f)			// �e�̃T�C�YX
//#define ENEMYBULLET_SIZEY		(7.5f)			// �e�̃T�C�YY
//#define ENEMYBULLET_MOVE		(15.0f)			// �e�̈ړ���
//
////*****************************************************************************
//// �e�̎�ށi���ޖځj
////*****************************************************************************
//typedef enum
//{
//	SHOTTYPE_NORMAL = 0,			// �ʏ�
//	SHOTTYPE_AIM,					// ���@�_��
//	SHOTTYPE_HOMING,				// �ǔ�
//	SHOTTYPE_MAX					// ����
//}SHOTTYPE;
//
////*****************************************************************************
//// �e�̍\���̒�`
////*****************************************************************************
//typedef struct
//{
//	D3DXVECTOR3 pos;					// �ʒu
//	D3DXVECTOR3 move;					// �ړ���
//	D3DXVECTOR3 rot;					// ����
//	BULLETTYPE type;					// �e�̎��
//	SHOTTYPE shottype;					// ���ޖڂ̒e�̎��
//	float fLength;						// �Ίp���̒���
//	float fAngle;						// �Ίp���̊p�x
//	int target;							// �z�[�~���O�̖ڕW
//	int nLife;							// ����
//	int nHomingTimer;					// �؂�ւ��p�^�C�}�[
//	bool bUse;							// �g�p���Ă��邩�ǂ���
//}Bullet;
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//void InitBullet(void);
//void UninitBullet(void);
//void UpdateBullet(void);
//void DrawBullet(void);
//void SetPlayerBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, SHOTTYPE shottype);
//void SetEnemyBullet(D3DXVECTOR3 pos, float fmove, int nLife, BULLETTYPE type, SHOTTYPE shottype, float fAngleE_P);
//void SetHomingBullet(D3DXVECTOR3 pos, BULLETTYPE type, float fmove, float fAngle, int nLife, int nHomingTimer);
//void CollisionEnemy(Bullet *pBullet);
//void CollisionPlayer(Bullet* pBullet);
//void CollisionOption(Bullet* pBullet);
//
//#endif
