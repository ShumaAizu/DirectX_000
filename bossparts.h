//=============================================================================
//
//	�G�̏��� [bossparts.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _BOSSPARTS_H_
#define _BOSSPARTS_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BOSSPARTS		(12)				// �G�̍ő吔
#define BOSSPARTS_SIZEX		(32.0f)				// �G�̃T�C�YX
#define BOSSPARTS_SIZEY		(32.0f)				// �G�̃T�C�YY

//*****************************************************************************
// �G�̏��
//*****************************************************************************
typedef enum
{
	BOSSPARTSSTATE_NORMAL = 0,		// �ʏ���
	BOSSPARTSSTATE_APPEAR,			// �o��
	BOSSPARTSSTATE_WAIT,			// �o���҂�
	BOSSPARTSSTATE_DAMAGE,			// �_���[�W���
	BOSSPARTSSTATE_MAX
}BOSSPARTSSTATE;

//*****************************************************************************
// �G�̎��
//*****************************************************************************
typedef enum
{
	BOSSPARTSTYPE_WING = 0,		// ��
	BOSSPARTSTYPE_HAND,			// ��
	BOSSPARTSTYPE_FLARE,		// ��
	BOSSPARTSTYPE_MAX
}BOSSPARTSTYPE;

//*****************************************************************************
// �G�\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	BOSSPARTSSTATE state;			// ���
	BOSSPARTSTYPE type;				// ���
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
}BossParts;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBossParts(void);
void UninitBossParts(void);
void UpdateBossParts(void);
void DrawBossParts(void);
void SetBossParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore);
BossParts* GetBossParts(void);
void HitBossParts(int nCntBossParts, int nDamage);
void ResetBossParts(BossParts* pBossParts);

#endif