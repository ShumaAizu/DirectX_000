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
#include "bossparts.h"
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
#define INIT_POSX		(WARLD_WIDTH / 2)			// �����ʒuX
#define INIT_POSY		(WARLD_HEIGHT / 2)			// �����ʒuY

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;				// �v���C���[�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		// �v���C���[�̒��_�o�b�t�@�ւ̃|�C���^
float g_fLengthPlayer = NULL;
float g_fAnglePlayer = NULL;
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
		"data\\TEXTURE\\player0001.png",
		&g_pTexturePlayer);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	g_fLengthPlayer = SQRTF(PLAYER_SIZE, PLAYER_SIZE);
	g_fAnglePlayer = atan2f(PLAYER_SIZE, PLAYER_SIZE);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	g_player.pos = D3DXVECTOR3(INIT_POSX, INIT_POSY, 0.0f);	// �ʒu
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 3.14f);			// ����������������
	g_player.fSpeed = PLAYER_SPEED;							// ���x��������
	g_player.fRadius = PLAYER_SIZE;							// �v���C���[�̑傫���̏�����
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
	float fRot = g_player.rot.z - D3DX_PI;	// �v���C���[�̌����̔���

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
			g_player.nCounterState = 60;				// ��Ԏ������ԃZ�b�g
			g_player.bDisp = true;						// �\����Ԃ�
		}
		return;

	case PLAYERSTATE_NORMAL:
		// �p�[�e�B�N���ݒ�
		if (fRot < -D3DX_PI)
		{
			fRot += D3DX_PI * 2;
		}
		else if (fRot > D3DX_PI)
		{
			fRot -= D3DX_PI * 2;
		}

		//SetRainbowParticle(g_player.pos, 3000.0f, 15);
		SetParticle(g_player.pos, D3DXCOLOR(0.25f, 0.1f, 0.25f, 1.0f), 3000.0f, 30, fRot, fRot);
		CollisionPlayertoEnemy();
		CollisionPlayertoBossParts();
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;		// ��Ԏ������Ԃ����炷
		SetRainbowParticle(g_player.pos, 5000.0f, 5, D3DX_PI, -D3DX_PI);
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

			g_player.state = PLAYERSTATE_NORMAL;		// ��Ԃ̏�����
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

	if (GetKeyboardRepeat(DIK_SPACE) == true || GetJoypadRepeat(JOYKEY_A) == true)
	{//SPACE�L�[�������ꂽ

		Option* pOption = GetOption();

		PlaySound(SOUND_LABEL_SE_SHOT001);

		//�e�̐ݒ�
		SetHomingBullet(g_player.pos - D3DXVECTOR3(20.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 15.0f, g_player.rot.z + (0.15f * D3DX_PI), 250, 15);
		SetHomingBullet(g_player.pos + D3DXVECTOR3(20.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 15.0f, g_player.rot.z - (0.15f * D3DX_PI), 250, 15);

		//�e�̐ݒ�
		for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
		{
			if ((pOption + nCntOption)->bUse == true)
			{
				SetEnemyBullet((pOption + nCntOption)->pos, 25.0f, 50, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL, ((pOption + nCntOption)->rot.z* D3DX_PI + g_player.rot.z));
			}
		}
	}

	float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

	float fMoveKeyboard;

	D3DXVECTOR3 KeyboardMove = {0.0f, 0.0f, 0.0f};

	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//A�L�[�������ꂽ
		KeyboardMove.x -= 1;
	}

	if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//D�L�[�������ꂽ
		KeyboardMove.x += 1;
	}

	if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{//W�L�[�������ꂽ
		KeyboardMove.y -= 1;
	}

	if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{//S�L�[�������ꂽ
		KeyboardMove.y += 1;
	}

	fMoveKeyboard = SQRTF(KeyboardMove.x, KeyboardMove.y);

	if (fMoveKeyboard != 0)
	{
		fRotMove = g_player.rot.z;								// ���̌���
		fRotDest = atan2f(KeyboardMove.x, KeyboardMove.y);		// �ړI�n�ւ̌���
		fRotDiff = fRotDest - fRotMove;							// ����

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		fRotMove += fRotDiff * 0.65f;

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		// �ړ��ʂ̍X�V
		g_player.move.x += sinf(fRotMove) * g_player.fSpeed;
		g_player.move.y += cosf(fRotMove) * g_player.fSpeed;
		g_player.rot.z = atan2f(g_player.move.x, g_player.move.y);
	}

	// �W���C�p�b�h�̏�Ԃ��擾
	XINPUT_STATE* pJoyState = GetJoypadState();

	// �X�e�B�b�N����
	float fThumbLX = pJoyState->Gamepad.sThumbLX;	// L�X�e�B�b�N��X�̒l
	float fThumbLY = pJoyState->Gamepad.sThumbLY;	// L�X�e�B�b�N��Y�̒l

	float fJoypadMove;		// �x�N�g��

	// �x�N�g���̒������Z�o
	fJoypadMove = sqrtf(fThumbLX * fThumbLX + fThumbLY * fThumbLY) * 0.5f;

	if (fJoypadMove > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// �����f�b�h�]�[���ȓ��Ȃ�
		// ���K��
		fThumbLX = (fThumbLX) / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		fThumbLY = (fThumbLY) / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		fRotMove = g_player.rot.z;					// ���̌���
		fRotDest = atan2f(fThumbLX, -fThumbLY);		// �ړI�n�ւ̌���
		fRotDiff = fRotDest - fRotMove;				// ����

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		fRotMove += fRotDiff * 0.65f;

		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}

		// �ړ��ʂ̍X�V
		g_player.move.x += sinf(fRotMove) * g_player.fSpeed;
		g_player.move.y += cosf(fRotMove) * g_player.fSpeed;
		g_player.rot.z = atan2f(g_player.move.x, g_player.move.y);
	}
	else
	{// �f�b�h�]�[���O�Ȃ�
		// ������
		fJoypadMove = 0.0f;
		fThumbLX = 0.0f;
		fThumbLY = 0.0f;
	}

	g_player.pos += g_player.move;

	 //�������X�V
	g_player.move.x += (0.0f - g_player.move.x) * 0.05f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.05f;

	// �[�ɍs�����甽�΂Ɉړ�����
	if (g_player.pos.x < 0)
	{
		g_player.pos.x = 0;
	}
	if (g_player.pos.x > WARLD_WIDTH)
	{
		g_player.pos.x = WARLD_WIDTH;
	}

	if (g_player.pos.y < 0)
	{
		g_player.pos.y = 0;
	}
	if (g_player.pos.y > WARLD_HEIGHT)
	{
		g_player.pos.y = WARLD_HEIGHT;
	}

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x - pCameraPos->x + sinf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y - pCameraPos->y + cosf(g_player.rot.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
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
		SubScore(nDamage);
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;
		SetJoypadVibration(6000, 9000, 30);

		PlaySound(SOUND_LABEL_SE_HIT001);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
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
			float fLenght = SQRTF((g_player.pos.x - pEnemy->pos.x), (g_player.pos.y - pEnemy->pos.y));

			if (fLenght <= g_player.fRadius + pEnemy->fRadius &&
				g_player.state == PLAYERSTATE_NORMAL && pEnemy->state != ENEMYSTATE_APPEAR)
			{// �����G�ƃv���C���[���������Ă�����
				// �q�b�g����
				HitPlayer(1500);
			}
		}
	}
}

//=============================================================================
//	�v���C���[�ƃ{�X���i�Ƃ̓����蔻��
//=============================================================================
void CollisionPlayertoBossParts(void)
{
	// �G�̏����擾
	BossParts* pBossParts = GetBossParts();

	for (int nCntEnemy = 0; nCntEnemy < MAX_BOSSPARTS; nCntEnemy++, pBossParts++)
	{
		if (pBossParts->bUse == true)
		{
			float fLenght = SQRTF((g_player.pos.x - pBossParts->pos.x), (g_player.pos.y - pBossParts->pos.y));

			if (fLenght <= g_player.fRadius + pBossParts->fRadius &&
				g_player.state == PLAYERSTATE_NORMAL && pBossParts->state != ENEMYSTATE_APPEAR)
			{// �����G�ƃv���C���[���������Ă�����
				// �q�b�g����
				HitPlayer(1500);
			}
		}
	}
}