//=============================================================================
//
//	�G�̏��� [enemy.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY		(128)				// �G�̍ő吔
#define ENEMY_SIZEX		(32.0f)				// �G�̃T�C�YX
#define ENEMY_SIZEY		(32.0f)				// �G�̃T�C�YY

//*****************************************************************************
// �G�̏��
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// �ʏ���
	ENEMYSTATE_APPEAR,			// �o��
	ENEMYSTATE_WAIT,			// �o���҂�
	ENEMYSTATE_DAMAGE,			// �_���[�W���
	ENEMYSTATE_WARNING,			// �댯���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*****************************************************************************
// �G�̈ړ��̏��
//*****************************************************************************
typedef enum
{
	ENEMY_MOVESTATE_WAIT = 0,			// �ҋ@
	ENEMY_MOVESTATE_MOVE,				// �ړ�
	ENEMY_MOVESTATE_MAX
}ENEMYMOVESTATE;

//*****************************************************************************
// �G�̎��
//*****************************************************************************
typedef enum
{
	ENEMYTYPE_NORMAL = 0,		// �ʏ�
	ENEMYTYPE_MIDDLEBOSS,		// ���{�X
	ENEMYTYPE_BOSS,				// �{�X
	ENEMYTYPE_MAX
}ENEMYTYPE;

//*****************************************************************************
// �G�̍U���̎��
//*****************************************************************************
typedef enum
{
	ENEMY_ATTACKTYPE_AIM = 0,
	ENEMY_ATTACKTYPE_HOMING,
	ENEMY_ATTACKTYPE_SCATTE,
	ENEMY_ATTACKTYPE_MAX
}ENEMYATTACKTYPE;

//*****************************************************************************
// �G�\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ����
	ENEMYSTATE state;				// ���
	ENEMYTYPE type;					// ���
	ENEMYATTACKTYPE attacktype;		// �U�����
	int nTimeLine;					// �^�C�����C��
	int nLife;						// �̗�
	int nCounterAnim;				// �A�j���[�V�����J�E���^�[
	int nPatternAnim;				// �A�j���[�V����NO.
	int nCounterState;				// ��ԃJ�E���^�[
	int nCounterAttack;				// �U���J�E���^�[
	int nCounterMove;				// �ړ���ԃJ�E���^�[
	bool bDisp;						// �\�����
	bool bBlinking;					// �_�ŏ��
	bool bUse;						// �g�p���Ă��邩�ǂ���
}Enemy;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, int nLife, int nTimeLine);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
void ResetEnemy(void);
void DeleteEnemy(void);
bool CheckEnemy(void);
int* GetNumEnemy(void);

#endif