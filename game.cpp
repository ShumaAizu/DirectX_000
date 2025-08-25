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
#include "option.h"
#include "time.h"
#include "life.h"
#include "stock.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMEEND g_gameend = GAMEEND_MAX;			// �I�������̏��
bool g_bGameend = false;

//========================================
//	�Q�[����ʂ̏���������
//========================================
void InitGame(void)
{
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

	float nCntX = 390.0f, nCntY = 100.0f;
	int nCnt1, nCnt2;

	for (nCnt1 = 0; nCnt1 < 6; nCnt1++, nCntX += 100.0f)
	{
		for (nCnt2 = 0, nCntY = 100.0f; nCnt2 < 5; nCnt2++, nCntY += 50.0f)
		{
			SetEnemy(D3DXVECTOR3(nCntX, nCntY, 0.0f), rand() % 5, rand() % 5 + 2);
		}
	}

	// �X�R�A�̏���������
	InitScore();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �I�v�V�����̏���������
	InitOption();

	// �������Ԃ̏�����
	InitTime();

	// ���C�t�̏�����
	InitLife();

	// �c�@�̏�����
	InitStock();

	// �I��������������
	g_bGameend = false;

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//========================================
//	�Q�[����ʂ̏I������
//========================================
void UninitGame(void)
{
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

	// �I�v�V�����̏I������
	UninitOption();

	// �������Ԃ̏I������
	UninitTime();

	// ���C�t�̏I������
	UninitLife();

	// �c�@�̏I������
	UninitStock();

	// �T�E���h���~�߂�
	StopSound();
}

//========================================
//	�Q�[����ʂ̍X�V����
//========================================
void UpdateGame(void)
{
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

//========================================
//	�Q�[����ʂ̕`�揈��
//========================================
void DrawGame(void)
{
	// �w�i�̕`�揈��
	DrawBg();

	// �e�̕`�揈��
	DrawBullet();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

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
}

//========================================
//	�Q�[���I�������̎擾
//========================================
GAMEEND GetGameEnd(void)
{
	return g_gameend;
}
