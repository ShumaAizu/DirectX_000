//=============================================================================
//
//	�V���[�e�B���O�Q�[���A���t�@�� [main.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MAIN_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include <windows.h>
#include <cstdio>
#include "d3dx9.h"								// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION		(0x0800)		// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"								// ���͏����ɕK�v
#include "xaudio2.h"							// �T�E���h�����ɕK�v
#include "xinput.h"								// �W���C�p�b�h�����ɕK�v

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")				// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")				// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")				// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")				// �V�X�e���̎����擾�ɕK�v
#pragma comment(lib, "dinput8.lib")				// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")				// �W���C�p�b�h�����ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH		(1280.0f)											// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(720.0f)											// �E�C���h�E�̍���
#define SCREEN_RADIUS		(640.0f)
#define WARLD_WIDTH			(3000.0f)											// ���[���h���WX
#define WARLD_HEIGHT		(3000.0f)											// ���[���h���WY
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// ���_�t�H�[�}�b�g(2D)
#define SQRTF(X, Y)			(sqrtf(X * X + Y * Y) * 0.5f)						// �Ίp���̒����Z�o

//*****************************************************************************
// ���[�h�̎��
//*****************************************************************************
typedef enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_GAME,				// �Q�[�����
	MODE_TUTORIAL,			// �`���[�g���A�����
	MODE_RESULT,			// ���U���g���
	MODE_RANKING,			// �����L���O���
	MODE_PAUSE,				// �|�[�Y���
	MODE_MAX				// ����
}MODE;

//*****************************************************************************
// ���_���̍\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���_���W(x, y, z)
	float rhw;				// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;			// ���_�J���[(r, g, b, a)
	D3DXVECTOR2 tex;		// �e�N�X�`�����W(x, y)
}VERTEX_2D;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif