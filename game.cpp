//=============================================================================
//
//	�Q�[����ʏ��� [game.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "game.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "option.h"
#include "time.h"
#include "life.h"
#include "stock.h"
#include "pause.h"
#include "camera.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMEEND g_gameend = GAMEEND_MAX;			// �I�������̏��
bool g_bGameend = false;					// �I�����邩�ǂ���
bool g_bPause = false;						// �|�[�Y�����ǂ���

//========================================
//	�Q�[����ʂ̏���������
//========================================
void InitGame(void)
{
	// �J�����̏���������
	InitCamera();

	// �w�i�̏���������
	InitBg();

	// �v���C���[�̏���������
	InitPlayer();

	// �e�̏���������
	InitBullet();

	// �����̏���������
	InitExplosion();

	// �G�̏���������
	InitEnemy();

	// �G�̃Z�b�g����
	srand((unsigned)timeGetTime());

	// �X�R�A�̏���������
	InitScore();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �p�[�e�B�N���̏���������
	InitParticle();

	// �I�v�V�����̏���������
	InitOption();

	// �������Ԃ̏�����
	InitTime();

	// ���C�t�̏�����
	InitLife();

	// �c�@�̏�����
	InitStock();

	// �|�[�Y���j���[�̏�����
	InitPause();

	// �I��������������
	g_bGameend = false;

	// �|�[�Y��Ԃ̏�����
	g_bPause = false;

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//========================================
//	�Q�[����ʂ̏I������
//========================================
void UninitGame(void)
{
	// �J�����̏I������
	UninitCamera();

	// �w�i�̏I������
	UninitBg();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitBullet();

	//�����̏I������
	UninitExplosion();

	// �G�̏I������
	UninitEnemy();

	// �X�R�A�̏I������
	UninitScore();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �I�v�V�����̏I������
	UninitOption();

	// �������Ԃ̏I������
	UninitTime();

	// ���C�t�̏I������
	UninitLife();

	// �c�@�̏I������
	UninitStock();

	// �|�[�Y���j���[�̏I������
	UninitPause();

	// �T�E���h���~�߂�
	StopSound();
}

//========================================
//	�Q�[����ʂ̍X�V����
//========================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
	{// �|�[�Y�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;		// �|�[�Y��Ԃ�؂�ւ���
	}

	if (g_bPause == true)
	{// �|�[�Y���Ȃ�
		// �|�[�Y�̍X�V����
		UpdatePause();
	}
	else
	{
		// �J�����̍X�V����
		UpdateCamera();

		// �w�i�̍X�V����
		UpdateBg();

		// �v���C���[�̍X�V����
		UpdatePlayer();


		// �e�̍X�V����
		UpdateBullet();

		// �����̍X�V����
		UpdateExplosion();

		// �G�̍X�V����
		UpdateEnemy();

		// �X�R�A�̍X�V����
		UpdateScore();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �p�[�e�B�N���̍X�V����
		UpdateParticle();

		// �I�v�V�����̍X�V����
		UpdateOption();

		// �������Ԃ̍X�V����
		UpdateTime();

		// ���C�t�̍X�V����
		UpdateLife();

		// �c�@�̍X�V����
		UpdateStock();

		// �v���C���[�̏����擾
		Player* pPlayer = GetPlayer();

		static int nGameEndCounter = 0;

		if (CheckEnemy() == false)
		{
			g_bGameend = true;
			g_gameend = GAMEEND_CLEAR;
		}
		else if (pPlayer->state == PLAYERSTATE_DEATH || CheckTime() == false)
		{
			g_bGameend = true;
			g_gameend = GAMEEND_GAMEOVER;
		}

		// �Q�[���I������
		if (g_bGameend == true)
		{
			nGameEndCounter++;
			if (nGameEndCounter % 60 == 0)
			{
				nGameEndCounter = 0;
				// ���[�h�ݒ�(���U���g��ʂɈڍs)
				SetFade(MODE_RESULT);
			}
		}
	}
}

//========================================
//	�Q�[����ʂ̕`�揈��
//========================================
void DrawGame(void)
{
	// �J�����̕`�揈��
	DrawCamera();

	// �w�i�̕`�揈��
	DrawBg();

	// �e�̕`�揈��
	DrawBullet();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �����̕`�揈��
	DrawExplosion();

	// �G�̕`�揈��
	DrawEnemy();

	// �X�R�A�̕`�揈��
	DrawScore();

	// �I�v�V�����̕`�揈��
	DrawOption();

	// �������Ԃ̕`�揈��
	DrawTime();

	// ���C�t�̕`�揈��
	DrawLife();

	// �c�@�̕`�揈��
	DrawStock();

	if (g_bPause == true)
	{// �|�[�Y���Ȃ�
		// �|�[�Y���j���[�̕`�揈��
		DrawPause();
	}
}

//========================================
//	�Q�[���I�������̎擾
//========================================
GAMEEND GetGameEnd(void)
{
	return g_gameend;
}

//========================================
//	�|�[�Y�̗L�������ݒ�
//========================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
