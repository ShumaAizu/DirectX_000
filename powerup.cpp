//=============================================================================
//
//	�p���[�A�b�v���� [powerup.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "powerup.h"
#include "input.h"
#include "effect.h"
#include "option.h"
#include "player.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POWERUP_LOWESTLINE		(2500)		// �p���[�A�b�v���邽�߂ɕK�v�ȃX�R�A�̉���
#define OPTION_COST				(1500)		// �I�v�V�����𐶐����邽�߂ɕK�v�ȃX�R�A
#define SPEEDUP_COST			(1000)		// ���x���グ�邽�߂ɕK�v�ȃX�R�A

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//====================================
//	�p���[�A�b�v�̏���������
//====================================
void InitPowerup(void)
{

}

//====================================
//	�p���[�A�b�v�̏I������
//====================================
void UninitPowerup(void)
{

}

//====================================
//	�p���[�A�b�v�̍X�V����
//====================================
void UpdatePowerup(void)
{
	D3DXVECTOR3* pOptionStandardRot = GetStandardRot();
	Player* pPlayer = GetPlayer();
	int nUseOption = GetUseOption();

	if (GetJoypadTrigger(JOYKEY_X) == true || GetKeyboardTrigger(DIK_O) == true)
	{
		if (GetScore() >= POWERUP_LOWESTLINE && nUseOption < MAX_OPTION)
		{
			SetOption(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, 0.0f), 64.0f, (pOptionStandardRot->z) * D3DX_PI);
			SubScore(OPTION_COST);
		}
	}

	if (GetJoypadTrigger(JOYKEY_B) == true || GetKeyboardTrigger(DIK_L) == true)
	{
		if (GetScore() >= POWERUP_LOWESTLINE)
		{
			pPlayer->fSpeed += 0.1f;
			SubScore(SPEEDUP_COST);
		}
	}

}

//====================================
//	�p���[�A�b�v�̕`�揈��
//====================================
void DrawPowerup(void)
{

}