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
#include "sound.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OPTION_COST				(1500)		// �I�v�V�����𐶐����邽�߂ɕK�v�ȃX�R�A
#define SPEEDUP_COST			(5)			// ���x���グ�邽�߂ɕK�v�ȃX�R�A
#define SPEED_LIMIT				(1.25f)		// ���x���
#define POWERUP_POSX			(640.0f)
#define POWERUP_POSY			(608.0f)
#define POWERUP_SIZEX			(64.0f)
#define POWERUP_SIZEY			(64.0f)

//*****************************************************************************
// �p���[�A�b�v�\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	bool bDisp;			// �\�����
}PowerUp;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
bool g_bSpeedUp = false;
LPDIRECT3DTEXTURE9 g_pTexturePowerUp = NULL;				// �p���[�A�b�v�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPowerUp = NULL;			// �p���[�A�b�v�̒��_�o�b�t�@�ւ̃|�C���^
PowerUp g_powerup[MAX_OPTION] = {};
bool bPowerUpTutorialState = false;

//====================================
//	�p���[�A�b�v�̏���������
//====================================
void InitPowerup(void)
{
	g_bSpeedUp = false;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntPowerUp = 0;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\OptionIcon.png", &g_pTexturePowerUp);


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OPTION,		// �ő吔����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPowerUp,
		NULL);

	bPowerUpTutorialState = false;

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffPowerUp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPowerUp = 0; nCntPowerUp < MAX_OPTION; nCntPowerUp++)
	{
		g_powerup[nCntPowerUp].bDisp = true;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX), POWERUP_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX) + POWERUP_SIZEX, POWERUP_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX), POWERUP_POSY + POWERUP_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(POWERUP_POSX + (nCntPowerUp * POWERUP_SIZEX) + POWERUP_SIZEX, POWERUP_POSY + POWERUP_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPowerUp->Unlock();
}

//====================================
//	�p���[�A�b�v�̏I������
//====================================
void UninitPowerup(void)
{
		// �e�N�X�`���̔j��
	if (g_pTexturePowerUp != NULL)
	{
		g_pTexturePowerUp->Release();
		g_pTexturePowerUp = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPowerUp != NULL)
	{
		g_pVtxBuffPowerUp->Release();
		g_pVtxBuffPowerUp = NULL;
	}
}

//====================================
//	�p���[�A�b�v�̍X�V����
//====================================
void UpdatePowerup(void)
{
	D3DXVECTOR3* pOptionStandardRot = GetStandardRot();
	Player* pPlayer = GetPlayer();
	int nUseOption = GetUseOption();

	if (GetTutorialEvent() == TUTORIALEVENT_POWERUP)
	{
		bPowerUpTutorialState = true;
	}

	if (bPowerUpTutorialState == false)
	{
		return;
	}

	for (int nCntPowerUp = 0; nCntPowerUp < MAX_OPTION; nCntPowerUp++)
	{
		if (nCntPowerUp < nUseOption)
		{
			g_powerup[nCntPowerUp].bDisp = false;
		}
	}


	if (GetJoypadTrigger(JOYKEY_X) == true || GetKeyboardTrigger(DIK_LCONTROL) == true || GetKeyboardTrigger(DIK_RCONTROL) == true)
	{
		if (GetScore() >= OPTION_LOWESTLINE && nUseOption < MAX_OPTION)
		{
			if (GetGameState() == GAMESTATE_TUTORIAL && nUseOption > 0)
			{

			}
			else
			{
				SetOption(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, 0.0f), 64.0f, (pOptionStandardRot->z) * D3DX_PI);
				SubScore(OPTION_COST);
			}
		}
	}

	g_bSpeedUp = false;

	if (GetJoypadPress(JOYKEY_B) == true || GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
	{
		if (GetScore() >= SPEEDUP_LOWESTLINE)
		{
			PlaySound(SOUND_LABEL_SE_SPEEDUP000);
			if (pPlayer->fSpeed < SPEED_LIMIT)
			{
				pPlayer->fSpeed += 0.025f;
			}
			g_bSpeedUp = true;
			SubScore(SPEEDUP_COST);
		}
	}
	
	if (pPlayer->fSpeed >= PLAYER_SPEED && g_bSpeedUp == false)
	{
		pPlayer->fSpeed -= 0.25f;

		if (pPlayer->fSpeed < PLAYER_SPEED)
		{
			pPlayer->fSpeed = PLAYER_SPEED;
		}
	}
}

//====================================
//	�p���[�A�b�v�̕`�揈��
//====================================
void DrawPowerup(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPowerUp, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePowerUp);

	for (int nCntPowerUp = 0; nCntPowerUp < MAX_OPTION; nCntPowerUp++)
	{
		if (g_powerup[nCntPowerUp].bDisp == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPowerUp * 4, 2);
		}
	}
}

//====================================
//	�p���[�A�b�v�̏��擾
//====================================
bool GetPowerUp(void)
{
	return g_bSpeedUp;
}