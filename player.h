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
#define PLAYER_SIZEX		(30)		// �v���C���[�̃T�C�YX
#define PLAYER_SIZEY		(30)		// �v���C���[�̃T�C�YY
#define PLAYER_MIN_SIZE		(30)		// �v���C���[�̍ŏ��T�C�Y
#define PLAYER_MOVEX		(1.5f)		// �v���C���[�̈ړ���X
#define PLAYER_MOVEY		(1.5f)		// �v���C���[�̈ړ���Y


//*****************************************************************************
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
	int nLife;							// �̗�
	int nStock;							// �c�@
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

#endif
