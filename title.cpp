//=============================================================================
//
//	�^�C�g����ʏ��� [title.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlebg.h"
#include "titlemenu.h"
#include "selectlevel.h"
#include "titlelogo.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TITLE_UI		(4)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_UI] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
TITLESTATE g_titleState = TITLESTATE_OP;							// �^�C�g���̏��
int g_nCounterTitleState = 0;										// ��ԊǗ��J�E���^�[

//====================================
//	�^�C�g���̏���������
//====================================
void InitTitle(void)
{

	g_titleState = TITLESTATE_OP;

	// �^�C�g���w�i�̏�����
	InitTitleBG();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �^�C�g�����S�̏���������
	InitTitleLogo();

	// �^�C�g�����j���[�̏�����
	InitTitleMenu();

	// ��Փx�I���̏���������
	InitSelectLevel();

	PlaySound(SOUND_LABEL_BGM_TITLE000);
}

//====================================
//	�^�C�g���̏I������
//====================================
void UninitTitle(void)
{
	// �^�C�g���w�i�̏I������
	UninitTitleBG();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �^�C�g�����S�̏I������
	UninitTitleLogo();

	// �^�C�g�����j���[�̏I������
	UninitTitleMenu();

	// ��Փx�I���̏I������
	UninitSelectLevel();

	// �T�E���h���~�߂�
	StopSound();
}

//====================================
//	�^�C�g���̕`�揈��
//====================================
void DrawTitle(void)
{
	// �^�C�g���w�i�̕`�揈��
	DrawTitleBG();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �^�C�g�����S�̕`�揈��
	DrawTitleLogo();

	if (g_titleState == TITLESTATE_MENU)
	{
		// �^�C�g�����j���[�̕`�揈��
		DrawTitleMenu();
	}
	else if (g_titleState == TITLESTATE_SELECTLEVEL)
	{
		DrawSelectLevel();
	}
}

//====================================
//	�^�C�g���̍X�V����
//====================================
void UpdateTitle(void)
{

	// �^�C�g���w�i�̍X�V����
	UpdateTitleBG();

	// �G�t�F�N�g�̍X�V����
	UpdateEffect();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	// �^�C�g�����S�̍X�V����
	UpdateTitleLogo();

	if (g_titleState == TITLESTATE_MENU)
	{
		// �^�C�g�����j���[�̍X�V����
		UpdateTitleMenu();
	}
	else if (g_titleState == TITLESTATE_SELECTLEVEL)
	{
		UpdateSelectLevel();
	}
}

//========================================
//	�^�C�g���̏�Ԑݒ�
//========================================
void SetTitleState(TITLESTATE titlestate)
{
	g_titleState = titlestate;
}

//========================================
//	�^�C�g���̏�Ԏ擾
//========================================
TITLESTATE GetTitleState(void)
{
	return g_titleState;
}