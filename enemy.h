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
#define ENEMY_SIZEX		(25)				// �G�̃T�C�YX
#define ENEMY_SIZEY		(25)				// �G�̃T�C�YY

//*****************************************************************************
// �G�̏��
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// �ʏ���
	ENEMYSTATE__WAIT,			// �o���҂�
	ENEMYSTATE_DAMAGE,			// �_���[�W���
	ENEMYSTATE_WARNING,			// �댯���
	ENEMYSTATE_MAX
}ENEMYSTATE;

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
	ENEMYSTATE state;				// ���
	ENEMYTYPE type;					// ���
	ENEMYATTACKTYPE attacktype;		// �U�����
	int nTimeline;					// �^�C�����C��
	int nLife;						// �̗�
	int nCounterAnim;				// �A�j���[�V�����J�E���^�[
	int nPatternAnim;				// �A�j���[�V����NO.
	int nCounterState;				// ��ԃJ�E���^�[
	int nCounterAttack;				// �U���J�E���^�[
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
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
void ResetEnemy(void);
void DeleteEnemy(void);
bool CheckEnemy(void);
void LoadEnemy(void);

#endif