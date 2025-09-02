//=============================================================================
//
//	�v���C���[���� [player.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "option.h"
#include "life.h"
#include "stock.h"
#include <stdlib.h>
#include "particle.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INIT_POSX		(640.0f)			// �����ʒuX
#define INIT_POSY		(500.0f)			// �����ʒuY

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;				// �v���C���[�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		// �v���C���[�̒��_�o�b�t�@�ւ̃|�C���^
Player g_player;										// �v���C���[�̏��
int g_nCounterAnimPlayer;								// �A�j���[�V�����J�E���^�[
int g_nPatternAnimPlayer;								// �A�j���[�V�����p�^�[��No.

//=============================================================================
//	�v���C���[����������
//=============================================================================
void InitPlayer(void)
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\player000.png",
		&g_pTexturePlayer);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	g_player.pos = D3DXVECTOR3(INIT_POSX, INIT_POSY, 0.0f);	// �ʒu
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����������������
	g_player.fRadius = PLAYER_SIZE;							// �v���C���[�̑傫���̏�����
	g_player.nLife = MAX_LIFE;								// ���C�t�̏�����
	g_player.nStock = MAX_STOCK;							// �c�@�̏�����
	g_player.state = PLAYERSTATE_NORMAL;					// ��Ԃ̏�����
	g_player.bDisp = true;									// �\����Ԃ̏�����

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void * *)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_player.pos.x - g_player.fRadius;
	pVtx[0].pos.y = g_player.pos.y - g_player.fRadius;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + g_player.fRadius;
	pVtx[1].pos.y = g_player.pos.y - g_player.fRadius;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - g_player.fRadius;
	pVtx[2].pos.y = g_player.pos.y + g_player.fRadius;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + g_player.fRadius;
	pVtx[3].pos.y = g_player.pos.y + g_player.fRadius;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
//	�v���C���[�̏I������
//=============================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================
//	�v���C���[�̕`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_player.bDisp == true)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
							   0,						//�`�悷��v���~�e�B�u��
							   2);						//�`�悷��ŏ��̒��_�C���f�b�N�X
	}
}

//=============================================================================
//	�v���C���[�̍X�V����
//=============================================================================
void UpdatePlayer(void)
{

	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.nCounterState--;		// ��Ԏ������Ԃ����炷

		if (g_player.nCounterState % 3 == 0)
		{
			g_player.bDisp = g_player.bDisp ? false : true;
		}

		if (g_player.nCounterState <= 0 && g_player.bDisp == true)
		{
			g_player.state = PLAYERSTATE_NORMAL;		// ��Ԃ��m�[�}����
		}
		break;

	case PLAYERSTATE_WAIT:
		g_player.nCounterState--;		// ��Ԏ������Ԃ����炷
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_APPEAR;		// ��Ԃ��o����
			g_player.nLife = MAX_LIFE;					// ���C�t������
			g_player.nCounterState = 60;				// ��Ԏ������ԃZ�b�g
			g_player.bDisp = true;						// �\����Ԃ�
		}
		return;

	case PLAYERSTATE_NORMAL:
		// �p�[�e�B�N���ݒ�
		SetRainbowParticle(g_player.pos, 3000.0f, 15);
		CollisionPlayertoEnemy();
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;		// ��Ԏ������Ԃ����炷
		if (g_player.nCounterState <= 0)
		{// ��Ԏ������Ԃ��Ȃ��Ȃ�����
			g_player.state = PLAYERSTATE_APPEAR;		// ������
			g_player.nCounterState = 30;				// ��Ԏ������ԃZ�b�g
			// ���_���W�̍X�V
			VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

			// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_DEATH:
#ifdef _DEBUG

		// �e�X�g�p�Z�b�g�G�l�~�[
		if (GetKeyboardTrigger(DIK_L) == true)
		{
			// �G�̃��Z�b�g
			ResetEnemy();

			float nCntX = 390.0f, nCntY = 100.0f;
			int nCnt1, nCnt2;

			for (nCnt1 = 0; nCnt1 < 6; nCnt1++, nCntX += 100.0f)
			{
				for (nCnt2 = 0, nCntY = 50.0f; nCnt2 < 5; nCnt2++, nCntY += 50.0f)
				{
					SetEnemy(D3DXVECTOR3(nCntX, nCntY, 0.0f), rand() % 4, rand() % 5 + 1);
				}
			}

			g_player.state = PLAYERSTATE_NORMAL;		// ��Ԃ̏�����
			g_player.nLife = 5;							// ���C�t�̏�����
			g_player.bDisp = true;						// �\����Ԃ̏�����

			// ���_���W�̍X�V
			VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

			// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffPlayer->Unlock();
		}
#endif
		return;
	}

	// �I�v�V�����̕W�������擾
	D3DXVECTOR3* pOptionStandardRot = GetStandardRot();

	if (GetKeyboardRepeat(DIK_SPACE) == true)
	{//SPACE�L�[�������ꂽ

		Option* pOption = GetOption();

		PlaySound(SOUND_LABEL_SE_SHOT);

		//�e�̐ݒ�
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(0.0f, -BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL);


		//�e�̐ݒ�
		for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
		{
			if ((pOption + nCntOption)->bUse == true)
			{
				SetEnemyBullet((pOption + nCntOption)->pos, 15.0f, 50, BULLETTYPE_PLAYER, SHOTTYPE_HOMING, (((pOption + nCntOption)->rot.z + pOptionStandardRot->z)* D3DX_PI));
			}
		}
	}

	if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[�������ꂽ
		if (GetKeyboardPress(DIK_W) == true)
		{//����W�L�[�������ꂽ
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//����S�L�[�������ꂽ
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x -= PLAYER_MOVEX;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[�������ꂽ
		if (GetKeyboardPress(DIK_W) == true)
		{//����W�L�[�������ꂽ
			g_player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//����S�L�[�������ꂽ
			g_player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x += PLAYER_MOVEX;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[�������ꂽ
		g_player.move.y -= PLAYER_MOVEY;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//S�L�[�������ꂽ
		g_player.move.y += PLAYER_MOVEY;
	}

	if (GetJoypadRepeat(JOYKEY_A) == true)
	{//SPACE�L�[�������ꂽ

		Option* pOption = GetOption();

		PlaySound(SOUND_LABEL_SE_SHOT);

		//�e�̐ݒ�
		SetPlayerBullet(g_player.pos, D3DXVECTOR3(0.0f, -BULLET_MOVE, 0.0f), 50, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL);


		//�e�̐ݒ�
		for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
		{
			if ((pOption + nCntOption)->bUse == true)
			{
				SetEnemyBullet((pOption + nCntOption)->pos, 15.0f, 50, BULLETTYPE_PLAYER, SHOTTYPE_HOMING, (((pOption + nCntOption)->rot.z + pOptionStandardRot->z)* D3DX_PI));
			}
		}
	}

	// �`���[�W�V���b�g
	if (GetJoypadRepeat(JOYKEY_B) == true)
	{
		static int ChargeCounter = 0;

		ChargeCounter++;

		if (ChargeCounter > 180)
		{
			SetParticle(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 75.0f, 50);
		}
		else
		{
			SetCollectParticle(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	// �I�v�V��������
	if (GetJoypadTrigger(JOYKEY_X) == true || GetKeyboardTrigger(DIK_F1) == true)
	{
		SetOption(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, 0.0f), 64.0f, (pOptionStandardRot->z) * D3DX_PI);
	}

	// �W���C�p�b�h�̏�Ԃ��擾
	XINPUT_STATE* pJoyState = GetJoypadState();

	// �X�e�B�b�N����
	float fThumbLX = pJoyState->Gamepad.sThumbLX;	// L�X�e�B�b�N��X�̒l
	float fThumbLY = pJoyState->Gamepad.sThumbLY;	// L�X�e�B�b�N��Y�̒l

	float fJoypadMove;		// �x�N�g��

	// �x�N�g���̒������Z�o
	fJoypadMove = sqrtf(fThumbLX * fThumbLX + fThumbLY * fThumbLY);

	if (fJoypadMove > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// �����f�b�h�]�[���ȓ��Ȃ�
		// ���K��
		fThumbLX = (fThumbLX) / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		fThumbLY = (fThumbLY) / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else
	{// �f�b�h�]�[���O�Ȃ�
		// ������
		fJoypadMove = 0.0f;
		fThumbLX = 0.0f;
		fThumbLY = 0.0f;
	}

	// �ړ��ʂ̍X�V
	g_player.move.x += fThumbLX * 0.5f;
	g_player.move.y -= fThumbLY * 0.5f;

	if (GetJoypadPress(JOYKEY_LEFT) == true)
	{//A�L�[�������ꂽ
		if (GetJoypadPress(JOYKEY_UP) == true)
		{//����W�L�[�������ꂽ
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetJoypadPress(JOYKEY_DOWN) == true)
		{//����S�L�[�������ꂽ
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x -= PLAYER_MOVEX;
		}
	}
	else if (GetJoypadPress(JOYKEY_RIGHT) == true)
	{//D�L�[�������ꂽ
		if (GetJoypadPress(JOYKEY_UP) == true)
		{//����W�L�[�������ꂽ
			g_player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_MOVEX;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_MOVEY;
		}
		else if (GetJoypadPress(JOYKEY_DOWN) == true)
		{//����S�L�[�������ꂽ
			g_player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_MOVEX;
			g_player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_MOVEY;
		}
		else
		{
			g_player.move.x += PLAYER_MOVEX;
		}
	}
	else if (GetJoypadPress(JOYKEY_UP) == true)
	{//W�L�[�������ꂽ
		g_player.move.y -= PLAYER_MOVEY;
	}
	else if (GetJoypadPress(JOYKEY_DOWN) == true)
	{//S�L�[�������ꂽ
		g_player.move.y += PLAYER_MOVEY;
	}

	// �e�X�g�p�G����
	if (GetKeyboardRepeat(DIK_DELETE) == true)
	{
		DeleteEnemy();
	}

	// �ʒu���X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	// �������X�V
	g_player.move.x += (0.0f - g_player.move.x) * 0.1f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.1f;

	//// �[�ɍs�����甽�΂Ɉړ�����
	//if (g_player.pos.x + PLAYER_SIZEX < 0)
	//{
	//	g_player.pos.x = SCREEN_WIDTH;
	//}
	//if (g_player.pos.x - PLAYER_SIZEX > SCREEN_WIDTH)
	//{
	//	g_player.pos.x = 0;
	//}

	//if (g_player.pos.y + PLAYER_SIZEY < 0)
	//{
	//	g_player.pos.y = SCREEN_HEIGHT;
	//}
	//if (g_player.pos.y - PLAYER_SIZEY > SCREEN_HEIGHT)
	//{
	//	g_player.pos.y = 0;
	//}

	// �e�N�X�`�����X�V
	g_nCounterAnimPlayer++;
	if ((g_nCounterAnimPlayer % 5) == 0)
	{
		g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % 10;
	}

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_player.pos.x - g_player.fRadius - pCameraPos->x;
	pVtx[0].pos.y = g_player.pos.y - g_player.fRadius - pCameraPos->y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + g_player.fRadius - pCameraPos->x;
	pVtx[1].pos.y = g_player.pos.y - g_player.fRadius - pCameraPos->y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - g_player.fRadius - pCameraPos->x;
	pVtx[2].pos.y = g_player.pos.y + g_player.fRadius - pCameraPos->y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + g_player.fRadius - pCameraPos->x;
	pVtx[3].pos.y = g_player.pos.y + g_player.fRadius - pCameraPos->y;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

}

//=============================================================================
//	�v���C���[���̎擾
//=============================================================================
Player* GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
//	�v���C���[�̃q�b�g����
//=============================================================================
void HitPlayer(int nDamage)
{

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_player.state != PLAYERSTATE_DEATH)
	{
		g_player.nLife -= nDamage;
		if (g_player.nLife <= 0)
		{// �v���C���[�̃��C�t���Ȃ��Ȃ���
			SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetJoypadVibration(9000, 12000);
			if (g_player.nStock > 0)
			{// �v���C���[�̃X�g�b�N���c���Ă�����
				g_player.state = PLAYERSTATE_WAIT;
				g_player.nStock--;
				g_player.nCounterState = 60;

				g_player.pos = D3DXVECTOR3(INIT_POSX, INIT_POSY, 0.0f);	// �ʒu�̏�����
				g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ��ʂ̏�����

				// ���_���W�̐ݒ�
				pVtx[0].pos.x = g_player.pos.x - g_player.fRadius;
				pVtx[0].pos.y = g_player.pos.y - g_player.fRadius;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_player.pos.x + g_player.fRadius;
				pVtx[1].pos.y = g_player.pos.y - g_player.fRadius;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_player.pos.x - g_player.fRadius;
				pVtx[2].pos.y = g_player.pos.y + g_player.fRadius;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_player.pos.x + g_player.fRadius;
				pVtx[3].pos.y = g_player.pos.y + g_player.fRadius;
				pVtx[3].pos.z = 0.0f;
			}
			else
			{
				g_player.state = PLAYERSTATE_DEATH;
			}

			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			g_player.bDisp = false;
		}
		else
		{// �v���C���[�̗̑͂��c���Ă�����
			g_player.state = PLAYERSTATE_DAMAGE;
			g_player.nCounterState = 5;
			SetJoypadVibration(6000, 9000);

			PlaySound(SOUND_LABEL_SE_HIT);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
//	�v���C���[�ƓG�Ƃ̓����蔻��
//=============================================================================
void CollisionPlayertoEnemy(void)
{
	// �G�̏����擾
	Enemy* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->pos.x >= g_player.pos.x - ENEMY_SIZEX - (PLAYER_SIZE / 2) &&
				pEnemy->pos.y >= g_player.pos.y - ENEMY_SIZEY - (PLAYER_SIZE / 2) &&
				pEnemy->pos.x <= g_player.pos.x + ENEMY_SIZEX + (PLAYER_SIZE / 2) &&
				pEnemy->pos.y <= g_player.pos.y + ENEMY_SIZEY + (PLAYER_SIZE / 2) &&
				g_player.state == PLAYERSTATE_NORMAL)
			{// �����G�ƃv���C���[���������Ă�����
				// �q�b�g����
				HitPlayer(MAX_LIFE);
			}
		}
	}
}