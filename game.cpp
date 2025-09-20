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
#include "frame.h"
#include "powerup.h"
#include "wave.h"
#include "marker.h"
#include "radar.h"
#include "ranking.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// �Q�[���̏��
GAMEEND g_gameend = GAMEEND_MAX;			// �I�������̏��
int g_nCounterGameState = 0;				// ��ԊǗ��J�E���^�[
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

	// �E�F�[�u�̏���������
	InitWave();

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

	// �}�[�J�[�̏�����
	InitMarker();

	// �t���[���̏�����
	InitFrame();

	// �|�[�Y���j���[�̏�����
	InitPause();

	// �p���[�A�b�v�̏�����
	InitPowerup();

	// ���[�_�[�̏�����
	InitRadar();

	g_gameState = GAMESTATE_NORMAL;

	// �I��������������
	g_bGameend = false;

	// �|�[�Y��Ԃ̏�����
	g_bPause = false;

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM_GAME000);
}

//========================================
//	�Q�[����ʂ̏I������
//========================================
void UninitGame(void)
{
	// �E�F�[�u�̏I������
	UninitWave();

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

	UninitPowerup();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �I�v�V�����̏I������
	UninitOption();

	// �������Ԃ̏I������
	UninitTime();

	// �}�[�J�[�̏I������
	UninitMarker();

	// ���[�_�[�̏I������
	UninitRadar();

	// �t���[���̏I������
	UninitFrame();

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

	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && GetFade() != FADE_OUT)
	{// �|�[�Y�L�[�������ꂽ
		PlaySound(SOUND_LABEL_SE_PAUSE000);
		g_bPause = g_bPause ? false : true;		// �|�[�Y��Ԃ�؂�ւ���
		SetPauseMenu(PAUSE_MENU_CONTINUE);
	}

	if (g_bPause == true)
	{// �|�[�Y���Ȃ�
		// �|�[�Y�̍X�V����
		UpdatePause();
	}
	else if(GetFade() != FADE_OUT)
	{

		// �X�R�A�̍X�V����
		UpdateScore();

		// �E�F�[�u�̍X�V����
		UpdateWave();

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

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �p�[�e�B�N���̍X�V����
		UpdateParticle();

		// �I�v�V�����̍X�V����
		UpdateOption();

		// �������Ԃ̍X�V����
		UpdateTime();

		// �p���[�A�b�v�̍X�V����
		UpdatePowerup();

		// �}�[�J�[�̍X�V����
		UpdateMarker();

		// ���[�_�[�̍X�V����
		UpdateRadar();

		// �t���[���̍X�V����
		UpdateFrame();

		switch (g_gameState)
		{
		case GAMESTATE_END:
			g_nCounterGameState--;
			if (g_nCounterGameState <= 0)
			{
				// ���[�h�ݒ�(���U���g��ʂɈڍs)
				g_gameend = GAMEEND_CLEAR;
				if (GetScore() > 0)
				{
					AddScore((GetTime() * 100));
				}
				SetRanking(GetScore());
				SetFade(MODE_RANKING, 0.025f, 0.025f);
			}
			break;
		}

	}
}

//========================================
//	�Q�[����ʂ̕`�揈��
//========================================
void DrawGame(void)
{
	// �E�F�[�u�̕`�揈��
	DrawWave();

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

	// �I�v�V�����̕`�揈��
	DrawOption();

	// �t���[���̕`�揈��
	DrawFrame();

	// �X�R�A�̕`�揈��
	DrawScore();

	DrawPowerup();

	// �������Ԃ̕`�揈��
	DrawTime();

	// �}�[�J�[�̕`�揈��
	DrawMarker();

	//// ���[�_�[�̕`�揈��
	//DrawRadar();

	if (g_bPause == true)
	{// �|�[�Y���Ȃ�
		// �|�[�Y���j���[�̕`�揈��
		DrawPause();
	}
}

//========================================
//	�Q�[���̏�Ԑݒ�
//========================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

//========================================
//	�Q�[���̏�Ԃ̎擾
//========================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
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
