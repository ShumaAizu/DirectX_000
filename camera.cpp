//=============================================================================
//
//	�J�������W���� [camera.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "camera.h"
#include "player.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3 g_camerapos = {};		// �J�������W

void InitCamera(void)
{
	g_camerapos = {};
}

void UninitCamera(void)
{
	g_camerapos = {};
}

void UpdateCamera(void)
{
	// �v���C���[�̏����擾
	Player* pPlayer = GetPlayer();

	// �J�����̈ʒu(�v���C���[�̈ʒu)����X�N���[���̌��_���Z�o
	g_camerapos.x = pPlayer->pos.x - SCREEN_WIDTH / 2;
	g_camerapos.y = pPlayer->pos.y - SCREEN_HEIGHT / 2;

	if (g_camerapos.x > WARLD_WIDTH - SCREEN_WIDTH)
	{
		g_camerapos.x = WARLD_WIDTH - SCREEN_WIDTH;
	}

	if (g_camerapos.x < 0)
	{
		g_camerapos.x = 0;
	}

	if (g_camerapos.y > WARLD_HEIGHT - SCREEN_HEIGHT)
	{
		g_camerapos.y = WARLD_HEIGHT - SCREEN_HEIGHT;
	}

	if (g_camerapos.y < 0)
	{
		g_camerapos.y = 0;
	}
}

void DrawCamera(void)
{

}

D3DXVECTOR3* GetCamera(void)
{
	return &g_camerapos;
}