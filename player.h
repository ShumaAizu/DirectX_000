//=============================================================================
//
//	�v���C���[���� [player.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_SIZE			(40.0f)		// �v���C���[�̃T�C�Y
#define PLAYER_SPEED		(0.5f)		// �v���C���[�̈ړ���
#define PLAYER_INIT_POSX		(WARLD_WIDTH / 2)			// �����ʒuX
#define PLAYER_INIT_POSY		(WARLD_HEIGHT / 2)			// �����ʒuY

//*****************************************************************************0
// �v���C���[�̏��
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,				// �o��
	PLAYERSTATE_NORMAL,					// �ʏ�
	PLAYERSTATE_DAMAGE,					// �_���[�W
	PLAYERSTATE_WAIT,					// �o���҂�
	PLAYERSTATE_DEATH,					// ���S
	PLAYERSTATE_MAX						// ����
}PLAYERSTATE;

//*****************************************************************************
// �v���C���[�\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXVECTOR3 move;					// �ړ���
	PLAYERSTATE state;					// ���
	float fRadius;						// ���a
	float fSpeed;						// ����
	int nCounterState;					// ��ԃJ�E���^�[
	bool bDisp;							// �\�����
}Player;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);
void CollisionPlayertoEnemy(void);
void CollisionPlayertoBossParts(void);

#endif
