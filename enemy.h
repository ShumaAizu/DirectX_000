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
	ENEMYSTATE_MAX
}ENEMYSTATE;

//*****************************************************************************
// �G�̈ړ��̏��
//*****************************************************************************
typedef enum
{
	ENEMY_MOVESTATE_WAIT = 0,			// �ҋ@
	ENEMY_MOVESTATE_MOVE,				// �ړ�
	ENEMY_MOVESTATE_CHASE,				// �ǐ�
	ENEMY_MOVESTATE_MAX
}ENEMYMOVESTATE;

//*****************************************************************************
// �G�̎��
//*****************************************************************************
typedef enum
{
	ENEMYTYPE_FLAME = 0,		// ��
	ENEMYTYPE_WATER,			// ��
	ENEMYTYPE_WIND,				// ��
	ENEMYTYPE_EARTH,			// �y
	ENEMYTYPE_BOSS,				// �{�X
	ENEMYTYPE_MAX
}ENEMYTYPE;

//*****************************************************************************
// �G�̍U���̎��
//*****************************************************************************
typedef enum
{
	ENEMY_ATTACKTYPE_AIM = 0,			// ���@�_��
	ENEMY_ATTACKTYPE_HOMING,			// �z�[�~���O
	ENEMY_ATTACKTYPE_SCATTE,			// �΂�܂�
	ENEMY_ATTACKTYPE_MAX
}ENEMYATTACKTYPE;

//*****************************************************************************
// �G�\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	ENEMYSTATE state;				// ���
	ENEMYTYPE type;					// ���
	float fRadius;					// ���a
	float fMove;					// �ړ���
	int nTimeLine;					// �^�C�����C��
	int nScore;						// ���j���l���X�R�A
	int nLife;						// �̗�
	int nCounterAnim;				// �A�j���[�V�����J�E���^�[
	int nPatternAnim;				// �A�j���[�V����NO.
	int nCounterState;				// ��ԃJ�E���^�[
	int nCounterAttack;				// �U���J�E���^�[
	int nUseCounter;				// �g�p�s�\�܂ł̃J�E���^�[
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
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
void ResetEnemy(Enemy* pEnemy);
void DeleteEnemy(void);
bool CheckEnemy(void);
int* GetNumEnemy(void);
Enemy* GetBOSSEnemy(void);

#endif