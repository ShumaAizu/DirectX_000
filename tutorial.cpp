//=============================================================================
//
//	�`���[�g���A����ʏ��� [result.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "tutorial.h"
#include "tutorialmenu.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "wave.h"
#include "bullet.h"
#include "score.h"
#include "camera.h"
#include "powerup.h"
#include "option.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TUTORIAL			(2)			// ���U���g�e�N�X�`���̖���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
TUTORIAL g_tutorial[MAX_TUTORIAL] = {};
int g_nTutorialTimer = 0;

//====================================
//	���U���g�̏���������
//====================================
void InitTutorial(void)
{
	g_nTutorialTimer = 180;
}

//====================================
//	���U���g�̏I������
//====================================
void UninitTutorial(void)
{

}

//====================================
//	���U���g�̕`�揈��
//====================================
void DrawTutorial(void)
{

}

//====================================
//	���U���g�̍X�V����
//====================================
void UpdateTutorial(void)
{
	Player* pPlayer = GetPlayer();

	int* pNumEnemy = GetNumEnemy();		// �G�̐����擾

	int nUseOption = GetUseOption();

	bool bSpeedUp = GetPowerUp();

	int nScore = GetScore();

	TUTORIALEVENT tutorialevent = GetTutorialEvent();

	if ((pPlayer->move.x > 0 || pPlayer->move.y > 0) && tutorialevent == TUTORIALEVENT_MOVE)
	{
		g_nTutorialTimer--;
		if (g_nTutorialTimer <= 0)
		{
			SetTutorialEvent(TUTORIALEVENT_ENEMY);
			g_nTutorialTimer = 60;
		}
	}

	if (*pNumEnemy <= 0 && tutorialevent == TUTORIALEVENT_ENEMY)
	{
		g_nTutorialTimer--;
		if (g_nTutorialTimer <= 0)
		{
			SetTutorialEvent(TUTORIALEVENT_POWERUP);
			g_nTutorialTimer = 60;
		}
	}

	if (tutorialevent == TUTORIALEVENT_POWERUP)
	{
		if (nScore < SPEEDUP_LOWESTLINE)
		{
			AddScore(1000);
		}

		if (bSpeedUp == true)
		{
			g_nTutorialTimer--;
		}

		if (nUseOption > 0 && g_nTutorialTimer <= 0)
		{
			SetTutorialEvent(TUTORIALEVENT_END);
			g_nTutorialTimer = 300;
		}
	}

	if (tutorialevent == TUTORIALEVENT_END)
	{
		g_nTutorialTimer--;
		if (g_nTutorialTimer <= 0)
		{
			SetScore(INIT_SCORE);
			SetGameState(GAMESTATE_NORMAL, 0);
		}
	}
}