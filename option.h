//=============================================================================
//
//	�I�v�V�����̏��� [option.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OPTION_H_
#define _OPTION_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_OPTION			(1)

//*****************************************************************************
// �I�v�V�����̏��
//*****************************************************************************
typedef enum
{
	OPTIONSTATE_NORMAL = 0,				// �ʏ�
	OPTIONSTATE_WAIT,					// �o���҂�
	OPTIONSTATE_COOLTIME,				// �@�\�s�\
	OPTIONSTATE_STAY,					// �ҋ@
	OPTIONSTATE_MAX
}OPTIONSTATE;
//*****************************************************************************
// �I�v�V�����\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 distance;	// �v���C���[�Ƃ̋���
	float fAngle;			// �v���C���[�Ƃ̊p�x
	OPTIONSTATE state;		// ���
	int nCounterState;		// ��ԃJ�E���^�[
	bool bUse;				// �g�p���Ă��邩�ǂ���
	bool bDisp;				// �\�����
}Option;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetOption(D3DXVECTOR3 pos, D3DXVECTOR3 Distance);
Option *GetOption(void);
void HitOption(int nCntOption);

#endif
